#include "ForthEditor.hpp"
#include "Config.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
ForthDocument::ForthDocument(Glib::RefPtr<Gsv::Language> language)
  : TextDocument(language),
    m_tab_sm(ForthAutoCompletSMBegin)
{
  // Tag for Forth word not in dictionary
  m_tag_unknown_word = m_buffer->create_tag("error");
  m_tag_unknown_word->property_underline() = Pango::UNDERLINE_ERROR;
  m_tag_unknown_word->property_style() = Pango::STYLE_ITALIC;

  // Tag for immediate Forth words
  m_tag_immediate_word = m_buffer->create_tag("immediate");
  m_tag_immediate_word->property_foreground() = "#FFA000";
  m_tag_immediate_word->property_weight() = Pango::WEIGHT_BOLD;

  // Signal for highlighting unknow Forth words aor immediate words.
  m_buffer->signal_insert().connect(sigc::mem_fun(this, &ForthDocument::onInsertText));
}

// *************************************************************************************************
//! \param keyval the value of the key pressed by the user when typing in the Forth editor
// *************************************************************************************************
void ForthDocument::autoCompleteWord(const int keyval)
{
  if (GDK_KEY_Tab == keyval)
    {
      // Get the iterator from the position of the cursor
      Gtk::TextBuffer::iterator cursor = m_buffer->get_iter_at_mark(m_buffer->get_insert());
      Gtk::TextBuffer::iterator start(cursor);

      // Remove the space/tabulator inserted during the state ForthAutoCompletSMBegin
      if (ForthAutoCompletSMEnd == m_tab_sm)
        {
          skipBackwardSpaces(start);
        }

      // The tabulator has not yet been inserted so no need to skip spaces
      skipBackwardWord(start);
      if (ForthAutoCompletSMBegin == m_tab_sm)
        {
          // Several space between the previous word: no completion allowed
          if (start == cursor)
            {
              // but remove the space because it (keyval) will be inserted
              m_buffer->erase(--start, cursor);
              return ;
            }
          // Extract the word to be auto-completed
          m_partial_word = m_buffer->get_text(start, cursor).raw();
          // Next step
          m_tab_sm = ForthAutoCompletSMEnd;
        }

      SimForth& forth = SimForth::instance();
      const char* completed_word = forth.completion(m_partial_word);
      if (NULL != completed_word)
        {
          // A Forth word has been found in the dictionary;
          // Replace the word by the found one.
          m_buffer->erase(start, cursor);
          cursor = m_buffer->get_iter_at_mark(m_buffer->get_insert());
          m_buffer->insert(cursor, completed_word);
        }
      else
        {
          // No Forth word found: restart the state machine
          m_tab_sm = ForthAutoCompletSMBegin;
        }
    }
  else
    {
      // User did not press the tabulator: restart the state machine
      m_tab_sm = ForthAutoCompletSMBegin;
    }
}

// *************************************************************************************************
//!
// *************************************************************************************************
void ForthDocument::skipBackwardWord(Gtk::TextBuffer::iterator& iter)
{
  while (1)
    {
      if (!iter.backward_char())
        return ;

      if (isspace(iter.get_char()))
        {
          iter.forward_char();
          return ;
        }
    }
}

// *************************************************************************************************
//!
// *************************************************************************************************
void ForthDocument::skipBackwardSpaces(Gtk::TextBuffer::iterator& iter)
{
  while (1)
    {
      if (!iter.backward_char())
        return ;

      if (!isspace(iter.get_char()))
        {
          iter.forward_char();
          return ;
        }
    }
}

// *************************************************************************************************
// Slot. FIXME: gerer les commentaires
// *************************************************************************************************
void ForthDocument::onInsertText(const Gtk::TextBuffer::iterator& pos1,
                                 const Glib::ustring& text_inserted,
                                 __attribute__((unused)) int bytes)
{
  // FIXME: enlever les tags
  // New char inserted
  std::string c = text_inserted.raw();
  // std::cout << "c: '" <<  text_inserted.raw() << "'" << std::endl;

  if (isspace(c[0])) // Pas bon !! il faut faire une boucle text_inserted peut etre un gros morceau de code
    {
      SimForth& forth = SimForth::instance();

      Gtk::TextBuffer::iterator pos(pos1);
      skipBackwardSpaces(pos);
      Gtk::TextBuffer::iterator start(pos);
      skipBackwardWord(start);
      std::string partial_word = m_buffer->get_text(start, pos).raw();
      //std::cout << "WORD '" << partial_word << "'" << std::endl;

      // TODO: not in a comment
      {
        // Mark unknown word. FIXME underline IMMEDIATE words
        Cell16 token;
        bool immediate;
        if (forth.dictionary().find(partial_word, token, immediate))
          {
            if (immediate)
              {
                m_buffer->apply_tag(m_tag_immediate_word, start, pos);
              }
          }
        else
          {
            Cell32 val;
            if (false == forth.toNumber(partial_word, val))
              {
                // Check if not a definition
                Gtk::TextBuffer::iterator p1(start);
                skipBackwardSpaces(p1);
                Gtk::TextBuffer::iterator p2(p1);
                skipBackwardWord(p1);
                partial_word = m_buffer->get_text(p1, p2).raw();
                if (0 != partial_word.compare(":"))
                  {
                    m_buffer->apply_tag(m_tag_unknown_word, start, pos);
                  }
              }
          }
      }
    }

  // FIXME: reset completion state
}

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::ForthEditor()
  : m_cout(std::cout, m_results.get_buffer()),
    m_cerr(std::cerr, m_messages.get_buffer()),
    m_nb_plugins(0)
{
  // Forth toolbar (horizontal)
  {
    {
      Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());
      button->set_label("Exec");
      button->set_stock_id(Gtk::Stock::EXECUTE);
      button->set_tooltip_text("Run Forth script");
      m_toolbar.append(*button, sigc::mem_fun(*this, &ForthEditor::exec));
    }
    m_toolbar.append(m_separator[1]);
  }

  // Menu '_Plugins'
  {
    m_menuitem[simtadyn::PlugginsMenu].set_label("_Plugins");
    m_menuitem[simtadyn::PlugginsMenu].set_use_underline(true);
    m_menuitem[simtadyn::PlugginsMenu].set_submenu(m_menu[simtadyn::PlugginsMenu]);
  }

  // Menu '_Forth'
  {
    m_menuitem[simtadyn::ForthMenu].set_label("_Forth");
    m_menuitem[simtadyn::ForthMenu].set_use_underline(true);
    m_menuitem[simtadyn::ForthMenu].set_submenu(m_menu[simtadyn::ForthMenu]);

    //
    m_submenu[0].set_label("New Script");
    m_image[0].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[0].set_image(m_image[0]);
    m_submenu[0].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));
    m_menu[simtadyn::ForthMenu].append(m_submenu[0]);

    //
    m_submenu[1].set_label("New Templated Script");
    m_image[1].set_from_icon_name("text-x-generic-template", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::templated));
    m_menu[simtadyn::ForthMenu].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Interactive");
    m_image[2].set_from_icon_name("utilities-terminal", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));// TODO
    m_menu[simtadyn::ForthMenu].append(m_submenu[2]);

    //
    m_menu[simtadyn::ForthMenu].append(m_menuseparator[0]);

    //
    m_submenu[3].set_label("Open Script");
    m_image[3].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    //FIXME    m_submenu[3].signal_activate().connect(sigc::mem_fun0<bool>(*this, &ForthEditor::open));
    m_menu[simtadyn::ForthMenu].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save Script");
    m_image[4].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::save));
    m_menu[simtadyn::ForthMenu].append(m_submenu[4]);

    //
    m_submenu[5].set_label("Save As Script");
    m_image[5].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect(sigc::mem_fun0(*this, &ForthEditor::saveAs));
    m_menu[simtadyn::ForthMenu].append(m_submenu[5]);

    //
    m_menu[simtadyn::ForthMenu].append(m_menuseparator[1]);

    //
    m_submenu[9].set_label("Load Dictionary");
    m_image[9].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[9].set_image(m_image[9]);
    m_submenu[9].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::loadDictionary));
    m_menu[simtadyn::ForthMenu].append(m_submenu[9]);

    //
    m_submenu[10].set_label("Save As Dictionary");
    m_image[10].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[10].set_image(m_image[10]);
    m_submenu[10].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::dumpDictionary));
    m_menu[simtadyn::ForthMenu].append(m_submenu[10]);
  }

  // Forth dictionary display
  {
    m_ref_tree_model = Gtk::ListStore::create(m_columns);
    m_dico.set_model(m_ref_tree_model);

    m_dico.append_column("Word", m_columns.m_word);
    m_dico.append_column("Token", m_columns.m_token);

    // Test
    Gtk::TreeModel::Row row = *(m_ref_tree_model->append());
    row[m_columns.m_word] = "Billy";
    row[m_columns.m_token] = 42;

    row = *(m_ref_tree_model->append());
    row[m_columns.m_word] = "Foo";
    row[m_columns.m_token] = 43;
  }

  // Forth text view for storing results, debug, historic
  {
    // FIXME: mettre les text view en read-only
    m_scrolledwindow[simtadyn::ForthResTab].add(m_results);
    m_scrolledwindow[simtadyn::ForthResTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[simtadyn::ForthHistoryTab].add(m_history);
    m_scrolledwindow[simtadyn::ForthHistoryTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[simtadyn::ForthDicoTab].add(m_dico);
    m_scrolledwindow[simtadyn::ForthDicoTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[simtadyn::ForthMsgTab].add(m_messages);
    m_scrolledwindow[simtadyn::ForthMsgTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  }

  // Forth detachable notebook
  {
    m_hpaned.pack1(m_res_notebooks[0]);
    m_res_notebooks[0].set_group_name("forth_res_notebooks");
    m_res_notebooks[0].append_page(m_scrolledwindow[simtadyn::ForthResTab], "_Result", true);
    m_res_notebooks[0].set_tab_detachable(m_scrolledwindow[simtadyn::ForthResTab], true);
    m_res_notebooks[0].append_page(m_scrolledwindow[simtadyn::ForthHistoryTab], "H_istory", true);
    m_res_notebooks[0].set_tab_detachable(m_scrolledwindow[simtadyn::ForthHistoryTab], true);
    m_res_notebooks[0].append_page(m_scrolledwindow[simtadyn::ForthDicoTab], "_Dico", true);
    m_res_notebooks[0].set_tab_detachable(m_scrolledwindow[simtadyn::ForthDicoTab], true);

    m_hpaned.pack2(m_res_notebooks[1]);
    m_res_notebooks[1].set_group_name("forth_res_notebooks");
    m_res_notebooks[1].append_page(m_scrolledwindow[simtadyn::ForthMsgTab], "_Messages", true);
    m_res_notebooks[1].set_tab_detachable(m_scrolledwindow[simtadyn::ForthMsgTab], true);

    // Scroll to the end of the document when inserting text
    // TBD: ouvrir l'onglet concerne ?
    m_messages.get_buffer()->signal_insert().connect(
    [this](const Gtk::TextIter, const Glib::ustring, int)
      {
        Gtk::TextBuffer::iterator iter = m_messages.get_buffer()->end();
        m_messages.scroll_to(iter);
      });
  }

  // Statusbar
  {
    m_statusbar.push("Welcome to SimTaDyn !");
  }
}

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::~ForthEditor()
{
  // TODO: save the historic buffer
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::autoCompleteWord(const int keyval)
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      doc->autoCompleteWord(keyval);
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::empty()
{
  TextEditor::empty("New Forth script");
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::templated()
{
  ForthEditor::empty();
  TextEditor::document()->buffer()->set_text(": COMPUTE-ME 1 1 + . ;\nCOMPUTE-ME");
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::dumpDictionary()
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for((Gtk::Window&) (*m_notebook.get_toplevel()));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_forth = Gtk::FileFilter::create();
  filter_forth->set_name("Forth dictionary files");
  filter_forth->add_pattern("*.simdico");
  dialog.add_filter(filter_forth);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      SimForth& forth = SimForth::instance();
      forth.dictionary().dump(dialog.get_filename());
      // FIXME return not taken into account
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::loadDictionary()
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_notebook.get_toplevel()));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_forth = Gtk::FileFilter::create();
  filter_forth->set_name("Forth dictionary files");
  filter_forth->add_pattern("*.simdico");
  dialog.add_filter(filter_forth);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      SimForth& forth = SimForth::instance();
      forth.dictionary().load(dialog.get_filename());
      // FIXME return not taken into account
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
std::string ForthEditor::elapsedTime()
{
  using namespace std;
  using namespace std::chrono;
  typedef duration<int, ratio<86400>> days;
  std::chrono::nanoseconds ns = m_elapsed_time;

  stringstream ss;
  char fill = ss.fill();

  ss.fill('0');
  auto d = duration_cast<days>(ns);
  ns -= d;
  auto h = duration_cast<hours>(ns);
  ns -= h;
  auto m = duration_cast<minutes>(ns);
  ns -= m;
  auto s = duration_cast<seconds>(ns);
  ns -= s;
  auto ms = duration_cast<milliseconds>(ns);
  ns -= ms;
  auto us = duration_cast<microseconds>(ns);
  ns -= us;

  ss << setw(2) << d.count() << "d:"
     << setw(2) << h.count() << "h:"
     << setw(2) << m.count() << "m:"
     << setw(2) << s.count() << "s:"
     << setw(2) << ms.count() << "ms:"
     << setw(2) << us.count() << "us";
  ss.fill(fill);
  return ss.str();
}

// *************************************************************************************************
// Get all text in the current text editor and give it to the Forth interpreter
// Return true if the code was interpreted correctly, else return false.
// *************************************************************************************************
bool ForthEditor::exec_(std::string const& script, std::string const& filename)
{
  std::cerr << "Exec_ " << script << " " << filename << std::endl;

  typedef std::chrono::nanoseconds ns;
  typedef std::chrono::high_resolution_clock Time;
  std::pair<bool, std::string> res;
  SimForth& forth = SimForth::instance();

  // Clear the old text in the "Result" tab of the notebook
  Glib::RefPtr<Gtk::TextBuffer> buf = m_results.get_buffer();
  buf->erase(buf->begin(), buf->end());

  // Exec the Forth script and  measure the execution time
  auto t0 = Time::now();
  res = forth.interpreteString(script, filename);
  auto t1 = Time::now();

  // Flush the std::cout in the textview
  //m_cout.flush();
  //m_cerr.flush();

  if (res.first)
    {
      m_elapsed_time = std::chrono::duration_cast<ns>(t1 - t0);
      m_statusbar.push(elapsedTime());

      // Paste the script Forth result in the "Result" tab of the notebook
      buf->insert(buf->end(), res.second);

      // Copy paste the Forth script into the historic buffer
      buf = m_history.get_buffer();
      buf->insert(buf->end(), script);
      buf->insert(buf->end(), "\n\n");

      // TODO: inserer nouveau mot dans tree
      return false;
    }
  else
    {
      // Text view: indiquer ligne ko
      m_statusbar.push("FAILED");

      // Show res (redirect sdout to gui)
      forth.ok(res);
      return true;
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::execButton(Gtk::ToolButton* button)
{
  // FIXME: ajouter le numero du bouton dans le nom pour eviter
  const char *name = "button";

  // Forbid to exec the script if it is currently modified by the user
  // and not saved.
  TextDocument *doc = tab(name);
  if ((nullptr != doc) && (doc->isModified()))
    {
      Gtk::MessageDialog dialog((Gtk::Window&) (*m_notebook.get_toplevel()),
                                "The document '" + doc->title() +
                                "' has been modified. Do you want to save it now before running its script ?",
                                false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);

      int result = dialog.run();
      if (Gtk::RESPONSE_YES == result)
        {
          button->set_label(doc->utext());
          button->set_tooltip_text(doc->utext());
          doc->modified(false);
        }
      else
        {
          // Do not exec the script forth while in edition
          m_statusbar.push("Use ignored saving the Forth script button"); // FIXME: inutile car ecrase par le resultat de l'exec
        }
    }

  if (ForthEditor::exec_(button->get_label().raw(), name))
    {
      TextDocument *doc = TextEditor::addTab(name);
      doc->clear();
      doc->appendText(button->get_label());
      doc->modified(false);
    }
  // FIXME: quand on sauvegarde ne pas stocker dans un fichier mais dans le bouton
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::execMenu(std::string const& script)
{
  const char *name = "menu";
  if (ForthEditor::exec_(script, name))
    {
      TextDocument *doc = TextEditor::addTab(name);
      doc->clear();
      doc->appendText(script);
    }
  // FIXME: quand on sauvegarde ne pas stocker dans un fichier mais dans le menu
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::exec()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      bool ret = ForthEditor::exec_(doc->text(), doc->filename());

      // FIXME: Clear the text editor if and only if we are in an interactive mode
      if (!ret /*&& (m_interactive)*/)
      {
        doc->clear();
      }
    }
  else
    {
      // Show the faulty document
      TextEditor::open(SimForth::instance().nameStreamInFault());
      // TODO: select in red the faulty word

      m_statusbar.push("Please, feed me with a Forth script !");
    }
}

// FIXME const Cell16 ForthToken)
// **************************************************************
// FIXME: si pile vide ou pas le bon nombre d'elements alors fenetre popup qui demande les param
// FIXME: ajouter le postip avec la definiton du mot "WORD ( n1 n2 -- n3 n4 )"
// FIXME ne pas autoriser a compiler
// **************************************************************
Gtk::ToolButton *ForthEditor::addButon(const Gtk::BuiltinStockID icon,
                                       const std::string &script,
                                       const std::string &help)
{
  Gtk::ToolButton *button = Gtk::manage(new Gtk::ToolButton());

  if (nullptr != button)
    {
      button->set_label(script);
      button->set_stock_id(icon);
      button->set_tooltip_text(help);
      m_toolbar.append(*button, sigc::bind<Gtk::ToolButton*>
        (sigc::mem_fun(*this, &ForthEditor::execButton), button));
      m_toolbar.show_all_children();
    }
  else
    {
      //FIXME Gtk::MessageDialog dialog(*this, "Failed creating a Forth button");
      //dialog.run();
    }
  return button;
}

// **************************************************************
//
// **************************************************************
uint32_t ForthEditor::addPluggin(const Glib::ustring& icon_name,
                                 const std::string &script,
                                 const std::string &help)
{
  m_plugins_submenu[m_nb_plugins].set_label(help);
  m_plugins_image[m_nb_plugins].set_from_icon_name(icon_name, Gtk::ICON_SIZE_MENU);
  m_plugins_submenu[m_nb_plugins].set_image(m_plugins_image[m_nb_plugins]);
  m_plugins_submenu[m_nb_plugins].signal_activate().connect(
    sigc::bind<const std::string>(sigc::mem_fun(*this, &ForthEditor::execMenu), script));
  m_menu[simtadyn::PlugginsMenu].append(m_plugins_submenu[m_nb_plugins]);
  ++m_nb_plugins;
  return m_nb_plugins - 1U;
}

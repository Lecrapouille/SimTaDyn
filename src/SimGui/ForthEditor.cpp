#include "ForthEditor.hpp"
#include "SimTaDynContext.hpp"

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

      SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
      const char* completed_word = simtadyn.m_forth.completion(m_partial_word);
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
      SimTaDynContext& simtadyn = SimTaDynContext::getInstance();

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
        if (simtadyn.m_forth.dictionary().find(partial_word, token, immediate))
          {
            if (immediate)
              {
                m_buffer->apply_tag(m_tag_immediate_word, start, pos);
              }
          }
        else
          {
            Cell32 val;
            if (false == simtadyn.m_forth.toNumber(partial_word, val))
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
    m_cerr(std::cerr, m_messages.get_buffer())
{
  // Menus '_Forth Scripts'
  {
    m_menuitem[1].set_label("_Forth");
    m_menuitem[1].set_use_underline(true);
    m_menuitem[1].set_submenu(m_menu[1]);

    //
    m_submenu[0].set_label("New Script");
    m_image[0].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[0].set_image(m_image[0]);
    m_submenu[0].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));
    m_menu[1].append(m_submenu[0]);

    //
    m_submenu[1].set_label("New Templated Script");
    m_image[1].set_from_icon_name("text-x-generic-template", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::templated));
    m_menu[1].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Interactive");
    m_image[2].set_from_icon_name("utilities-terminal", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::empty));// TODO
    m_menu[1].append(m_submenu[2]);

    //
    m_menu[1].append(m_menuseparator[0]);

    //
    m_submenu[3].set_label("Open Script");
    m_image[3].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect(sigc::mem_fun0(*this, &TextEditor::open));
    m_menu[1].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save Script");
    m_image[4].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::save));
    m_menu[1].append(m_submenu[4]);

    //
    m_submenu[5].set_label("Save As Script");
    m_image[5].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect(sigc::mem_fun0(*this, &ForthEditor::saveAs));
    m_menu[1].append(m_submenu[5]);

    //
    m_menu[1].append(m_menuseparator[1]);

    //
    m_submenu[9].set_label("Load Dictionary");
    m_image[9].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[9].set_image(m_image[9]);
    m_submenu[9].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::loadDictionary));
    m_menu[1].append(m_submenu[9]);

    //
    m_submenu[10].set_label("Save As Dictionary");
    m_image[10].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[10].set_image(m_image[10]);
    m_submenu[10].signal_activate().connect(sigc::mem_fun(*this, &ForthEditor::dumpDictionary));
    m_menu[1].append(m_submenu[10]);
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
    m_scrolledwindow[ForthResTab].add(m_results);
    m_scrolledwindow[ForthResTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[ForthHistoryTab].add(m_history);
    m_scrolledwindow[ForthHistoryTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[ForthDicoTab].add(m_dico);
    m_scrolledwindow[ForthDicoTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolledwindow[ForthMsgTab].add(m_messages);
    m_scrolledwindow[ForthMsgTab].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  }

  // Forth detachable notebook
  {
    m_hpaned.pack1(m_res_notebooks[0]);
    m_res_notebooks[0].set_group_name("forth_res_notebooks");
    m_res_notebooks[0].append_page(m_scrolledwindow[ForthResTab], "_Result", true);
    m_res_notebooks[0].set_tab_detachable(m_scrolledwindow[ForthResTab], true);
    m_res_notebooks[0].append_page(m_scrolledwindow[ForthHistoryTab], "H_istory", true);
    m_res_notebooks[0].set_tab_detachable(m_scrolledwindow[ForthHistoryTab], true);
    m_res_notebooks[0].append_page(m_scrolledwindow[ForthDicoTab], "_Dico", true);
    m_res_notebooks[0].set_tab_detachable(m_scrolledwindow[ForthDicoTab], true);

    m_hpaned.pack2(m_res_notebooks[1]);
    m_res_notebooks[1].set_group_name("forth_res_notebooks");
    m_res_notebooks[1].append_page(m_scrolledwindow[ForthMsgTab], "_Messages", true);
    m_res_notebooks[1].set_tab_detachable(m_scrolledwindow[ForthMsgTab], true);

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
      SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
      simtadyn.m_forth.dictionary().dump(dialog.get_filename());
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
      SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
      simtadyn.m_forth.dictionary().load(dialog.get_filename());
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
void ForthEditor::exec1(const std::string &script)
{
  typedef std::chrono::nanoseconds ns;
  typedef std::chrono::high_resolution_clock Time;
  std::pair<bool, std::string> res;
  SimTaDynContext& simtadyn = SimTaDynContext::getInstance();

  // Clear the old text in the "Result" tab of the notebook
  Glib::RefPtr<Gtk::TextBuffer> buf = m_results.get_buffer();
  buf->erase(buf->begin(), buf->end());

  // Exec the Forth script and  measure the execution time
  auto t0 = Time::now();
  res = simtadyn.m_forth.interpreteString(script, document()->filename());
  auto t1 = Time::now();

  // Flush the std::cout in the textview
  m_cout.flush();
  m_cerr.flush();

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

      // Clear the text editor if and only if we are in an interactive mode
      //if (m_interactive)
      {
        ForthEditor::clear();
      }

      // TODO: inserer nouveau mot dans tree
    }
  else
    {
      // Text view: indiquer ligne ko
      m_statusbar.push("FAILED");

      // Show the faulty document
      TextEditor::open(simtadyn.m_forth.nameStreamInFault());
      // TODO: select in red the faulty word

      // Show res (redirect sdout to gui)
      simtadyn.m_forth.ok(res);
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::exec()
{
  TextDocument* doc = document();

  if (nullptr != doc)
    {
      ForthEditor::exec1(ForthEditor::text().raw());
    }
  else
    {
      m_statusbar.push("Please, feed me with a Forth script !");
    }
}

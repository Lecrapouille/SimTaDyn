//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "ForthEditor.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
ForthDocument::ForthDocument(SimForth& forth, Glib::RefPtr<Gsv::Language> language)
  : TextDocument(language),
    m_forth(forth),
    m_tab_sm(ForthAutoCompletSMBegin)
{
  LOGI("Creating ForthDocument");

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
//
// *************************************************************************************************
ForthDocument::~ForthDocument()
{
  LOGI("Destroying ForthDocument");
}

// *************************************************************************************************
//! \param keyval the value of the key pressed by the user when typing in the Forth editor
// *************************************************************************************************
void ForthDocument::completeForthName(const bool reset_state)
{
  // User did not press the tabulator: restart the state machine
  if (reset_state)
    {
      m_tab_sm = ForthAutoCompletSMBegin;
      return ;
    }

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

  const char* completed_word = m_forth.completion(m_partial_word);
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

// *************************************************************************************************
//!
// *************************************************************************************************
void ForthDocument::skipBackwardWord(Gtk::TextBuffer::iterator& iter)
{
  while (1)
    {
      if (!iter.backward_char())
        return ;

      if (g_unichar_isspace(iter.get_char()))
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

      if (!g_unichar_isspace(iter.get_char()))
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
        if (m_forth.dictionary().find(partial_word, token, immediate))
          {
            if (immediate)
              {
                m_buffer->apply_tag(m_tag_immediate_word, start, pos);
              }
          }
        else
          {
            Cell32 val;
            if (false == m_forth.toNumber(partial_word, val))
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

// FIXME: temporary
#define FORTH_TOOLBAR_PLUGINS 0
#define FORTH_TOOLBAR_CMDS 1

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::ForthEditor(SimForth& forth)
  : m_forth(forth),
    m_dico_inspector(forth),
    m_stack_inspector(forth),
    m_cout(std::cout, m_results.get_buffer()),
    m_cerr(std::cerr, m_messages.get_buffer())
{
  LOGI("Creating ForthEditor");
  m_hbox.pack_start(m_toolbars[FORTH_TOOLBAR_PLUGINS], Gtk::PACK_SHRINK);
  m_hbox.pack_start(m_vbox);
  m_vbox.pack_start(*this, Gtk::PACK_EXPAND_WIDGET);
  m_vbox.pack_start(m_toolbars[FORTH_TOOLBAR_CMDS], Gtk::PACK_SHRINK);
  m_vbox.pack_start(m_statusbar, Gtk::PACK_SHRINK);
  m_vbox.pack_start(m_notebook, Gtk::PACK_EXPAND_WIDGET);

  addNoteBookPage(ForthResTab, m_results, "_Result");
  addNoteBookPage(ForthHistoryTab, m_history, "H_istory");
  addNoteBookPage(ForthMsgTab, m_messages, "_Messages");
  addNoteBookPage(ForthDicoTab, m_dico_inspector.widget(), "_Dico");
  addNoteBookPage(ForthStackTab, m_stack_inspector.widget(), "Data _Stack");

  populateToolBars();
}

void ForthEditor::populateToolBars()
{
  // Horizontal toolbar: Forth commands
  {
    Gtk::Toolbar& toolbar = m_toolbars[FORTH_TOOLBAR_CMDS];
    toolbar.set_property("orientation", Gtk::ORIENTATION_HORIZONTAL);
    toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    // Forth exec button
    {
      Gtk::ToolButton *button = Gtk::make_managed<Gtk::ToolButton>();
      button->set_label("Exec");
      button->set_stock_id(Gtk::Stock::EXECUTE);
      button->set_tooltip_text("Run Forth script");
      toolbar.append(*button, [&]{interpreteCurrentDocument();});
      toolbar.append(m_separator[1]);
    }
  }

  // Vertical toolbar: Forth plugins
  {
    Gtk::Toolbar& toolbar = m_toolbars[FORTH_TOOLBAR_PLUGINS];
    toolbar.set_property("orientation", Gtk::ORIENTATION_VERTICAL);
    toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    // FIXME temporary plugin
    {
      addForthButton(Gtk::Stock::EXECUTE, "42 42 + .", "Plugin example");
    }
  }
}

Gtk::Widget& ForthEditor::widget()
{
  return m_hbox;
}

// *************************************************************************************************
//
// *************************************************************************************************
ForthEditor::~ForthEditor()
{
  LOGI("Destroying ForthEditor");
  // TODO: be sure no script is running on the map before destroying
  // TODO: save the historic buffer
}

//------------------------------------------------------------------
Glib::RefPtr<Gio::Menu> ForthEditor::populatePopovMenu(Gtk::ApplicationWindow& win)
{
  Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();
  //Glib::RefPtr<Gio::Menu> submenu_text_editor = TextEditor::populatePopovMenu();
  Glib::RefPtr<Gio::Menu> submenu_forth_editor = Gio::Menu::create();
  //TODO menu->append_submenu("Text Editor", submenu_text_editor);
  menu->append_submenu("Forth Editor", submenu_forth_editor);

  submenu_forth_editor->append("New Script", "win.script-create-dummy");
  submenu_forth_editor->append("New Template Script", "win.script-create-template");
  submenu_forth_editor->append("Interactive Script", "win.script-interactive");
  submenu_forth_editor->append("Load dictionary", "win.dico-load");
  submenu_forth_editor->append("Dump dictionary", "win.dico-dump");

  win.add_action("script-create-dummy", sigc::mem_fun(*this, &ForthEditor::createEmptyScript));
  win.add_action("script-create-template", sigc::mem_fun(*this, &ForthEditor::createTemplateScript));
  win.add_action("script-interactive", sigc::mem_fun(*this, &ForthEditor::openInteractiveScript));
  win.add_action("dico-load", sigc::mem_fun(*this, &ForthEditor::loadDictionary));
  win.add_action("dico-dump", sigc::mem_fun(*this, &ForthEditor::dumpDictionary));

  return menu;
}

void ForthEditor::addNoteBookPage(uint32_t const nth, Gtk::Widget& widget, const char* label)
{
  m_scrolled[nth].add(widget);
  m_scrolled[nth].set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_notebook.append_page(m_scrolled[nth], label, true);
  m_notebook.set_tab_detachable(m_scrolled[nth], true);
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::statusBarSays(std::string const& message)
{
  m_statusbar.push(message);
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::completeForthName(const bool reset_state)
{
  TextDocument* txt_doc = TextEditor::document();
  if (nullptr == txt_doc)
    return ;

  ForthDocument* doc = dynamic_cast<ForthDocument*>(txt_doc);
  if (nullptr == txt_doc)
    {
      LOGES("Cannot cast TextDocument to ForthDocument");
      return ;
    }
  doc->completeForthName(reset_state);
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::createEmptyScript()
{
  TextEditor::empty("New Forth script");
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::createTemplateScript()
{
  ForthEditor::empty();
  TextDocument* doc = TextEditor::document();
  if (nullptr == doc)
    return ;

  // TODO: use factory pattern
  doc->buffer()->set_text(": COMPUTE-ME 1 1 + . ;\nCOMPUTE-ME");
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::dumpDictionary()
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for((Gtk::Window&) (*m_notebook.get_toplevel()));

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(config::data_path);

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
      m_forth.dictionary().dump(dialog.get_filename());
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

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(config::data_path);

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
      m_forth.dictionary().load(dialog.get_filename());
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
bool ForthEditor::interpreteCurrentDocument()
{
  TextDocument* doc = document();

  if (nullptr == doc)
    {
      m_statusbar.push("Please, feed me with a Forth script !");
      return false;
    }

  bool res = interpreteScript(doc->text(), doc->filename());
  if (res /* && m_interactive*/)
    {
      // FIXME: Clear the text editor if and only if we are in an interactive mode
      doc->clear();
    }
  return res;
}

bool ForthEditor::interpreteScript(std::string const& script, std::string const& filename)
{
  LOGI("ForthEditor executing script '%s': '%s'", filename.c_str(), script.c_str());

  typedef std::chrono::nanoseconds ns;
  typedef std::chrono::steady_clock Time;

  // Clear the old text in the "Result" tab of the notebook
  Glib::RefPtr<Gtk::TextBuffer> buf = m_results.get_buffer();
  buf->erase(buf->begin(), buf->end());

  // Execute the Forth script and measure its execution time
  auto t0 = Time::now();
  std::pair<bool, std::string> res = m_forth.interpreteString(script, filename);
  auto t1 = Time::now();

  // Flush the std::cout in the textview
  m_cout.flush();
  m_cerr.flush();

  if (res.first)
    {
      LOGI("Succeeded executing script '%s'", filename.c_str());

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
      LOGE("Failed executing script '%s'", filename.c_str());

      // Text view: indiquer ligne ko
      m_statusbar.push("FAILED");

      // Show the faulty document
      TextEditor::open(m_forth.nameStreamInFault());
      // TODO: select in red the faulty word

      // Show res (redirect sdout to gui)
      m_forth.ok(res);
      return true;
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void ForthEditor::onForthButtonClicked(Gtk::ToolButton& button)
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
          button.set_label(doc->utext());
          button.set_tooltip_text(doc->utext());
          doc->modified(false);
        }
      else
        {
          // Do not exec the script forth while in edition
          m_statusbar.push("Use ignored saving the Forth script button"); // FIXME: inutile car ecrase par le resultat de l'exec
        }
    }

  if (ForthEditor::interpreteScript(button.get_label().raw(), name))
    {
      doc = TextEditor::addTab(name);
      doc->clear();
      doc->appendText(button.get_label());
      doc->modified(false);
    }
  // FIXME: quand on sauvegarde ne pas stocker dans un fichier mais dans le bouton
}

// *************************************************************************************************
// FIXME const Cell16 ForthToken)
// **************************************************************
// FIXME: si pile vide ou pas le bon nombre d'elements alors fenetre popup qui demande les param
// FIXME: ajouter le postip avec la definiton du mot "WORD ( n1 n2 -- n3 n4 )"
// FIXME ne pas autoriser a compiler
// **************************************************************
Gtk::ToolButton& ForthEditor::addForthButton(Gtk::BuiltinStockID const icon,
                                             std::string const& script,
                                             std::string const& help)
{
  Gtk::ToolButton *button = Gtk::make_managed<Gtk::ToolButton>();

  if (nullptr != button)
    {
      Gtk::Toolbar& toolbar = m_toolbars[FORTH_TOOLBAR_PLUGINS];

      button->set_label(script);
      button->set_stock_id(icon);
      button->set_tooltip_text(help);
      toolbar.append(*button, sigc::bind<Gtk::ToolButton&>
        (sigc::mem_fun(*this, &ForthEditor::onForthButtonClicked), *button));
      toolbar.show_all_children();
    }
  else
    {
      //FIXME Gtk::MessageDialog dialog(*this, "Failed creating a Forth button");
      //dialog.run();
    }
  return *button;
}

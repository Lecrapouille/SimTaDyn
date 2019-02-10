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

#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "ForthInspector.hpp"
#  include "SimTaDynForth.hpp" // FIXME Forth.hpp
#  include "Redirection.hpp"
#  include <chrono>

// *************************************************************************************************
//
// *************************************************************************************************
class ForthDocument: public TextDocument
{
  friend class ForthEditor;

public:

  ForthDocument(SimForth& forth, Glib::RefPtr<Gsv::Language> language);
  ~ForthDocument();

private:

  SimForth& m_forth;
  // FIXME changer le nom de cette fonction
  //! \brief Slot called when text has been inserted. Use it for checking unknown words
  void onInsertText(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text_inserted, int bytes);
  //! \brief Skip the previous word.
  static void skipBackwardWord(Gtk::TextBuffer::iterator& iter);
  //! \brief Skip previous spaces characters.
  static void skipBackwardSpaces(Gtk::TextBuffer::iterator& iter);
  //! \brief Complete a Forth word when the user type on the tabulator key.
  void completeForthName(const bool reset_state);
  //! Gtk tag in textbuffer for highlighting Forth words not present in the dictionary.
  Glib::RefPtr<Gtk::TextTag> m_tag_unknown_word;
    //! Gtk tag in textbuffer for highlighting immediate Forth words.
  Glib::RefPtr<Gtk::TextTag> m_tag_immediate_word;
  //! Extracted word at the first step of the auto-completion algorithm.
  std::string m_partial_word;
  //! States for the auto-completion state-machine algorithm.
  enum ForthAutoCompletSM { ForthAutoCompletSMBegin, ForthAutoCompletSMEnd };
  //! Current state for the auto-completion state-machine algorithm.
  ForthAutoCompletSM m_tab_sm;
};

// *************************************************************************************************
//
// *************************************************************************************************
class ForthEditor
  : public TextEditor
{
  //! \brief Notebook tab names for the Forth editor (text editor):
  //! results of Forth script, history of old forth commands, Forth dictionary, Simtadyn errors.
  enum ForthTabNames { ForthResTab, ForthHistoryTab, ForthDicoTab, ForthStackTab, ForthMsgTab, Max_ };

public:

  ForthEditor(SimForth& forth);
  virtual ~ForthEditor();
  Glib::RefPtr<Gio::Menu> populatePopovMenu(Gtk::ApplicationWindow& win);

  Gtk::Widget& widget();
  void statusBarSays(std::string const& message);
  void createEmptyScript();
  void createTemplateScript();
  void openInteractiveScript() { createTemplateScript(); } // FIXME: not yet implemented
  Gtk::ToolButton& addForthButton(const Gtk::BuiltinStockID icon,
                                  const std::string& script,
                                  const std::string& help);
  uint32_t addForthActionMenu(const Glib::ustring& icon_name,
                              const std::string &script,
                              const std::string &help);
  bool interpreteCurrentDocument();
  bool interpreteScript(std::string const& script, std::string const& filename);
  void loadDictionary();
  void dumpDictionary();
  void completeForthName(const bool reset_state);

private:

  void populateToolBars();
  void addNoteBookPage(uint32_t const nth, Gtk::Widget& widget, const char* label);
  std::string elapsedTime();
  void onForthButtonClicked(Gtk::ToolButton& button);

  inline virtual TextDocument *create() override
  {
    return new ForthDocument(m_forth, m_language);
  }

private:

  SimForth&              m_forth;
  Gtk::VPaned            m_vpaned;
  Gtk::HBox              m_hbox;
  Gtk::VBox              m_vbox;
  Gtk::Notebook          m_notebook;
  Gtk::Toolbar           m_toolbars[2];
  Gtk::Statusbar         m_statusbar;
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::TextView          m_results;
  Gtk::TextView          m_history;
  Gtk::TextView          m_messages;
  Gtk::ScrolledWindow    m_scrolled[ForthTabNames::Max_];
  ForthDicoInspector     m_dico_inspector;
  ForthStackInspector    m_stack_inspector;
  std::chrono::nanoseconds m_elapsed_time;
  streamgui m_cout; // std::cout redirected inside the GUI
  streamgui m_cerr; // std::cerr redirected inside the GUI
};

#endif /* FORTHEDITOR_HPP_ */

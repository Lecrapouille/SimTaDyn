//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "SimTaDynForth.hpp" // FIXME Forth.hpp
#  include "Redirection.hpp"
#  include <chrono>

// *************************************************************************************************
//
// *************************************************************************************************
class ForthDocument: public TextDocument
{
public:
  ForthDocument(Glib::RefPtr<Gsv::Language> language);
  ~ForthDocument();

protected:
  // FIXME changer le nom de cette fonction
  //! \brief Slot called when text has been inserted. Use it for checking unknown words
  void onInsertText(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text_inserted, int bytes);
  //! \brief Skip the previous word.
  void skipBackwardWord(Gtk::TextBuffer::iterator& iter);
  //! \brief Skip previous spaces characters.
  void skipBackwardSpaces(Gtk::TextBuffer::iterator& iter);
  //! \brief Complete a Forth word when the user type on the tabulator key.
  virtual void autoCompleteWord(const int keyval) override;
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
class ForthEditor: public TextEditor, public Singleton<ForthEditor>
{
  friend class Singleton<ForthEditor>;

public:

  ForthEditor();
  virtual ~ForthEditor();
  void empty();
  void templated();
  void exec();
  void execMenu(std::string const& script);
  void execButton(Gtk::ToolButton* button); // FIXME nom
  std::string elapsedTime();
  void loadDictionary();
  void dumpDictionary();
  //! Call the auto-completion on the current document.
  void autoCompleteWord(const int keyval);

  uint32_t addPluggin(const Glib::ustring& icon_name,
                      const std::string &script,
                      const std::string &help);
  Gtk::ToolButton *addButton(const Gtk::BuiltinStockID icon,
                             const std::string &script,
                             const std::string &help);  // FIXME: Glib::ustring, const Cell16 Forthtoken);

  Gtk::Notebook          m_res_notebooks[2]; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow    m_scrolledwindow[4];
  Gtk::ImageMenuItem     m_plugins_submenu[8];
  Gtk::Image             m_plugins_image[8];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::TextView          m_results;
  Gtk::TextView          m_history;
  Gtk::TextView          m_messages;
  Gtk::TreeView          m_dico;
  Gtk::Statusbar         m_statusbar;
  Gtk::Toolbar           m_toolbar;
  Gtk::HPaned            m_hpaned;
  Gtk::VPaned            m_vpaned;
  Gtk::VBox              m_vbox[2];

protected:

  bool exec_(std::string const& script, std::string const& filename);

  inline virtual TextDocument *create() override
  {
    return new ForthDocument(m_language);
  }
  class ModelColumns : public Gtk::TreeModelColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_word);
      add(m_token);
    }

    Gtk::TreeModelColumn<Glib::ustring> m_word;
    Gtk::TreeModelColumn<int> m_token;
  };

  ModelColumns m_columns;
  Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;
  std::chrono::nanoseconds m_elapsed_time;
  streamgui m_cout; // std::cout redirected inside the GUI
  streamgui m_cerr; // std::cerr redirected inside the GUI
  uint32_t m_nb_plugins;
};

#endif /* FORTHEDITOR_HPP_ */

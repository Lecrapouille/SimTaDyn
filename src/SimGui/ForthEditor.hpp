#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "SimForth.hpp"
#  include <chrono>
#  include "Redirection.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class ForthDocument: public TextDocument
{
public:
  ForthDocument(Glib::RefPtr<Gsv::Language> language);

protected:
  // FIXME changer le nom de cette fonction
  //! \breif Slot called when text has been inserted. Use it for checking unknown words
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
class ForthEditor : public TextEditor
{
public:
  enum ForthTabNames { ForthResTab, ForthHistoryTab, ForthDicoTab, ForthMsgTab };

  ForthEditor();
  ~ForthEditor();
  void empty();
  void templated();
  void exec1(const std::string &script);
  void exec();
  std::string elapsedTime();
  void loadDictionary();
  void dumpDictionary();
  //! Call the auto-completion on the current document.
  void autoCompleteWord(const int keyval);

  Gtk::Notebook       m_res_notebooks[2]; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView       m_results;
  Gtk::TextView       m_history;
  Gtk::TextView       m_messages;
  Gtk::TreeView       m_dico;
  Gtk::Statusbar      m_statusbar;
  Gtk::HPaned         m_hpaned;

  //SimForth* m_forth; // FIXME: utile si on gere plusieurs cartes

protected:
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
};

#endif /* FORTHEDITOR_HPP_ */

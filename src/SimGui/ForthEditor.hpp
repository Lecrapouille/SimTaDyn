#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "SimForth.hpp"
#  include <chrono>
#  include "Redirection.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class ForthEditor : public TextEditor
{
public:
  ForthEditor();
  ~ForthEditor();
  void empty();
  void templated();
  void exec1(const std::string &script);
  void exec();
  std::string elapsedTime();
  void loadDictionary();
  void dumpDictionary();

  Gtk::Notebook       m_res_notebooks; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView       m_result;
  Gtk::TextView       m_historic;
  Gtk::TextView       m_debug;
  Gtk::TreeView       m_dico;
  Gtk::Statusbar      m_statusbar;

  //SimForth* m_forth; // FIXME: utile si on gere plusieurs cartes

protected:
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

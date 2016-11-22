#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "Forth.hpp"
#  include <chrono>

// *************************************************************************************************
//
// *************************************************************************************************
class ForthEditor : public TextEditor
{
public:

  ForthEditor();
  ~ForthEditor();
  void newTemplatedDocument();
  void exec1(const std::string &script);
  void exec();
  std::string elapsedTime();

  Gtk::Notebook       m_res_notebooks; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView       m_result;
  Gtk::TextView       m_historic;
  Gtk::TextView       m_debug;
  Gtk::TextView       m_dico;
  Gtk::Statusbar      m_statusbar;
  Gtk::Menu              m_menu;
  Gtk::SeparatorMenuItem m_menuseparator[2];
  Gtk::Image             m_menuimage[8];
  //SimForth* m_forth; // FIXME: utile si on gere plusieurs cartes

protected:
  std::chrono::nanoseconds m_elapsed_time;
};

#endif /* FORTHEDITOR_HPP_ */

#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "Forth.hpp"
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

  Gtk::Notebook       m_res_notebooks; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView       m_result;
  Gtk::TextView       m_historic;
  Gtk::TextView       m_debug;
  Gtk::TextView       m_dico;
  Gtk::Statusbar      m_statusbar;
  Gtk::MenuItem       m_menuitem;
  Gtk::Menu           m_menu;
  Gtk::ImageMenuItem  m_submenu[16];
  Gtk::Image          m_image[16];
  Gtk::SeparatorMenuItem m_menuseparator[2];

  //SimForth* m_forth; // FIXME: utile si on gere plusieurs cartes

protected:
  std::chrono::nanoseconds m_elapsed_time;
  streamgui m_cout; // std::cout redirected inside the GUI
  streamgui m_cerr; // std::cerr redirected inside the GUI
};

#endif /* FORTHEDITOR_HPP_ */

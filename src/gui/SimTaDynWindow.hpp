#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "DrawingArea.hpp"
#  include <utility>
#  include <gtkmm.h> // FIXME

class SimTaDynWindow: public Gtk::Window
{
public:
  SimTaDynWindow(const std::string& title);
  virtual ~SimTaDynWindow() {};

  uint32_t getScreenWidth()
  {
    return m_drawing_area. getScreenWidth();
  }
 uint32_t getScreenHeight()
  {
    return m_drawing_area.getScreenHeight();
  }

protected:
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  // TODO faire ma propre class notebook/text editor ??
  std::pair<Gtk::TextView*, std::string> getCurrentTextView();
  void fileOpen();
  void fileSave();
  void fileSaveAs();
  void fileNew();

  GlDrawingArea m_drawing_area;
  Gtk::HPaned m_hpaned[1];
  Gtk::VBox m_vbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::Menu m_menu[3];
  Gtk::Notebook m_notebook[1];
  Gtk::ScrolledWindow m_scrolledwindow[16];
  Gtk::TextView m_textview[16];
  Gtk::Image m_menuimage[32];
  uint32_t m_nb_opened_documents;
};

#endif /* SIMTADYNWINDOW_HPP_ */

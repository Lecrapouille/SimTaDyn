#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "DrawingArea.hpp"
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

  GlDrawingArea m_drawing_area;
  Gtk::HPaned m_hpaned[1];
  Gtk::VBox m_vbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::Menu m_menufile, m_menuedit;
  Gtk::Notebook m_notebook[1];
  Gtk::ScrolledWindow m_scrolledwindow;
  Gtk::TextView m_textview;
  Gtk::Label m_Label1;
};

#endif /* SIMTADYNWINDOW_HPP_ */

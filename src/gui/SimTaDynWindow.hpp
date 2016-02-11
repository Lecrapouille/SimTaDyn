#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "DrawingArea.hpp"
#  include "ForthEditor.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:
  SimTaDynWindow(const std::string& title);
  virtual ~SimTaDynWindow()
  {
    Gtk::Main::quit();
  }

  uint32_t getScreenWidth()
  {
    return m_drawing_area.getScreenWidth();
  }
 uint32_t getScreenHeight()
  {
    return m_drawing_area.getScreenHeight();
  }

protected:
  enum ToolBarNames { MapToolbar, ForthToolbar };

  bool quitting();
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  inline void addTemplateTab() // FIXME
  {
    m_fortheditor.newTemplatedDocument("1 1 + .");
  }

  GlDrawingArea m_drawing_area;
  Gtk::HPaned m_hpaned[1];
  Gtk::VPaned m_vpaned[1];

  Gtk::VBox m_vbox[2];
  Gtk::HBox m_hbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::Menu m_menu[32];
  Gtk::Image m_menuimage[32];
  ForthEditor m_fortheditor;

  Gtk::SeparatorToolItem m_separator[2];
  Gtk::SeparatorMenuItem separator[2];
  FindWindow *m_findwin;

public:
  Gtk::ToolButton m_toolbutton[32];
  Gtk::Toolbar m_toolbar[2];
};

#endif /* SIMTADYNWINDOW_HPP_ */

#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "DrawingArea.hpp"
#  include "TextEditor.hpp"

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
    return m_drawing_area. getScreenWidth();
  }
 uint32_t getScreenHeight()
  {
    return m_drawing_area.getScreenHeight();
  }

protected:
  bool quitting();
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  inline void saveCurrentTab()
  {
    m_fortheditor.saveCurrentTab();
  }
  inline void saveCurrentTabAs()
  {
    m_fortheditor.saveCurrentTabAs();
  }
  inline void addEmptyTab()
  {
    m_fortheditor.addEmptyTab();
  }
  inline void addFileTab()
  {
    m_fortheditor.addFileTab();
  }
  void execForth();

  GlDrawingArea m_drawing_area;
  Gtk::HPaned m_hpaned[1];
  Gtk::VPaned m_vpaned[1];

  Gtk::VBox m_vbox[2];
  Gtk::HBox m_hbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::Menu m_menu[3];
  Gtk::Image m_menuimage[32];
  ForthEditor m_fortheditor;
  Gtk::Toolbar m_toolbar[2];
  Gtk::Notebook m_notebook[1];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Statusbar m_statusbar[1];
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView m_textview[4];
  Gtk::ToolButton m_toolbutton[4];
};

#endif /* SIMTADYNWINDOW_HPP_ */

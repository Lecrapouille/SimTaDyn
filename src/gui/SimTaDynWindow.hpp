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

  inline void saveCurrentTab() // FIXME
  {
    m_fortheditor.saveCurrentDocument();
  }
  inline void saveCurrentTabAs() // FIXME
  {
    m_fortheditor.saveAsCurrentDocument();
  }
  inline void addEmptyTab() // FIXME
  {
    m_fortheditor.newEmptyDocument();
  }
  inline void addTemplateTab() // FIXME
  {
    m_fortheditor.newTemplatedDocument("1 1 + .");
  }
  inline void addFileTab() // FIXME
  {
    m_fortheditor.newDocument();
  }
  inline void find()
  {
    m_fortheditor.find();
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
  Gtk::SeparatorMenuItem separator[1];
  FindWindow *m_findwin;
};

#endif /* SIMTADYNWINDOW_HPP_ */

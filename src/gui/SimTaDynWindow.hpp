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

  void addForthMenu(const Gtk::BuiltinStockID icon,
                    const std::string &word);
  void addForthButon(const Gtk::BuiltinStockID icon,
                     const std::string &word); // FIXME: Glib::ustring, const Cell16 Forthtoken);

protected:
  enum ToolBarNames { MapToolbar, ForthToolbar };
  enum MenuNames { MapMenu, ForthMenu, PlugginsMenu };

  bool quitting();
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  Gtk::HPaned m_hpaned[1];
  Gtk::VPaned m_vpaned[1];
  Gtk::VBox m_vbox[2];
  Gtk::HBox m_hbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::Menu m_menu[4];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar m_toolbar[2];

  // FIXME: remplacer tableau de button par std::array
  Gtk::ToolButton m_toolbutton[32];
  uint32_t m_nb_forth_buttons;
  uint32_t m_nb_map_buttons;

  GlDrawingArea m_drawing_area;
  ForthEditor m_fortheditor;
  FindWindow *m_findwin;
};

#endif /* SIMTADYNWINDOW_HPP_ */

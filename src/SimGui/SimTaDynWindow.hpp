#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "DrawingArea.hpp"
#  include "ForthEditor.hpp"
#  include "MapEditor.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:
  enum ToolBarNames { MapToolbar, ForthToolbar };
  enum MenuNames { MapMenu, ForthMenu, PlugginsMenu, HelpMenu };

  SimTaDynWindow(const std::string& title);
  virtual ~SimTaDynWindow()
  {
  }

  void onRealize();
  void onUnrealize();
  bool onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */);

  uint32_t addPluggin(const Glib::ustring& icon_name,
                      const std::string &script,
                      const std::string &help);
  Gtk::ToolButton *addForthButon(enum ToolBarNames toolbar,
                                 const Gtk::BuiltinStockID icon,
                                 const std::string &script,
                                 const std::string &help);  // FIXME: Glib::ustring, const Cell16 Forthtoken);
  Gtk::ToolButton *addForthScriptButon(const Gtk::BuiltinStockID icon,
                                       const std::string &script,
                                       const std::string &help);
  Gtk::ToolButton *addMapScriptButon(const Gtk::BuiltinStockID icon,
                                     const std::string &script,
                                     const std::string &help);

protected:
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  ForthEditor m_fortheditor;
  MapEditor m_mapeditor;
  Gtk::HPaned m_hpaned[1];
  Gtk::VPaned m_vpaned[1];
  Gtk::VBox m_vbox[2];
  Gtk::HBox m_hbox[1];
  Gtk::MenuBar m_menubar;
  Gtk::Menu     m_menu[4];
  Gtk::MenuItem m_menuitem[4];
  Gtk::ImageMenuItem  m_plugins_submenu[8];
  Gtk::Image          m_plugins_image[8];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar m_toolbar[2];
  std::vector<Gtk::ToolButton> m_toolbuttons;

  GlDrawingArea m_drawing_area;
  FindWindow *m_findwin;
  ReplaceWindow *m_replacewin;
  GotoLineWindow *m_gotolinewindow;
  uint32_t m_nb_plugins;
};

#endif /* SIMTADYNWINDOW_HPP_ */

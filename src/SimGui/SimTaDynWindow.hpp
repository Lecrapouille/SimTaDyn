#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "Namespaces.hpp"
#  include "DrawingArea.hpp"
#  include "TextEditor.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:

  //! \brief Constructor.
  SimTaDynWindow();

protected:

  //! \brief Initialize the OpenGL context
  inline void onRealize()
  {
    m_drawing_area.onRealize();
  }
  //! \brief Clean up
  inline void onUnrealize()
  {
    m_drawing_area.onUnrealize();
  }
  //! \brief Draw the scene
  inline bool onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */)
  {
    return m_drawing_area.onRender();
  }
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

protected:

  Gtk::HPaned                  m_hpaned[1];
  Gtk::VPaned                  m_vpaned[1];
  Gtk::VBox                    m_vbox[2];
  Gtk::HBox                    m_hbox[1];
  Gtk::MenuBar                 m_menubar;
  Gtk::Menu                    m_menu[simtadyn::MaxMapMenuNames +
                                      simtadyn::MaxForthMenuNames +
                                      simtadyn::MaxGeneralMenuNames + 1];
  Gtk::MenuItem                m_menuitem[simtadyn::MaxMapMenuNames +
                                          simtadyn::MaxForthMenuNames +
                                          simtadyn::MaxGeneralMenuNames + 1];
  std::vector<Gtk::ToolButton> m_toolbuttons; // FIXME resize
  GLDrawingArea                m_drawing_area;
  FindWindow                  *m_findwin = nullptr;
  ReplaceWindow               *m_replacewin = nullptr;
  GotoLineWindow              *m_gotolinewindow = nullptr;
};

#endif /* SIMTADYNWINDOW_HPP_ */

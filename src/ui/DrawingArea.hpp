//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef DRAWINGAREA_HPP_
#  define DRAWINGAREA_HPP_

#  include "Dialogs.hpp"
#  include "Renderer.hpp"
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  pragma GCC diagnostic ignored "-Wredundant-decls"
#    include <gtkmm/glarea.h>
#    include <glibmm.h>
#  pragma GCC diagnostic pop

// *************************************************************************************************
//! \brief Class for OpenGL renderer area
// *************************************************************************************************
class GLDrawingArea
  : public Gtk::GLArea,
    public GLRenderer,
    public PopupException,
    private UniqueID<GLDrawingArea>
{
public:

  enum Direction { Forward, Backward, Up, Down, Right, Left,
                   DirectionIterEnd = Left, DirectionIterBegin = Forward };

  //------------------------------------------------------------------
  //! \brief Constructor. Connect GTK+ io signals
  //------------------------------------------------------------------
  GLDrawingArea();

  //------------------------------------------------------------------
  //! \brief Destructor.
  //------------------------------------------------------------------
  ~GLDrawingArea();

  inline Key id() const { return m_id; }

  //------------------------------------------------------------------
  //! \brief Return the current OpenGL screen width.
  //------------------------------------------------------------------
  inline uint32_t screenWidth() const override
  {
    return static_cast<uint32_t>(Gtk::GLArea::get_width());
  }

  //------------------------------------------------------------------
  //! \brief Return the current OpenGL screen height.
  //------------------------------------------------------------------
  inline uint32_t screenHeight() const override
  {
    return static_cast<uint32_t>(Gtk::GLArea::get_height());
  }

  //------------------------------------------------------------------
  //! \brief Callback when the GTK+ window is creating.
  //! Create OpenGL context. Call the renderer setup.
  //------------------------------------------------------------------
  void onCreate();

  //------------------------------------------------------------------
  //! \brief Callback when the GTK+ window is destroying.
  //! Clean up OpenGL context.
  //------------------------------------------------------------------
  void onRelease();

  //------------------------------------------------------------------
  //! \brief Draw the scene.
  //! \return true if no problem occured
  //------------------------------------------------------------------
  bool onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */);

private:

  //------------------------------------------------------------------
  //! \brief Return the Gtk window containing this widget
  //------------------------------------------------------------------
  virtual Gtk::Window& getRootWindow() override
  {
    return (Gtk::Window&) *get_toplevel();
  }

  //------------------------------------------------------------------
  //! \brief Start OpenGL context.
  //------------------------------------------------------------------
  void createOpenGLContext();

  //------------------------------------------------------------------
  //! \brief Time-triggered callback. Manage pressed keys.
  //------------------------------------------------------------------
  bool onRefreshKeyboard();

  //------------------------------------------------------------------
  //! \brief Mouse scrolling event.
  //------------------------------------------------------------------
  //bool onScrollEvent(GdkEventScroll *event);

  //------------------------------------------------------------------
  //! \brief Desired OpenGL context version: 3.3.
  //! \warning GLArea only supports Core profile.
  //------------------------------------------------------------------
  inline void setCoreVersion() override
  {
    set_required_version(3, 3);
  }

  //------------------------------------------------------------------
  //! \brief Activate OpenGL GL_BLEND
  //------------------------------------------------------------------
  inline void activateTransparency() override
  {
    set_has_alpha(true);
  }

  //------------------------------------------------------------------
  //! \brief Activate OpenGL GL_DEPTH_TEST
  //------------------------------------------------------------------
  inline void activateDepthBuffer() override
  {
    set_has_depth_buffer();
  }

private:

  Key m_id;

  // TODO: memoriser les boutons de la carte:
  // quand la souris passe sur une carte, les boutons sont remis
  // dans le bon etat.
};

#endif /* DRAWINGAREA_HPP_ */

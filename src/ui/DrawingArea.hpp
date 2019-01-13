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
    public GLRenderer
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

  //------------------------------------------------------------------
  //! \brief Asynchronous callback when a keyboard key has been
  //! pressed. We memorize the pressed key. A GTK+ timer will trig
  //! onRefreshKeyboard() which will actions associated to all pressed
  //! keys thanks to .
  //------------------------------------------------------------------
  inline void keyPressed(Direction d)
  {
    m_direction[d] = true;
  }

  //------------------------------------------------------------------
  //! \brief Asynchronous callback when a keyboard key has been released.
  //------------------------------------------------------------------
  inline void keyReleased(Direction d)
  {
    m_direction[d] = false;
  }

private:

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
  bool onScrollEvent(GdkEventScroll *event);

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

  //! \brief Keyboard pressed keys.
  bool m_direction[DirectionIterEnd + 1] = {0};

  //! \brief Keyboard refresh rate
  const unsigned int m_timeout_ms = 10;
};

#endif /* DRAWINGAREA_HPP_ */

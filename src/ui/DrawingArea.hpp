//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef DRAWINGAREA_HPP_
#  define DRAWINGAREA_HPP_

#  include "Renderer.hpp"
#  include <gtkmm/glarea.h>
#  include <glibmm.h>

// *************************************************************************************************
//! \brief Class for OpenGL renderer area
// *************************************************************************************************
class GLDrawingArea
  : public Gtk::GLArea,
    public GLRenderer
{
public:

  enum Direction { Forward, Backward, Up, Down, Right, Left, DirectionIterEnd = Left, DirectionIterBegin = Forward };

  //! \brief Constructor.
  GLDrawingArea(const unsigned int timeout = 10)
    : Gtk::GLArea(),
      GLRenderer()
  {
    // OpenGL context
    setContext();

    //
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK |
               Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

    // Reset keyboard states every 10 ms
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &GLDrawingArea::keyboard),
                                   timeout);

    // Use the mouse scroll event
    signal_scroll_event().connect(sigc::mem_fun(*this, &GLDrawingArea::onScrollEvent));

  }

  virtual ~GLDrawingArea()
  {
  }

  inline uint32_t screenWidth() const override
  {
    return Gtk::GLArea::get_width();
  }

  inline uint32_t screenHeight() const override
  {
    return Gtk::GLArea::get_height();
  }

  //FIXME protected:

  // Desired OpenGL context version 3.3
  inline void setContext() override
  {
    set_required_version(3, 3);
  }

  inline void activateTransparency() override
  {
    set_has_alpha(true);
  }

  inline void activateDepthBuffer() override
  {
    set_has_depth_buffer();
  }

  //! \brief
  inline void keyPressed(Direction d) { m_direction[d] = true; }
  //! \brief
  inline void keyReleased(Direction d) { m_direction[d] = false; }
  //! \brief
  bool on_button_press_event(GdkEventButton* event);
  //! \brief
  bool keyboard();
  //! \brief
  bool onScrollEvent(GdkEventScroll *event);
  //! \brief Draw the scene
  bool onRender();
  //! \brief
  void onRealize();
  //! \brief Clean up
  void onUnrealize();

private:

  bool m_success_init = false;
  bool m_direction[DirectionIterEnd + 1] = {0};
};

#endif /* DRAWINGAREA_HPP_ */

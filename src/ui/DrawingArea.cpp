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

#include "DrawingArea.hpp"
#include "MapEditor.hpp"
#include "DialogException.hpp"
#include <exception>

//------------------------------------------------------------------
GLDrawingArea::GLDrawingArea()
  : Gtk::GLArea(),
    GLRenderer()
{
  // OpenGL core is mandatory
  setCoreVersion();

  // Filter GTK+ events
  add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

  // Reset keyboard states every 10 ms
  Glib::signal_timeout().connect(
      sigc::mem_fun(*this, &GLDrawingArea::onRefreshKeyboard), m_timeout_ms);

  // Use the mouse scroll event
  signal_scroll_event().connect(sigc::mem_fun(*this, &GLDrawingArea::onScrollEvent));
}

//------------------------------------------------------------------
GLDrawingArea::~GLDrawingArea()
{
}

//------------------------------------------------------------------
void GLDrawingArea::createOpenGLContext()
{
  LOGI("Starting OpenGL context");

#if defined(__APPLE__)
  throw OpenGLException("Sorry OpenGL quartz is not implemented by GTK+ team");
#endif

  glewExperimental = true;
  GLenum err = glewInit();

  if (err != GLEW_OK)
    {
      const GLubyte* msg = glewGetErrorString(err);
      const char *m = reinterpret_cast<const char*>(msg);
      LOGX("Throw Gdk::GLError exception");
      throw Gdk::GLError(Gdk::GLError::NOT_AVAILABLE, Glib::ustring(m));
    }

  opengl::hasCreatedContext() = true;
  LOGI("OpenGL context created with success");
}

//------------------------------------------------------------------
void GLDrawingArea::onCreate()
{
  Gtk::GLArea::make_current();
  try
    {
      createOpenGLContext();
      Gtk::GLArea::throw_if_error();

      if (unlikely(false == GLRenderer::setupGraphics()))
        {
          LOGES("During the setup of SimTaDyn graphic renderer");
        }
    }
  catch (const Gdk::GLError& gle)
    {
      PopupException((Gtk::Window&) *get_toplevel(), "An error occured making the OpenGL context during GLArea creation", gle.what(), "");
      opengl::hasCreatedContext() = false;
    }
  catch (const OpenGLException& e)
    {
      PopupException((Gtk::Window&) *get_toplevel(), e, "An OpenGL error occurred during the setupGraphics()");
      opengl::hasCreatedContext() = false;
    }
}

//------------------------------------------------------------------
void GLDrawingArea::onRelease()
{
  if (unlikely(false == opengl::hasCreatedContext()))
    return ;

  Gtk::GLArea::make_current();
  try
    {
      Gtk::GLArea::throw_if_error();
    }
  catch (const Gdk::GLError& gle)
    {
      LOGES("An error occured making the context current during onRelease(): %s - %u: %s",
            gle.domain(), static_cast<unsigned>(gle.code()), gle.what().c_str());
    }
}

//------------------------------------------------------------------
bool GLDrawingArea::onRender()
{
  if (unlikely(false == opengl::hasCreatedContext()))
    {
      static bool singleton = true;
      if (singleton)
        {
          LOGD("No OpenGL rendering will be made");
          singleton = false;
        }
      return false;
    }

  try
    {
      Gtk::GLArea::throw_if_error();

      GLRenderer::clearScreen();
      GLRenderer::draw();
    }
  catch (const Gdk::GLError& gle)
    {
      LOGES("An error occured making the context current during onRender(): %s - %u: %s",
            gle.domain(), static_cast<unsigned>(gle.code()), gle.what().c_str());
      return false;
    }
  catch (const OpenGLException& e)
    {
      static bool singleton = true;
      if (singleton)
        {
          PopupException((Gtk::Window&) *get_toplevel(), e, "");
          singleton = false;
        }
      else
        {
          LOGES("An OpenGL exception occurred during the rendering %s - %s",
                e.what(), e.message().c_str());
        }
      return false;
    }

  return true;
}

//------------------------------------------------------------------
bool GLDrawingArea::on_button_press_event(GdkEventButton* event)
{
  if (event->type == GDK_BUTTON_PRESS)
    {
      switch (event->button)
        {
        case 1:
          std::cout << "GLDrawingArea::on_button_press_event button1" << std::endl;
          MapEditor::instance().button1PressEvent(event->x, event->y);
          break;
        case 2:
          std::cout << "GLDrawingArea::on_button_press_event button2" << std::endl;
          MapEditor::instance().button2PressEvent(event->x, event->y);
          break;
        case 3:
          std::cout << "GLDrawingArea::on_button_press_event button3" << std::endl;
          MapEditor::instance().button3PressEvent(event->x, event->y);
          break;
        }
    }

  return true;
}

//------------------------------------------------------------------
bool GLDrawingArea::onRefreshKeyboard()
{
  /*Camera2D& camera = GLRenderer::camera2D();

  if (m_direction[GLDrawingArea::Forward])
    {
      camera.zoomOffset(0.01f);
    }
  if (m_direction[GLDrawingArea::Backward])
    {
      camera.zoomOffset(-0.01f);
    }
  if (m_direction[GLDrawingArea::Up])
    {
      camera.moveOffset(0.0f, -10.0f);
    }
  if (m_direction[GLDrawingArea::Down])
    {
      camera.moveOffset(0.0f, 10.0f);
    }
  if (m_direction[GLDrawingArea::Right])
    {
      camera.moveOffset(10.0f, 0.0f);
    }
  if (m_direction[GLDrawingArea::Left])
    {
      camera.moveOffset(-10.0f, 0.0f);
    }

  // std::cout << camera << std::endl;
  GLRenderer::applyViewport(camera);*/
  return true;
}

//------------------------------------------------------------------
bool GLDrawingArea::onScrollEvent(GdkEventScroll *event)
{
  //GLRenderer::camera2D().zoomAt(event->x, event->y, event->delta_y);
  //GLRenderer::applyViewport();
  return true;
}

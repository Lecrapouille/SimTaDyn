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
#include "DialogException.hpp"
#include <exception>

#if 0
//------------------------------------------------------------------
void GLDrawingArea::onEnterViewEvent(GdkEventCrossing* /*crossing_event*/, GLDrawingArea& view)
{
  view.grab_focus();
  view.backgroundColor(Color(0.0f, 0.0f, 0.4f, 1.0f));
  m_drawing_area = &view;
  std::cout << "ME:Entering "
            << (int) view.id() << " "
            << &view << std::endl;
  // TODO restaurer les boutons associes a la vue
}

//------------------------------------------------------------------
void GLDrawingArea::onLeaveViewEvent(GdkEventCrossing* /*crossing_event*/, GLDrawingArea& view)
{
  view.backgroundColor(Color(0.0f, 0.0f, 0.2f, 1.0f));
}
#endif

//------------------------------------------------------------------
GLDrawingArea::GLDrawingArea()
  : Gtk::GLArea(),
    GLRenderer(),
    m_id(getID())
{
  std::cout << "New GLDrawingArea " << (int) getID() << std::endl;

  // Widget aspect
  set_hexpand(true);
  set_vexpand(true);
  set_auto_render(true);

  // Detect when the mouse cursor enters the widget
  set_can_focus(true);

  // OpenGL core is mandatory
  setCoreVersion();

  // Filter GTK+ events
  add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK |
             Gdk::POINTER_MOTION_MASK | Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);

  // Use the mouse scroll event
  //signal_scroll_event().connect(sigc::mem_fun(*this, &GLDrawingArea::onScrollEvent));
  // Connect drawing area signals
  signal_realize().connect(sigc::mem_fun(*this, &GLDrawingArea::onCreate));
  signal_unrealize().connect(sigc::mem_fun(*this, &GLDrawingArea::onRelease), false);
  signal_render().connect(sigc::mem_fun(*this, &GLDrawingArea::onRender));
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
      popupException("An error occured making the OpenGL context during GLArea creation", gle.what(), "");
      opengl::hasCreatedContext() = false;
    }
  catch (const OpenGLException& e)
    {
      popupException(e, "An OpenGL error occurred during the setupGraphics()");
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
bool GLDrawingArea::onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */)
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
          popupException(e, "");
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
#if 0
bool GLDrawingArea::onScrollEvent(GdkEventScroll *event)
{
  //GLRenderer::camera2D().zoomAt(event->x, event->y, event->delta_y);
  //GLRenderer::applyViewport();
  return true;
}
#endif

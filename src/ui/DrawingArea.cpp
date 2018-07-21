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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "DrawingArea.hpp"
#include "MapEditor.hpp"
#include <exception>

// *************************************************************************************************
//
// *************************************************************************************************
bool GLDrawingArea::on_button_press_event(GdkEventButton* event)
{
  if (event->type == GDK_BUTTON_PRESS)
    {
      switch (event->button)
        {
        case 1:
          MapEditor::instance().button1PressEvent(event->x, event->y);
          break;
        case 2:
          MapEditor::instance().button2PressEvent(event->x, event->y);
          break;
        case 3:
          MapEditor::instance().button3PressEvent(event->x, event->y);
          break;
        }
    }

  return true;
}

// FIXME: plutot MapEditor::keyboard() ?
bool GLDrawingArea::keyboard()
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

bool GLDrawingArea::onScrollEvent(GdkEventScroll *event)
{
  //GLRenderer::camera2D().zoomAt(event->x, event->y, event->delta_y);
  //GLRenderer::applyViewport();
  return true;
}

void GLDrawingArea::onRealize()
{
#if ARCHI == OSX
  // FIXME: sorry OpenGL quartz is not implemented by GTK team
  m_success_init = false;
  return ;
#endif

  Gtk::GLArea::make_current();
  SimTaDyn::glStartContext();
  Gtk::GLArea::throw_if_error();
  try
    {
      m_success_init = GLRenderer::setupGraphics();
    }
  catch (const OpenGLException& e)
    {
      std::cerr << "An error occurred in the realize callback of the GLArea" << std::endl;
      std::cerr << e.what() << " - " << e.message() << std::endl;
      m_success_init = false;
    }
  if (false == m_success_init)
    {
      LOGES("During the setup of SimTaDyn graphic renderer");
    }
}

void GLDrawingArea::onUnrealize()
{
  if (false == m_success_init)
    return ;

  Gtk::GLArea::make_current();
  try
    {
      Gtk::GLArea::throw_if_error();
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "[FAILED] An error occured making the context current during unrealize" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
  catch (const OpenGLException& e)
    {
      std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
      std::cerr << e.what() << " - " << e.message() << std::endl;
    }
}

bool GLDrawingArea::onRender()
{
  if (false == m_success_init)
    return false;

  try
    {
      Gtk::GLArea::throw_if_error();

      if (SimTaDyn::glIsFunctional() && m_success_init /*&& GLRenderer::needRefresh()*/)
        {
          GLRenderer::draw();
        }
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
      return false;
    }
  catch (const OpenGLException& e)
    {
      std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
      std::cerr << e.what() << " - " << e.message() << std::endl;
      return false;
    }

  return true;
}

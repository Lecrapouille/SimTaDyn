#include "DrawingArea.hpp"
#include <exception>

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
  Gtk::GLArea::make_current();
  SimTaDyn::glStartContext();
  Gtk::GLArea::throw_if_error();
  m_success_init = GLRenderer::setupGraphics();
  if (!m_success_init)
    {
      LOGES("During the setup of SimTaDyn graphic renderer");
    }
}

void GLDrawingArea::onUnrealize()
{
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
}

bool GLDrawingArea::onRender()
{
  try
    {
      Gtk::GLArea::throw_if_error();

      if (SimTaDyn::glIsFunctional() && m_success_init /*&& GLRenderer::needRefresh()*/)
        {
          GLRenderer::draw();
        }

      return true;
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
      return false;
    }
}

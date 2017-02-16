#include "DrawingArea.hpp"

bool GLDrawingArea::keyboard()
{
    Camera2D& camera = GLRenderer::camera2D();

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
    GLRenderer::applyViewport(camera);
    return true;
}

bool GLDrawingArea::onScrollEvent(GdkEventScroll *event)
{
    GLRenderer::camera2D().zoomAt(event->x, event->y, event->delta_y);
    GLRenderer::applyViewport();
    return true;
}

void GLDrawingArea::onRealize()
{
  Gtk::GLArea::make_current();
  try
    {
      // GLArea only support Core profile.
      glewExperimental = true;
      if (GLEW_OK != glewInit())
        {
          std::cerr << "[FAILED] when initializing GLEW (Maybe the OpenGL context does not exist)." << std::endl;
          return ;
        }

      Gtk::GLArea::throw_if_error();
      GLRenderer::init();
      GLRenderer::clearScreen();
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "[FAILED] An error occured making the context current during realize:" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

void GLDrawingArea::onUnrealize()
{
  Gtk::GLArea::make_current();
  try
    {
      Gtk::GLArea::throw_if_error();
      //GLRenderer::release();
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

      GLRenderer::begin();
      GLRenderer::draw();
      GLRenderer::end();

      return true;
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
      return false;
    }
}

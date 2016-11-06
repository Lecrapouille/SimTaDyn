#include "DrawingArea.hpp"
#include "SimTaDynContext.hpp"

// *************************************************************************************************
// Class for OpenGl renderer area
// *************************************************************************************************
GlDrawingArea::GlDrawingArea(): Gtk::DrawingArea()
{
  std::cout << "GlDrawingArea::GlDrawingArea()\n";
  // No movement
  for (size_t i = 0; i < no_; ++i) direction_[i] = false;

  // OpenGL Configuration
  Glib::RefPtr<Gdk::GL::Config> configuration;

  // Try double-buffered visual
  configuration = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA   |
                                          Gdk::GL::MODE_DEPTH  |
                                          Gdk::GL::MODE_DOUBLE);
  if (!configuration)
    {
      std::cout << "*** Cannot find the double-buffered visual." << std::endl;
      std::cout << "*** Trying single-buffered visual." << std::endl;

      // Try single-buffered visual
      configuration = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA  |
                                              Gdk::GL::MODE_DEPTH);

      if (!configuration)
        {
          std::cerr << "*** No appropriate OpenGL-capable visual found." << std::endl;
          throw ErrorGlDrawingArea();
        }
    }

  set_gl_capability(configuration);
}

// *************************************************************************************************
// Initialize the OpenGL context
// *************************************************************************************************
void GlDrawingArea::on_realize()
{
  std::cout << "GlDrawingArea::on_realize()\n";
  Gtk::Widget::on_realize();
  if (!create_gl_context()) throw ErrorGlDrawingArea();

  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

  windowGL->gl_begin(get_gl_context());

  // FIXME: on_realize() appelle je ne sais pas pourquoi on_configure_event puis Renderer::initialize
  // ca fait beaucoup d'appel pour rien
  Renderer::initialize();
  Renderer::clearScreen();

  windowGL->gl_end();

  // Draw regularly
  Glib::signal_idle().connect(sigc::mem_fun(*this, &GlDrawingArea::onIdle));
  // Reset keyboard states every 10 ms
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &GlDrawingArea::onTimeout), 10);
  // Use the mouse scroll for zooming on the opengl window
  signal_scroll_event().connect(sigc::mem_fun(*this, &GlDrawingArea::on_scroll_event));
}

// *************************************************************************************************
// Redraw on the GlDrawingArea request
// *************************************************************************************************
bool GlDrawingArea::on_expose_event(GdkEventExpose* event)
{
  std::cout << "GlDrawingArea::on_expose_event()\n";
  const SimTaDynContext& simtadyn = SIMTADYN();
  Renderer::zoomFitPage(simtadyn.m_graph);
  Renderer::applyViewport();

  onIdle();
  return Gtk::Widget::on_expose_event(event);
}

// *************************************************************************************************
// When the windo< cahnged of size
// *************************************************************************************************
bool GlDrawingArea::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  windowGL->gl_begin(get_gl_context());

  const SimTaDynContext& simtadyn = SIMTADYN();
  Renderer::zoomFitPage(simtadyn.m_graph);
  Renderer::applyViewport();

  windowGL->gl_end();
  return Gtk::Widget::on_configure_event(event);
}

// *************************************************************************************************
// Draw the scene
// *************************************************************************************************
bool GlDrawingArea::onIdle()
{
  SimTaDynContext& simtadyn = SIMTADYN();
  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  windowGL->gl_begin(get_gl_context());

  Renderer::clearScreen();
  glCheck(glLoadIdentity());
  Renderer::draw(simtadyn.m_graph);

  windowGL->gl_end();
  windowGL->swap_buffers();
  return true;
}

// *************************************************************************************************
// Keybord reset
// *************************************************************************************************
bool GlDrawingArea::onTimeout()
{
  Camera2D& camera = Renderer::getCamera2D();

  if (direction_[Forward])
    {
      camera.zoomOffset(0.01f);
    }
  if (direction_[Backward])
    {
      camera.zoomOffset(-0.01f);
    }
  if (direction_[Up])
    {
      camera.moveOffset(0.0f, -10.0f);
    }
  if (direction_[Down])
    {
      camera.moveOffset(0.0f, 10.0f);
    }
  if (direction_[Right])
    {
      camera.moveOffset(10.0f, 0.0f);
    }
  if (direction_[Left])
    {
      camera.moveOffset(-10.0f, 0.0f);
    }

  Renderer::applyViewport(camera);
  return true;
}

// Mouse wheel event detected : update scale
bool GlDrawingArea::on_scroll_event(GdkEventScroll *event)
{
  float32_t delta_scroll;

#if GTKMM_MAJOR_VERSION >= 3
  delta_scroll = event->delta_y;
#else // gtkmm-2.4
  if (GDK_SCROLL_UP == event->direction)
    delta_scroll = -0.1;
  else if (GDK_SCROLL_DOWN == event->direction)
    delta_scroll = 0.1;
  else
    return false;
#endif


  Renderer::getCamera2D().zoomAt(event->x, event->y, delta_scroll);
  Renderer::applyViewport();
  return true;
}

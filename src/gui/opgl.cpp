#include "opgl.hpp"

// *************************************************************************************************
// Class for OpenGl renderer area
// *************************************************************************************************
GlDrawingArea::GlDrawingArea(): Gtk::DrawingArea()
{
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
  Gtk::Widget::on_realize();
  if (!create_gl_context()) throw ErrorGlDrawingArea();

  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  add_events(Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

  windowGL->gl_begin(get_gl_context());

  // FIXME: on_realize() appelle je ne sais pas pourquoi on_configure_event puis Renderer::initialize
  // ca fait beaucoup d'appel pour rien
  Renderer::initialize();
  Renderer::clearScreen(Color::Black);
  glClearDepth(1.0f);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(10);
  glEnable(GL_ALPHA_TEST);

  windowGL->gl_end();

  loader.loadShapefile("../data/3dpoints.shp", graph);
  std::cout << graph.name << std::endl;

  /*  loader.loadShapefile("../data/3dpoints.shp", graph);
  std::cout << std::endl << "Il y a " << SimTaDynNode::howMany() << " nodes" << std::endl;
  graph.addArc(1, 20, "AAA", 0, 42);
  graph.addArc(1, 10, "AAA", 0, 42);
  graph.addArc(10, 11, "AAA", 0, 42);
  graph.addArc(10, 4, "AAA", 0, 42);
  std::cout << std::endl << "Il y a " << SimTaDynArc::howMany() << " arcs: " << graph.getArc(SimTaDynCell::howMany())->name << std::endl;
  */
  //Position3D& p = graph.getNode(1)->getPosition();
  //getCamera2D().zoomfitImage(getScreenWidth(), getScreenHeight(),
  //                           graph.bbox.bbmax.x - graph.bbox.bbmin.x,
  //                          graph.bbox.bbmax.y - graph.bbox.bbmin.y);
                             //moveAt(p.x, p.y);

  // Draw regurlary
  Glib::signal_idle().connect(sigc::mem_fun(*this, &GlDrawingArea::onIdle));
  // Reset keyboard states every 10 ms
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &GlDrawingArea::onTimeout), 10);

  signal_scroll_event().connect(sigc::mem_fun(*this, &GlDrawingArea::on_scroll_event));
}

// *************************************************************************************************
// Redraw on the GlDrawingArea request
// *************************************************************************************************
bool GlDrawingArea::on_expose_event(GdkEventExpose* event)
{
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

  Camera2D& camera = getCamera2D();
  camera.setCameraSize(getScreenWidth(), getScreenHeight());
  applyViewport(camera);

  windowGL->gl_end();
  return Gtk::Widget::on_configure_event(event);
}

// *************************************************************************************************
// Draw the scene
// *************************************************************************************************
bool GlDrawingArea::onIdle()
{
  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  windowGL->gl_begin(get_gl_context());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  graph.draw();

  windowGL->gl_end();
  windowGL->swap_buffers();
  return true;
}

// *************************************************************************************************
// Keybord reset
// *************************************************************************************************
bool GlDrawingArea::onTimeout()
{
  Camera2D& camera = getCamera2D();

  if (direction_[Forward])
    {
      camera.setZoom(camera.getZoom() * (1.0 + 0.01f));
    }
  if (direction_[Backward])
    {
      camera.setZoom(camera.getZoom() * (1.0 - 0.01f));
    }
  if (direction_[Up])
    {
      camera.moveOffset(0.0f, 1.0f);
    }
  if (direction_[Down])
    {
      camera.moveOffset(0.0f, -1.0f);
    }
  if (direction_[Right])
    {
      camera.moveOffset(-1.0f, 0.0f);
    }
  if (direction_[Left])
    {
      camera.moveOffset(1.0f, 0.0f);
    }

  applyViewport(camera);
  return true;
}

// Mouse wheel event detected : update scale
bool GlDrawingArea::on_scroll_event(GdkEventScroll *event)
{
  float32_t delta_scroll;

#if 1 // gtkmm-2.4
  if (GDK_SCROLL_UP == event->direction)
    delta_scroll = -0.1;
  else if (GDK_SCROLL_DOWN == event->direction)
    delta_scroll = 0.1;
  else
    return false;
#else // gtkmm-3.0
  delta_scroll = event->delta_y;
#endif

  getCamera2D().zoomScrollAt(event->x, event->y, delta_scroll,
                             getScreenWidth(), getScreenHeight());
  applyViewport();
  return true;
}

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
  glLineWidth(1.5f);
  glEnable(GL_ALPHA_TEST);

  windowGL->gl_end();

  getCamera2D().moveAt(300, 300);

  // Draw regurlary
  Glib::signal_idle().connect(sigc::mem_fun(*this, &GlDrawingArea::onIdle));
  // Reset keyboard states every 10 ms
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &GlDrawingArea::onTimeout), 10);
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
  camera.setSize(getScreenWidth(), getScreenHeight());
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

  glPushMatrix();
  glTranslated(300.0f, 300.0f, 0.0f);
  glRecti(5.0f, -5.0f, -5.0f, 5.0f);
  glPopMatrix();

  glPushMatrix();
  glTranslated(10.0f, 10.0f, 0.0f);
  glRecti(5.0f, -5.0f, -5.0f, 5.0f);
  glPopMatrix();

  glPushMatrix();
  glTranslated(1.0f, 1.0f, 0.0f);
  glRecti(5.0f, -5.0f, -5.0f, 5.0f);
  glPopMatrix();

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
      camera.zoom(0.01f); // Bug suite geometrique au lieu de zoomer sut taille d'oritgine
    }
  if (direction_[Backward])
    {
      camera.zoom(-0.01f);
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

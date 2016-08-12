#include "opgl.hpp"
//#include "draw.hpp"

/**************************************************************************
 *
 **************************************************************************/
static void opgl_2d_mode(GLsizei width, GLsizei height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, width, 0.0f, height, -1.0f, DISTANCE_CLIPPING);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// *************************************************************************************************
// Class for OpenGl renderer area
// *************************************************************************************************
GlDrawingArea::GlDrawingArea(): Gtk::DrawingArea()
{
  // No movement
  for (size_t i = 0; i < no_; ++i) direction_[i] = false;
  x = y = z = 0.0f;

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

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(1.5f);
  glEnable(GL_ALPHA_TEST);

  windowGL->gl_end();

  // Draw regurlary
  Glib::signal_idle().connect(sigc::mem_fun(*this, &GlDrawingArea::on_idle));
  // Reset keyboard states every 10 ms
  Glib::signal_timeout().connect(sigc::mem_fun(*this, &GlDrawingArea::on_timeout), 10);
}

// *************************************************************************************************
// Redraw on the GlDrawingArea request
// *************************************************************************************************
bool GlDrawingArea::on_expose_event(GdkEventExpose* event)
{
  on_idle();
  return Gtk::Widget::on_expose_event(event);
}

// *************************************************************************************************
// When the windo< cahnged of size
// *************************************************************************************************
bool GlDrawingArea::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  const GLsizei width = static_cast<GLsizei>(get_width());
  const GLsizei height = static_cast<GLsizei>(get_height());

  windowGL->gl_begin(get_gl_context());
  glViewport(0.0f, 0.0f, width, height);
  opgl_2d_mode(width, height);
  windowGL->gl_end();
  return Gtk::Widget::on_configure_event(event);
}

// *************************************************************************************************
// Draw the scene
// *************************************************************************************************
bool GlDrawingArea::on_idle()
{
  Glib::RefPtr<Gdk::GL::Window> windowGL = get_gl_window();
  if (!windowGL) throw ErrorGlDrawingArea();

  windowGL->gl_begin(get_gl_context());

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(x, y, z);
  //drawMap();

  glPushMatrix();
  glTranslated(300.0f, 300.0f, 0.0f);
  glRecti(THICKNESS, -THICKNESS, -THICKNESS, THICKNESS);
  glPopMatrix();

  windowGL->gl_end();
  windowGL->swap_buffers();
  return true;
}

// *************************************************************************************************
// Keybord reset
// *************************************************************************************************
bool GlDrawingArea::on_timeout()
{
  if (direction_[Forward]) z = 0.0f;
  if (direction_[Backward]) z = 0.0f;
  if (direction_[Up]) y += 1.0f;
  if (direction_[Down]) y -= 1.0f;
  if (direction_[Right]) x += 1.0f;
  if (direction_[Left]) x -= 1.0f;
  return true;
}

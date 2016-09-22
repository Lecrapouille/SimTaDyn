#ifndef DRAWINGAREA_HPP_
#  define DRAWINGAREA_HPP_

#  include <gtkmm/main.h>
#  include <gtkmm/widget.h>
#  include <gtkmm/window.h>
#  include <gtkmm/drawingarea.h>
#  include <gdk/gdkkeysyms.h>
#  include <gtkglmm.h>
#  include <glibmm/error.h>
#  include "Renderer.hpp"

// *************************************************************************************************
// Exception
// *************************************************************************************************
class ErrorGlDrawingArea: public std::exception
{
public:
  ErrorGlDrawingArea() throw () {}
  ~ErrorGlDrawingArea() throw () {}
  virtual const char* what () const throw ()
  {
    return "Error from OpenGL during the config or context";
  }
};

// *************************************************************************************************
// Class for OpenGl renderer area
// *************************************************************************************************
class GlDrawingArea: public Gtk::DrawingArea,
                     public Gtk::GL::Widget<GlDrawingArea>,
                     public Renderer
{
public:
  GlDrawingArea();
  virtual ~GlDrawingArea() {};

  // Movement in the world
  enum Direction { Forward, Backward, Up, Down, Right, Left, no_ };
  void keyPressed(Direction d) { direction_[d] = true; }
  void keyReleased(Direction d) { direction_[d] = false; }

  uint32_t getScreenWidth() const { return get_width(); }
  uint32_t getScreenHeight() const { return get_height(); }
  bool on_scroll_event(GdkEventScroll *event);

protected:
  virtual void on_realize();
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_configure_event(GdkEventConfigure* event);
  bool onIdle();
  bool onTimeout();
  void mode2D(GLfloat width, GLfloat height);

private:
  bool direction_[no_];
};

#endif /* DRAWINGAREA_HPP_ */

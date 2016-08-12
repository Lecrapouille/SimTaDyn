#ifndef OPGL_HPP_
#  define OPGL_HPP_

#  include <gtkmm/window.h>
#  include <gtkmm/drawingarea.h>
#  include <gdk/gdkkeysyms.h>
#  include <gtkglmm.h>
#  include <glibmm/error.h>
#  include <string>
#  include <cstddef>
#  include <exception>
#  include <iostream>

#  ifdef __APPLE__
#    include <GLUT/glut.h>
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#  else
#    include <GL/glut.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#  endif

#  define DISTANCE_CLIPPING 100000.0f
#  define THICKNESS 5.0f

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
class GlDrawingArea: public Gtk::DrawingArea, public Gtk::GL::Widget<GlDrawingArea>
{
public:
  GlDrawingArea();
  virtual ~GlDrawingArea() {};
  GLfloat x, y, z;

  // Movement in the world
  enum Direction { Forward, Backward, Up, Down, Right, Left, no_};
  void keyPressed(Direction d) { direction_[d] = true; }
  void keyReleased(Direction d) { direction_[d] = false; }

protected:
  virtual void on_realize();
  virtual bool on_expose_event(GdkEventExpose* event);
  virtual bool on_configure_event(GdkEventConfigure* event);
  bool on_idle();
  bool on_timeout();

private:
  bool direction_[no_];
};

#endif /* OPGL_HPP_ */

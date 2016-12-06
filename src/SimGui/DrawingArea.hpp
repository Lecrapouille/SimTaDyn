#ifndef DRAWINGAREA_HPP_
#  define DRAWINGAREA_HPP_

#  include "Renderer.hpp"

// *************************************************************************************************
// Class for OpenGl renderer area
// *************************************************************************************************
class GlDrawingArea: public Gtk::GLArea, public Renderer
{
public:
  GlDrawingArea()
  {
  }

  virtual ~GlDrawingArea()
  {
  }

  inline uint32_t screenWidth() const //override
  {
    return get_width();
  }

  inline uint32_t screenHeight() const //override
  {
    return get_height();
  }
};

#endif /* DRAWINGAREA_HPP_ */

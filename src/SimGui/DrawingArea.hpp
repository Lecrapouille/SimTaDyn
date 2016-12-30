#ifndef DRAWINGAREA_HPP_
#  define DRAWINGAREA_HPP_

#  include "Renderer.hpp"

// *************************************************************************************************
// Class for OpenGl renderer area
// *************************************************************************************************
class GlDrawingArea
  : public Gtk::GLArea,
    public Renderer
{
public:
  enum Direction { Forward, Backward, Up, Down, Right, Left, DirectionIterEnd = Left, DirectionIterBegin = Forward };

  GlDrawingArea(const unsigned int timeout = 10)
    : Gtk::GLArea(),
      Renderer()
  {
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK |
               Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

    // Reset keyboard states every 10 ms
    Glib::signal_timeout().connect(
                                   sigc::mem_fun(*this, &GlDrawingArea::keyboard),
                                   timeout);

    // Use the mouse scroll event
    signal_scroll_event().connect(sigc::mem_fun(*this, &GlDrawingArea::onScrollEvent));

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

  inline void keyPressed(Direction d) { m_direction[d] = true; }
  inline void keyReleased(Direction d) { m_direction[d] = false; }

  bool keyboard();
  bool onScrollEvent(GdkEventScroll *event);

private:
  bool m_direction[DirectionIterEnd + 1] = {0};
};

#endif /* DRAWINGAREA_HPP_ */

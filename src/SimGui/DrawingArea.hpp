#ifndef DRAWINGAREA_HPP_
#  define DRAWINGAREA_HPP_

#  include "Renderer.hpp"
#  include <gtkmm.h>

// *************************************************************************************************
//! \brief Class for OpenGL renderer area
// *************************************************************************************************
class GLDrawingArea
  : public Gtk::GLArea,
    public GLRenderer
{
public:
  enum Direction { Forward, Backward, Up, Down, Right, Left, DirectionIterEnd = Left, DirectionIterBegin = Forward };

  //! \brief Constructor.
  GLDrawingArea(const unsigned int timeout = 10)
    : Gtk::GLArea(),
      GLRenderer()
  {
    // OpenGL context
    setContext();

    //
    add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK | Gdk::BUTTON_PRESS_MASK |
               Gdk::BUTTON_RELEASE_MASK | Gdk::SCROLL_MASK | Gdk::POINTER_MOTION_MASK);

    // Reset keyboard states every 10 ms
    Glib::signal_timeout().connect(sigc::mem_fun(*this, &GLDrawingArea::keyboard),
                                   timeout);

    // Use the mouse scroll event
    signal_scroll_event().connect(sigc::mem_fun(*this, &GLDrawingArea::onScrollEvent));

  }

  virtual ~GLDrawingArea()
  {
  }

  // Desired OpenGL context version 3.3
  inline void setContext() override
  {
    set_required_version(3, 3);
  }

  inline uint32_t screenWidth() const override
  {
    return Gtk::GLArea::get_width();
  }

  inline uint32_t screenHeight() const override
  {
    return Gtk::GLArea::get_height();
  }

  //! \brief
  inline void keyPressed(Direction d) { m_direction[d] = true; }
  //! \brief
  inline void keyReleased(Direction d) { m_direction[d] = false; }
  //! \brief
  bool keyboard();
  //! \brief
  bool onScrollEvent(GdkEventScroll *event);
  //! \brief Draw the scene
  bool onRender();
  //! \brief
  void onRealize();
  //! \brief Clean up
  void onUnrealize();

private:
  bool m_direction[DirectionIterEnd + 1] = {0};
};

#endif /* DRAWINGAREA_HPP_ */

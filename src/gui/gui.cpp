#include "gui.hpp"

// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(const std::string& title): Gtk::Window()
{
  set_title(title);
  set_default_size(800, 600);
  set_position(Gtk::WIN_POS_CENTER);
  add(drawing_area_);

  add_events(Gdk::KEY_RELEASE_MASK);
  signal_key_press_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyPressed));
  signal_key_release_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyReleased));

  //signal_scroll_event().connect(sigc::mem_fun(*this, &SimTaDynWindow::on_scroll_event));

  show_all_children();
}

#if 0
// Mouse button pressed : process mouse button event
bool SimTaDynWindow::on_button_press_event(GdkEventButton *event)
{
  // Check if the event is a left button click.
  if (1 == event->button)
    {
      // Memorize pointer position
      lastXMouse = event->x;
      lastYMouse = event->y;
      // Start moving the view
      moveFlag = true;
      // Event has been handled
      return true;
    }

#if 0
  // Check if the event is a right button click.
  if (3 == event->button)
    {
      // Memorize mouse coordinates
      lastXMouse = event->x;
      lastYMouse = event->y;
      // Display the popup menu
      m_Menu_Popup.popup(event->button, event->time);
      // The event has been handled.
      return true;
    }
#endif

  // Event has not been handled
  return false;
}

// Mouse button released : process mouse button event
bool GlDrawingArea::on_button_release_event(GdkEventButton *event)
{
  // Check if it is the left button
  if ((1 == event->button) && moveFlag)
    {
      // End of motion
      moveFlag = false;
      // Update image focus
      imgFocusX -= (event->x - lastXMouse) / scale;
      imgFocusY -= (event->y - lastYMouse) / scale;
      return true;
    }
  // Event has been handled
  return false;
}

// Mouse pointer moved : process event
bool GlDrawingArea::on_motion_notify_event(GdkEventMotion*event)
{
  // If the left button is pressed, move the view
  if (moveFlag)
    {
      // Get mouse coordinates
      int XMouse = event->x;
      int YMouse = event->y;

      // Update image focus
      imgFocusX -= (XMouse - lastXMouse) / scale;
      imgFocusY -= (YMouse - lastYMouse) / scale;

      // Memorize new position of the pointer
      lastXMouse = XMouse;
      lastYMouse = YMouse;

      // Update view
      camera.moveAt(imgFocusX, imgFocusY);
      applyViewport();
      return true;
    }
  // Event has been handled
  return false;
}
#endif

void SimTaDynWindow::onKeyPressed(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Page_Up:
      drawing_area_.keyPressed(GlDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      drawing_area_.keyPressed(GlDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      drawing_area_.keyPressed(GlDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      drawing_area_.keyPressed(GlDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      drawing_area_.keyPressed(GlDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      drawing_area_.keyPressed(GlDrawingArea::Left);
      break;
    default:
      break;
    }
}

void SimTaDynWindow::onKeyReleased(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Page_Up:
      drawing_area_.keyReleased(GlDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      drawing_area_.keyReleased(GlDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      drawing_area_.keyReleased(GlDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      drawing_area_.keyReleased(GlDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      drawing_area_.keyReleased(GlDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      drawing_area_.keyReleased(GlDrawingArea::Left);
      break;
    default:
      break;
    }
}

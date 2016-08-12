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

  show_all_children();
}

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

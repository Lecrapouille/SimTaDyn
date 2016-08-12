#ifndef GUI_HPP_
#  define GUI_HPP_

#  include "opgl.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:
  SimTaDynWindow(const std::string& title);
  virtual ~SimTaDynWindow() {};

protected:
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  GlDrawingArea drawing_area_;
};

#endif /* GUI_HPP_ */

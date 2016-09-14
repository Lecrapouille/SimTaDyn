#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "DrawingArea.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:
  SimTaDynWindow(const std::string& title);
  virtual ~SimTaDynWindow() {};

  uint32_t getScreenWidth()
  {
    return drawing_area_. getScreenWidth();
  }
 uint32_t getScreenHeight()
  {
    return drawing_area_.getScreenHeight();
  }
protected:
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

  GlDrawingArea drawing_area_;
};

#endif /* SIMTADYNWINDOW_HPP_ */

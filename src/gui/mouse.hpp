#ifndef MOUSE_HPP_
#  define MOUSE_HPP_

// From http://www.lucidarme.me/?p=4898 (example 23)
class Mouse
{
public:
  Mouse() { scale = 1.0; };

  // Scale of the image
  double                      scale;

  // Coordinates of the image point to place at the center of the window (focussed pixel)
  double                      imgFocusX,imgFocusY;

  // Used to memorize last mouse coordinates
  int                         lastXMouse, lastYMouse;

  // Flags
  bool                        resetFlag;
  bool                        moveFlag;
  bool                        targetFlag;

  void fitImage(int winWidth,int winHeight, int imgWidth, int imgHeight);
};

#endif /* MOUSE_HPP_ */

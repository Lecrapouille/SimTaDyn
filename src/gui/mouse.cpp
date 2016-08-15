#include "mouse.hpp"

void Mouse::fitImage(int winWidth,int winHeight, int imgWidth, int imgHeight)
{
  // Compute ratio of the window and the image
  double winRatio = (double) winWidth / winHeight;
  double imgRatio = (double) imgWidth / imgHeight;

  // Check what is the best fit for the image according to the ratio
  if (imgRatio < winRatio)
    scale= (double) winHeight / imgHeight;
  else
    scale= (double) winWidth / imgWidth;

  // The focus is on the center of the image
  imgFocusX = imgWidth / 2.0;
  imgFocusY = imgHeight / 2.0;
}

#ifndef CAMERA_HPP_
#  define CAMERA_HPP_

#  include "simtypes.hpp"

#  define MIN_ZOOM (0.05f)

class Camera2D
{
public:

  // *************************************************************************************************
  // Construct a camera with parameters defining the zone of the world we want to observe.
  // By default the view is displayed on full range on the
  // *************************************************************************************************
  Camera2D(float32_t x, float32_t y, float32_t width, float32_t height)
  {
    // look at in the world
    cameraFitScreen(x, y, width, height);
    // Display at on the screen
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
    rotation_ = 0.0f;
    zoom_ = 1.0f;
  }
  // *************************************************************************************************
  // Construct a camera with default parameters defining the zone of the world to display
  // *************************************************************************************************
  Camera2D()
  {
    cameraFitScreen(0U, 0U, 800U, 600U);
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
    rotation_ = 0.0f;
    zoom_ = 1.0f;
  }

  // *************************************************************************************************
  // (x, y) is the top left position of the screen
  // *************************************************************************************************
  void cameraFitScreen(float32_t x, float32_t y, float32_t width, float32_t height)
  {
    look_at_x_ = x + width / 2.0f;
    look_at_y_ = y + height / 2.0f;
    look_at_width_ = width;
    look_at_height_ = height;
  }

  // *************************************************************************************************
  // Change the display zone.
  // *************************************************************************************************
  void displayAt(float32_t x, float32_t y, float32_t width, float32_t height)
  {
    if ((x > 1.0f) || (y > 1.0f) || (width > 1.0f) || (height > 1.0f))
      {
        std::cerr << "Error: parameters for displayAt(x, y, width, height) are ratio of the screen size." << std::endl;
      }
    else
      {
        display_at_x_ = x;
        display_at_y_ = y;
        display_at_width_ = width;
        display_at_height_ = height;
      }
  }
  void lookAt(float32_t x, float32_t y)
  {
    look_at_x_ = x;
    look_at_y_ = y;
  }
  void moveAt(float32_t x, float32_t y)
  {
    look_at_x_ = x;
    look_at_y_ = y;
  }
  void lookAtOffset(float32_t offsetX, float32_t offsetY)
  {
    lookAt(look_at_x_ + offsetX, look_at_y_ + offsetY);
  }
  void moveOffset(float32_t offsetX, float32_t offsetY)
  {
    moveAt(look_at_x_ + offsetX, look_at_y_ + offsetY);
  }
  void setCameraSize(float32_t width, float32_t height)
  {
    look_at_width_ = width;
    look_at_height_ = height;
  }
  void setZoom(float32_t factor)
  {
    if (factor < MIN_ZOOM)
      {
        zoom_ = MIN_ZOOM;
      }
    else
      {
        zoom_ = factor;
      }
  }
  void zoomAt(float32_t x, float32_t y, float32_t factor)
  {
    lookAt(x, y);
    setZoom(factor);
  }
  void zoomScrollAt(float32_t x, float32_t y, float32_t delta_scroll, float32_t width, float32_t height)
  {
    setZoom(zoom_ * (1.0f - delta_scroll));
    lookAt((width / 2.0f - x) / zoom_, (height / 2.0f - y) / zoom_);
  }
  void zoomfitImage(float32_t window_width, float32_t window_height, float32_t image_width, float32_t image_height);
  float32_t getZoom()
  {
    return zoom_;
  }
  const float32_t* getTransform();

  //private:
  float32_t rotation_;
  float32_t zoom_;
  float32_t look_at_x_;          // in pixel
  float32_t look_at_y_;          // in pixel
  float32_t look_at_width_;      // in pixel
  float32_t look_at_height_;     // in pixel

  float32_t display_at_x_;       // screen ratio
  float32_t display_at_y_;       // screen ratio
  float32_t display_at_width_;   // screen ratio
  float32_t display_at_height_;  // screen ratio

  float32_t matrix[16];
};

#endif /* CAMERA_HPP_ */

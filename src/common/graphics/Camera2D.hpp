#ifndef CAMERA_HPP_
#  define CAMERA_HPP_

#  include <cstdint>
#  include <iostream>
#  include <cmath>

#  define MIN_ZOOM (0.05f)

class Camera2D
{
public:
  // *************************************************************************************************
  // Construct a camera with parameters the zone of the world we want to observe: x, y is the center
  // of view and width, height the view dimension. By default the view is displayed on full range on
  // the screen region (window).
  // *************************************************************************************************
 Camera2D(const float x, const float y, const float width, const float height)
  {
    // World observation
    lookAt(x, y, width, height);
    zoom_ = 1.0f;

    // Screen ratio
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
  }

  Camera2D()
  {
    // World observation
    lookAt(400, 300, 800, 600);
    zoom_ = 1.0f;

    // Screen ratio
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
  }

  // *************************************************************************************************
  // Change the position on the window screen (values are ratio of the screen size)
  // *************************************************************************************************
  void displayAt(const float x, const float y, const float width, const float height)
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

  // *************************************************************************************************
  // Change the world observation: x, y is the center of view and width, height the view dimension.
  // *************************************************************************************************
  void lookAt(const float x, const float y, const float width, const float height)
  {
    look_at_x_ = x;
    look_at_y_ = y;
    look_at_width_ = width;
    look_at_height_ = height;
  }

  // *************************************************************************************************
  // Change the world observation: x, y is the center of view.
  // *************************************************************************************************
  void lookAt(const float x, const float y)
  {
    look_at_x_ = x;
    look_at_y_ = y;
  }

  // *************************************************************************************************
  // Deplace the word observation from an offset
  // *************************************************************************************************
  void moveOffset(const float offsetX, const float offsetY)
  {
    look_at_x_ += offsetX;
    look_at_y_ += offsetY;
  }

  // *************************************************************************************************
  // Change dimension of the observed world
  // *************************************************************************************************
  void setDimension(const float width, const float height)
  {
    look_at_width_ = width;
    look_at_height_ = height;
  }

  // *************************************************************************************************
  // Zoom on the observed world
  // *************************************************************************************************
  void zoom(const float factor)
  {
    if (factor < MIN_ZOOM)
      {
        zoom_ = MIN_ZOOM;
      }
    else
      {
        zoom_ = factor;
      }
    std::cout << "New zoom " << zoom_ << "\n";
  }

  // *************************************************************************************************
  // Zoom on the observed world
  // *************************************************************************************************
  void zoomOffset(const float factor)
  {
    zoom_ = zoom_ * (1.0 + factor);
  }

  // *************************************************************************************************
  // Look at given location and zoom
  // *************************************************************************************************
  void zoomAt(const float x, const float y, const float factor)
  {
    lookAt(x, y);
    zoomOffset(factor);
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  void zoomFit(const float window_width, const float window_height,
               const float image_width, const float image_height)
  {
    lookAt(window_width / 2.0f, window_height / 2.0f);
    std::cout << window_width << "/" << image_width << ", "
              << window_height << "/" << image_height << "==> min("
              << window_width / image_width << ", " << window_height / image_height
              << ")\n";

    zoom(std::min(window_width / image_width, window_height / image_height));
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  float getZoom() const
  {
    return zoom_;
  }

  // *************************************************************************************************
  //
  // *************************************************************************************************
  const float* getTransform();

  //private:
  float zoom_;
  float look_at_x_;          // in pixel
  float look_at_y_;          // in pixel
  float look_at_width_;      // in pixel
  float look_at_height_;     // in pixel

  float display_at_x_;       // screen ratio
  float display_at_y_;       // screen ratio
  float display_at_width_;   // screen ratio
  float display_at_height_;  // screen ratio

  float matrix[16];
};

inline std::ostream& operator<<(std::ostream& os, const Camera2D& c)
{
  os << "Camera2D((x: " << c.look_at_x_ << ", y: " << c.look_at_y_
     << "), (W: " << c.look_at_width_ << ", H: " << c.look_at_height_
     << "))";
  return os;
}

#endif /* CAMERA_HPP_ */

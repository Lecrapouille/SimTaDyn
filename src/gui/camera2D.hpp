#ifndef CAMERA_HPP_
#  define CAMERA_HPP_

#  include "../simtypes.hpp"

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
    lookAt(x, y, width, height);
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
    lookAt(0U, 0U, 800U, 600U);
    displayAt(0.0f, 0.0f, 1.0f, 1.0f);
    rotation_ = 0.0f;
    zoom_ = 1.0f;
  }

  // *************************************************************************************************
  // Change the zone of the world we want to observe.
  // *************************************************************************************************
  void lookAt(float32_t x, float32_t y, float32_t width, float32_t height)
  {
    look_at_x_ = x + width / 2.0f;
    look_at_y_ = y + height / 2.0f;
    look_at_width_ = width;
    look_at_height_ = height;
    look_at_orig_width_ = look_at_width_;
    look_at_orig_height_ = look_at_height_;
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
  void moveAt(float32_t x, float32_t y)
  {
    look_at_x_ = x;
    look_at_y_ = y;
  }
  void getCenter(float32_t& x, float32_t& y)
  {
    x = look_at_x_;
    y = look_at_y_;
  }
  void setSize(float32_t width, float32_t height)
  {
    look_at_width_ = width * zoom_;
    look_at_height_ = height * zoom_;
    look_at_orig_width_ = look_at_width_;
    look_at_orig_height_ = look_at_height_;
  }
  void getSize(float32_t& width, float32_t& height)
  {
    width = look_at_width_;
    height = look_at_height_;
  }
  void moveOffset(float32_t offsetX, float32_t offsetY)
  {
    //std::cout << "Move offset " << offsetX << " " << offsetY << std::endl;
    moveAt(look_at_x_ + offsetX, look_at_y_ + offsetY);
  }
  void setZoom(float32_t factor)
  {
    zoom_ = factor;
    look_at_width_ = look_at_orig_width_ * zoom_;
    look_at_height_ = look_at_orig_height_ * zoom_;
  }
  void zoom(float32_t factor)
  {
    setZoom(zoom_ + factor);
  }
  void getZoom(float32_t& factor)
  {
    factor = zoom_;
  }
  const float32_t* getTransform();

  //private:
  float32_t rotation_;
  float32_t zoom_;
  float32_t look_at_x_;          // in pixel
  float32_t look_at_y_;          // in pixel
  float32_t look_at_width_;      // in pixel
  float32_t look_at_height_;     // in pixel
  float32_t look_at_orig_width_;      // in pixel
  float32_t look_at_orig_height_;     // in pixel

  float32_t display_at_x_;       // screen ratio
  float32_t display_at_y_;       // screen ratio
  float32_t display_at_width_;   // screen ratio
  float32_t display_at_height_;  // screen ratio

  float32_t matrix[16];
};

#endif /* CAMERA_HPP_ */

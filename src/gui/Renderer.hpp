#ifndef RENDERER_HPP_
#  define RENDERER_HPP_

#  include "OpenGL.hpp"
#  include "Color.hpp"
#  include "Camera2D.hpp"

// Defined in Drawable.hpp
class Drawable;


class Renderer
{
public:
  Renderer()
    : default_camera_(),
      current_camera_()
  {
  }

  void initialize();
  void clearScreen(const Color& color);
  void changeCamera2D(const Camera2D& camera);
  Camera2D& getCamera2D();
  void resetCamera2D();
  void applyViewport(Camera2D& camera);
  void applyViewport();
  //void draw(const Drawable& drawable);
  virtual uint32_t getScreenWidth() const = 0;
  virtual uint32_t getScreenHeight() const = 0;

private:
  Camera2D default_camera_;
  Camera2D current_camera_;
};

#endif /* RENDERER_HPP_ */

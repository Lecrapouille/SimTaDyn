#include "Renderer.hpp"

void Renderer::initialize()
{
  default_camera_.cameraFitScreen(0.0f, 0.0f,
                                  static_cast<float32_t>(getScreenWidth()),
                                  static_cast<float32_t>(getScreenHeight()));
  current_camera_ = default_camera_;
}

void Renderer::applyViewport(Camera2D& camera)
{
  float32_t width  = static_cast<float32_t>(getScreenWidth());
  float32_t height = static_cast<float32_t>(getScreenHeight());
  uint32_t rect[4];

  rect[0] = static_cast<int>(0.5f + width  * camera.display_at_x_);
  rect[1] = static_cast<int>(0.5f + height * camera.display_at_y_);
  rect[2] = static_cast<int>(0.5f + width  * camera.display_at_width_);
  rect[3] = static_cast<int>(0.5f + height * camera.display_at_height_);

  glCheck(glViewport(rect[0], rect[1], rect[2], rect[3]));
  glCheck(glMatrixMode(GL_PROJECTION));
  glCheck(glLoadMatrixf(camera.getTransform()));
  glCheck(glMatrixMode(GL_MODELVIEW));
}

void Renderer::applyViewport()
{
  applyViewport(current_camera_);
}

void Renderer::clearScreen(const Color& color)
{
  glCheck(glClearColor(color.r, color.g, color.b, color.a));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::changeCamera2D(const Camera2D& camera)
{
  current_camera_ = camera;
}

Camera2D& Renderer::getCamera2D()
{
  return current_camera_;
}

void Renderer::resetCamera2D()
{
  current_camera_ = default_camera_;
}

/*void Renderer::draw(const Drawable& drawable)
{
  drawable.draw(*this);
}
*/

//void viewPort();

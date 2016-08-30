#ifndef RENDERER_HPP_
#  define RENDERER_HPP_

#  ifdef __APPLE__
#    include <GLUT/glut.h>
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#  else
#    include <GL/glut.h>
#    include <GL/gl.h>
#    include <GL/glu.h>
#  endif

#  include "Color.hpp"
#  include "Camera2D.hpp"

class Renderer
{
public:
  Renderer() : default_camera_(), current_camera_() { }
  void initialize();
  void clearScreen(const Color& color);
  void changeCamera2D(const Camera2D& camera);
  Camera2D& getCamera2D();
  void resetCamera2D();
  void applyViewport(Camera2D& camera);
  void applyViewport();
  virtual uint32_t getScreenWidth() const = 0;
  virtual uint32_t getScreenHeight() const = 0;

private:
  Camera2D default_camera_;
  Camera2D current_camera_;
};

#endif /* RENDERER_HPP_ */

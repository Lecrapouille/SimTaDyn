#ifndef GLWINDOW_HPP_
#  define GLWINDOW_HPP_

#  include <iostream>
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#  include "GLObject.hpp"

// Return the number of elements in an array
#  ifndef ARRAY_SIZE
#    define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#  endif

class IGLWindow
{
public:

  virtual ~IGLWindow() {}
  int create();
  virtual bool setup() = 0;
  virtual bool draw() = 0;
  void update();
  void close();
  inline float dt() const
  {
    return m_deltaTime;
  }

private:

  void FPS();

  double m_lastTime;
  double m_lastFrameTime;
  int m_nbFrames;
  float m_deltaTime;

protected:

  const int m_width = 1024;
  const int m_height = 768;
  const char *m_title = "Example";
  GLFWwindow *m_window;
};

#endif /* GLWINDOW_HPP_ */

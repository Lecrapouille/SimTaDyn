#ifndef EXAMPLE_02_HPP_
#  define EXAMPLE_02_HPP_

#  include "OpenGL.hpp"
#  include "Movable.tpp"

#define FRMBFF 0
#define SCREEN 1

class GLExample02: public IGLWindow
{
public:

  GLExample02()
    : m_prog0("fb")//,
      //m_prog1("screen")
  {
  }

  ~GLExample02()
  {
    std::cout << "Bye" << std::endl;
  }

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw()  override;

public:

  GLVAO              m_cubeVAO;
  GLVAO              m_floorVAO;
  GLVAO              m_screenVAO;
  GLVertexShader     m_vs[2];
  GLFragmentShader   m_fs[2];
  GLProgram          m_prog0;
  //GLProgram          m_prog1;
  Movable<float, 3U> m_movable;
};

#  endif /* EXAMPLE_02_HPP_ */

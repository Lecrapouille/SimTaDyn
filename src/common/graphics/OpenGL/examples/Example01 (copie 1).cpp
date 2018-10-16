#include "Example01.hpp"
#include <math.h>

bool GLExample01::setup()
{
  LOGE("GLExample01::setup");
  m_vs.loadFromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/Example01.vertex");
  m_fs.loadFromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/Example01.fragment");
  std::cout << "VS: " << m_vs.gpuID() << std::endl;
  std::cout << "FS: " << m_fs.gpuID() << std::endl;

  m_quad.attachShader(m_vs).attachShader(m_fs);
  m_quad.begin();

  if (!m_quad.hasUniform("scale"))
    {
      std::cout << "perdu" << std::endl;
      return false;
    }

  std::cout << m_quad["scale"].name() << std::endl;
  std::cout << typeid(m_quad["scale"]).name() << std::endl;

  /*
  ** TODO: m_quad["scale"] = 4.0f;
  ** GLUniform<float> &scale = m_quad["scale"];
  ** scale = 2.0f;
  */

  //std::cout << "AVANT " << m_quad.getUniformVal<float>("scale") << std::endl;
  m_quad.setUniformVal("scale", 4.0f);
  std::cout << "Apres " << m_quad.getUniformVal<float>("scale") << std::endl;
  m_quad.uniform<float>("scale") = 1.0f;
  std::cout << "Apres " << m_quad.getUniformVal<float>("scale") << std::endl;

  /*
  ** m_quad["color"] = {
  **   Vector4f(1, 0, 0, 1), Vector4f(0, 1, 0, 1),
  **   Vector4f(0, 0, 1, 1), Vector4f(1, 1, 0, 1)
  ** };
  */

  GLVertexBuffer<Vector4f> vbo("gg");
  vbo.qq = { Vector4f(4.0f), Vector4f(3.0f), Vector4f(2.0f) };
  std::cout << vbo.qq << std::endl;

  m_quad.attribute<Vector4f>("color")/*.data().qq*/ =
    {
      Vector4f(1, 0, 0, 1), Vector4f(0, 1, 0, 1),
      Vector4f(0, 0, 1, 1), Vector4f(1, 1, 0, 1),
    };

  std::cout << "COLOR: " << m_quad.attribute<Vector4f>("color").cdata().qq << std::endl;

  m_quad.attribute<Vector2f>("position") =
    {
      Vector2f(-1, -1), Vector2f(-1, +1),
      Vector2f(+1, -1), Vector2f(+1, +1),
    };

  return true;
}

bool GLExample01::draw()
{
  LOGE("GLExample01::draw");
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  const float deltaTime = dt();
  static float time = 0.0f;
  time += deltaTime;
  m_quad.uniform<float>("scale") = cosf(time);

  float ct = cosf(time) / 2.0f + 0.5f;
  float st = sinf(time) / 2.0f + 0.5f;
m_quad.attribute<Vector4f>("color") = {
  Vector4f(ct, st, st, 1), Vector4f(st, ct, st, 1),
  Vector4f(st, st, ct, 1), Vector4f(ct, ct, st, 1),
};

  m_quad.draw(GL_TRIANGLE_STRIP, 0, 4);

  return true;
}

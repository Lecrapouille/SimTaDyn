#include "Example01.hpp"

static GLfloat vertexData[] =
{
  // X      Y     Z
  0.0f,  0.8f, 0.0f,
 -0.8f, -0.8f, 0.0f,
  0.8f, -0.8f, 0.0f,
};

static GLfloat colorData[] =
{
  // R     G     B
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
};

bool GLExample01::setup()
{
  LOGI("GLExample01::setup()");

  m_pos.append(vertexData, ARRAY_SIZE(vertexData));
  m_col.append(colorData, ARRAY_SIZE(colorData));

  if (0U == m_shader.load("Example01.vertex", "Example01.fragment"))
    return false;
  
  m_vao.start();
  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_col.setup(m_shader, 3, GL_FLOAT);
  m_vao.stop();

  return true;
}

void GLExample01::moveMe()
{
  int c = 0;
  if (glfwGetKey(m_window, GLFW_KEY_UP))
    {
      c = 1;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
    {
      c = -1;
    }

  if (c != 0)
    {
      GLfloat v = m_pos.get(0);
      v = v + m_deltaTime * ((float) c) / 2.0f;
      m_pos.modify(0, v);
      m_pos.block(0)->start(); // m_pos.start(0) KOOOOO !!
      m_pos.block(0)->stop();
    }
}

bool GLExample01::draw()
{
  LOGI("GLExample01::draw()");

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // m_shader.draw(GL_TRIANGLES) {
  m_shader.start();
  m_vao.start();

  moveMe();

  glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));

  m_vao.stop();
  m_shader.stop();
// }

  return true;
}

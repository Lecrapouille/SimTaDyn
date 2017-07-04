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

  m_pos.add(vertexData, ARRAY_SIZE(vertexData));
  m_col.add(colorData, ARRAY_SIZE(colorData));

  if (0U == m_shader.load("Example01.vertex", "Example01.fragment"))
    return false;
  m_shader.start();

  m_pos/*_attrib*/.setup(m_shader, 3, GL_FLOAT);
  m_col/*_attrib*/.setup(m_shader, 3, GL_FLOAT);

  m_vao.start();
  {
    m_pos.start();
    //m_pos_attrib.start();
    m_col.start();
    //m_col_attrib.start();
  }
  m_vao.stop();

  return true;
}

void GLExample01::moveMe()
{

  if (glfwGetKey(m_window, GLFW_KEY_UP))
    {
      m_pos[0] += m_deltaTime / 2.0f;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
    {
      m_pos[0] -= m_deltaTime / 2.0f;
    }
  else
    {
      // Do nothing
    }
}

bool GLExample01::draw()
{
  LOGI("GLExample01::draw()");

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  // m_vao.draw(GL_TRIANGLES) {
  m_shader.start();
  m_vao.start();
  m_pos.start();
  m_col.start();

  moveMe();

  glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));

  m_col.stop();
  m_pos.stop();
  m_vao.stop();
  m_shader.stop();
// }

  return true;
}

#include "Example01.hpp"

//! \file Draw a colorfull triangle and allow the user to move one its vertices.

// Vertices for drawing a triangle
static GLfloat vertexData[] =
{
  // X      Y     Z
  0.0f,  0.8f, 0.0f,
 -0.8f, -0.8f, 0.0f,
  0.8f, -0.8f, 0.0f,
};

// Color for each triangle vertices
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

  // Copy arrays inside the VBO class
  m_pos.add(vertexData, ARRAY_SIZE(vertexData));
  m_col.add(colorData, ARRAY_SIZE(colorData));

  // Compile a shader program
  if (0U == m_shader.load("Example01.vertex", "Example01.fragment"))
    return false;

  // Tell to OpenGL how to manage VBO values. This fixes the size
  // of the VBO container to its current capacity (ie. now the VBO
  // size no longer be larger): the GPU have allocated static memory.
  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_col.setup(m_shader, 3, GL_FLOAT);

  for (uint32_t i = 0U; i < 9U; ++i)
    {
      std::cout << m_pos.m_data[i] << std::endl;
    }

  m_shader.start();
  {
    // Bind VBOs to the VAO. It's now enough for drawing primitives.
    m_vao.start();
    {
      m_pos.start();
      m_col.start();
    }
    m_vao.stop();
  }
  m_shader.stop();
  return true;
}

// Allow the user to displace one vertex with
// UP/DOWN key of the keyboard. Vertices new
// positions are automaticly updated to the GPU
// with the ::start() method.
void GLExample01::moveMe()
{
  const float deltaTime = dt();

  if (glfwGetKey(m_window, GLFW_KEY_UP))
    {
      m_pos[0] += deltaTime / 2.0f;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
    {
      m_pos[0] -= deltaTime / 2.0f;
    }
  else
    {
      // Do nothing
    }
}

// Draw the triangle
bool GLExample01::draw()
{
  LOGI("GLExample01::draw()");

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  m_shader.start();
  {
    m_vao.start();
    {
      m_pos.start();
      m_col.start();

      moveMe();

      glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));

      m_col.stop();
      m_pos.stop();
    }
    m_vao.stop();
  }
  m_shader.stop();

  return true;
}

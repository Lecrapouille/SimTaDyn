#include "Example02.hpp"

//! \file Example02.cpp
//! Draw a textured and colorfull triangle.

// Vertices for drawing a triangle
static GLfloat vertexData[] =
  {
    // X      Y     Z
    0.0f,  0.8f, 0.0f,
   -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
  };

// Texture position on each vertices
static GLfloat textureData[] =
  {
    // U     V
    0.5f, 1.0f - 1.0f,
    0.0f, 1.0f - 0.0f,
    1.0f, 1.0f - 0.0f,
  };

// Color for each triangle vertices
static GLfloat colorData[] =
{
  // R     G     B
  1.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
};

bool GLExample02::setup()
{
  LOGI("GLExample02::setup()");

  // Copy arrays inside the VBO class
  m_pos.add(vertexData, ARRAY_SIZE(vertexData));
  m_col.add(colorData, ARRAY_SIZE(colorData));
  m_tex.add(textureData, ARRAY_SIZE(textureData));

  // Compile a shader program
  if (0U == m_shader.load("Example02.vertex", "Example02.fragment"))
    return false;

  // Configure the texture
  m_texture.interpolation(GL_LINEAR);
  m_texture.wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_texture.load("hazard.png"))
    return false;

  // Tell to OpenGL how to manage VBO values. This fixes the size
  // of the VBO container to its current capacity (ie. now the VBO
  // size no longer be larger): the GPU have allocated static memory.
  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_col.setup(m_shader, 3, GL_FLOAT);
  m_tex.setup(m_shader, 2, GL_FLOAT);

  // Bind VBOs to the VAO. It's now enough for drawing primitives.
  m_shader.start();
  {
    m_vao.start();
    {
      m_pos.start();
      m_col.start();
      m_tex.start();
    }
    m_vao.stop();
  }
  m_shader.stop();
  return true;
}

// Draw the triangle
bool GLExample02::draw()
{
  LOGI("GLExample02::draw()");

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));

  m_shader.start();
  {
    m_vao.start();
    m_pos.start();
    m_col.start();

    // Set to 0 because the texture is bound to GL_TEXTURE0
    GLint TextureID = glCheck(glGetUniformLocation(m_shader, "tex"));
    glCheck(glUniform1i(TextureID, 0));
    glCheck(glActiveTexture(GL_TEXTURE0));
    m_texture.start();

    glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));

    m_col.stop();
    m_pos.stop();
    m_texture.stop();
    m_vao.stop();
  }
  m_shader.stop();

  return true;
}

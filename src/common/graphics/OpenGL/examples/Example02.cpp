#include "Example02.hpp"

static GLint TextureID;

static GLfloat vertexData[] =
  {
    // X      Y     Z
    0.0f,  0.8f, 0.0f,
   -0.8f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
  };

static GLfloat textureData[] =
  {
    // U     V
    0.5f, 1.0f - 1.0f,
    0.0f, 1.0f - 0.0f,
    1.0f, 1.0f - 0.0f,
  };
    
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

  m_pos.append(vertexData, ARRAY_SIZE(vertexData));
  m_col.append(colorData, ARRAY_SIZE(colorData));
  m_tex.append(textureData, ARRAY_SIZE(textureData));

  if (0U == m_shader.load("Example02.vertex", "Example02.fragment"))
    return false;

  m_texture.interpolation(GL_LINEAR);
  m_texture.wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_texture.load("hazard.png"))
    return false;

  m_vao.start();

  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_col.setup(m_shader, 3, GL_FLOAT);
  m_tex.setup(m_shader, 2, GL_FLOAT);

  m_vao.stop();
  return true;
}

bool GLExample02::draw()
{
  LOGI("GLExample02::draw()");

  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));
    
  m_shader.start();
  m_vao.start();

  // Set to 0 because the texture is bound to GL_TEXTURE0
  TextureID  = glCheck(glGetUniformLocation(m_shader, "tex"));
  glCheck(glUniform1i(TextureID, 0));

  glCheck(glActiveTexture(GL_TEXTURE0));
  m_texture.start();

  glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));  

  m_texture.stop();
  m_vao.stop();
  m_shader.stop();

  return true;
}

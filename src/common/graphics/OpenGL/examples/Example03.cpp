#include "Example03.hpp"

//! \file Example03.hpp
//! Draw a textured cube and display it with perspective.

//FIXME: use index.

// Vertices for drawing a cube
static GLfloat vertexData[] = {
  //  X     Y     Z
  // bottom
  -1.0f,-1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f, 1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,

  // top
  -1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,

  // front
  -1.0f,-1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,

  // back
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,

  // left
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,

  // right
  1.0f,-1.0f, 1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f,-1.0f, 1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f, 1.0f, 1.0f,
};

// Teaxture position on each vertices
static GLfloat textureData[] = {
  //  U     V
  // bottom
  0.0f, 0.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 1.0f,

  // top
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,

  // front
  1.0f, 0.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,

  // back
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,

  // left
  0.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 0.0f,

  // right
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  0.0f, 1.0f
};

// FIXME: m_shader::setUniform
void GLExample03::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id = glCheck(glGetUniformLocation(m_shader.getID(), name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

bool GLExample03::setup()
{
  LOGI("GLExample03::setup()");

  // Enable the depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Copy arrays inside the VBO class
  m_pos.add(vertexData, ARRAY_SIZE(vertexData));
  m_tex.add(textureData, ARRAY_SIZE(textureData));

  // Compile a shader program
  if (0U == m_shader.load("Example03.vertex", "Example03.fragment"))
    return false;

  // Configure the texture
  m_texture.interpolation(GL_LINEAR);
  m_texture.wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_texture.load("wooden-crate.jpg"))
    return false;

  // Tell to OpenGL how to manage VBO values. This fixes the size
  // of the VBO container to its current capacity (ie. now the VBO
  // size no longer be larger): the GPU have allocated static memory.
  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_tex.setup(m_shader, 2, GL_FLOAT);

  // Bind VBOs to the VAO. It's now enough for drawing primitives.
  m_shader.start();
  {
    m_vao.start();
    {
      m_pos.start();
      m_tex.start();
    }
    m_vao.stop();
  }
  m_shader.stop();
  return true;
}

// Allow the user to displace and rotate the cube
// withUP/DOWN/LEFT/RIGHT key of the keyboard.
// New vertices positions are automaticly updated
// to the GPU with the ::start() method.
void GLExample03::moveMe()
{
  const float deltaTime = dt();
  // Sign of translation, rotation
  int r = 0;
  int t = 0;

  // t for translation
  if (glfwGetKey(m_window, GLFW_KEY_RIGHT))
    {
      t = 1;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_LEFT))
    {
      t = -1;
    }

  if (t != 0)
    {
      const GLfloat offset = deltaTime * ((float) t) / 1.0f;
      m_movable.move(Vector3f(offset));
    }

  // r for rotation
  if (glfwGetKey(m_window, GLFW_KEY_UP))
    {
      r = 1;
    }
  else if (glfwGetKey(m_window, GLFW_KEY_DOWN))
    {
      r = -1;
    }

  if (r != 0)
    {
      const GLfloat degreesPerSecond = 80.0f;
      m_degreesRotated += r * deltaTime * degreesPerSecond;
      m_degreesRotated = maths::wrapTo180(m_degreesRotated);
      m_movable.rotate(maths::radians(m_degreesRotated), Vector3f(0, 1, 0));
    }
}

// Draw the cube
bool GLExample03::draw()
{
  LOGI("GLExample03::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_shader.start();
  m_vao.start();

  // Model matrix transformation
  setUniform("model", m_movable.transform());

  // Projection matrix transformation
  float ratio = ((float) m_width) / ((float) m_height);
  Matrix44f projection = matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  setUniform("projection", projection);

  // View matrix transformation
  Matrix44f camera = matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));
  setUniform("camera", camera);

  //set to 0 because the texture is bound to GL_TEXTURE0
  GLint TextureID = glCheck(glGetUniformLocation(m_shader.getID(), "tex"));
  glCheck(glUniform1i(TextureID, 0));
  glCheck(glActiveTexture(GL_TEXTURE0));
  m_texture.start();

  moveMe();

  glCheck(glDrawArrays(GL_TRIANGLES, 0, 6*2*3));

  m_texture.stop();
  m_vao.stop();
  m_shader.stop();

  return true;
}

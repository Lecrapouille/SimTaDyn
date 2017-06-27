#include "Example03.hpp"

static float degreesRotated = 0.0f;
static Movable<float, 3U> movable;
static GLint TextureID;

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

// FIXME
void GLExample03::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id  = glCheck(glGetUniformLocation(m_shader, name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

bool GLExample03::setup()
{
  LOGI("GLExample03::setup()");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_pos.append(vertexData, ARRAY_SIZE(vertexData));
  m_tex.append(textureData, ARRAY_SIZE(textureData));

  if (0U == m_shader.load("Example03.vertex", "Example03.fragment"))
    return false;

  m_texture.interpolation(GL_LINEAR);
  m_texture.wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_texture.load("wooden-crate.jpg"))
    return false;

  m_vao.start();

  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_tex.setup(m_shader, 2, GL_FLOAT);

  m_vao.stop();
  return true;
}

void GLExample03::moveMe()
{
  // Sign of translation, rotation
  int r = 0;
  int t = 0;

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
      const GLfloat offset = m_deltaTime * ((float) t) / 1.0f;
      movable.move(Vector3f(offset));
    }


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
      degreesRotated += r * m_deltaTime * degreesPerSecond;
      degreesRotated = maths::wrapTo180(degreesRotated);
      movable.rotate(maths::radians(degreesRotated), Vector3f(0, 1, 0));
    }
}

bool GLExample03::draw()
{
  LOGI("GLExample03::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_shader.start();
  m_vao.start();

  /*m_shader.*/setUniform("model", movable.transform());
  float ratio = ((float) m_width) / ((float) m_height);
  Matrix44f projection = matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  /*m_shader.*/setUniform("projection", projection);
  Matrix44f camera = matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));
  /*m_shader.*/setUniform("camera", camera);

  TextureID  = glCheck(glGetUniformLocation(m_shader, "tex"));
  glCheck(glUniform1i(TextureID, 0)); //set to 0 because the texture is bound to GL_TEXTURE0

  glCheck(glActiveTexture(GL_TEXTURE0));
  m_texture.start();

  moveMe();

  glCheck(glDrawArrays(GL_TRIANGLES, 0, 6*2*3));

  m_texture.stop();
  m_vao.stop();
  m_shader.stop();

  return true;
}

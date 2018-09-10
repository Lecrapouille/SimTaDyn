#include "Example04.hpp"

// FIXME
void GLExample04::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id  = glCheck(glGetUniformLocation(m_shader.getID(), name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

bool GLExample04::setup()
{
  LOGI("GLExample04::setup()");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (0U == m_shader.load("Example04.vertex", "Example04.fragment"))
    return false;

  return m_cube.setup(m_shader);
}

bool GLExample04::draw()
{
  LOGI("GLExample04::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_shader.start();
  {
    /*m_shader.*/setUniform("model", movable.transform());
    float ratio = ((float) m_width) / ((float) m_height);
    Matrix44f projection = matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
    /*m_shader.*/setUniform("projection", projection);
    Matrix44f camera = matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));
    /*m_shader.*/setUniform("camera", camera);


    m_cube.draw();
  }
  m_shader.stop();

  return true;
}

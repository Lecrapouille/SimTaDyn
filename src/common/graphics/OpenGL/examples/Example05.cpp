#include "Example05.hpp"

// FIXME
void GLExample05::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id  = glCheck(glGetUniformLocation(m_shader, name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

bool GLExample05::setup()
{
  LOGI("GLExample05::setup()");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (0U == m_shader.load("Example05.vertex", "Example05.fragment"))
    return false;

  m_shader.start();
  {
    m_robot1->setup(m_shader);
    m_robot2->setup(m_shader);
    m_robot3->setup(m_shader);

    // Projection matrix
    float ratio = ((float) m_width) / ((float) m_height);
    Matrix44f m = matrix::perspective(maths::radians(50.0f), ratio, 0.01f, 10000.0f);
    setUniform("projection", m);

    // Camera matrix
    m = matrix::lookAt(Vector3f(0.0f, 10.0f, 100.0f), Vector3f(30), Vector3f(0,1,0));
    setUniform("camera", m);
  }
  m_shader.stop();

  return true;
}

void GLExample05::drawNode(SceneNode<float, 3U> &node)
{
  LOGI("Renderer:drawNode '%s'", node.m_name.c_str());

  Mesh *m = node.mesh();
  if (nullptr != m)
    {
      Matrix44f transform = matrix::scale(node.worldTransform(), node.scaleF());
      setUniform("model", transform);
      m->draw(GL_TRIANGLES);
    }

  // FIXME
  for (auto i: node.m_childrens)
    {
      drawNode(*i);
    }
  // FIXME
  /*  for (auto i: node.cbegin(); i != node.cend(); ++i)
    {
      drawNode(*i);
    }
  */
}

bool GLExample05::draw()
{
  LOGI("GLExample05::update()");

  if (nullptr == m_root)
    return false;

  m_root->update(m_deltaTime);

  LOGI("GLExample05::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_shader.start();
  {
    drawNode(*m_root);
  }
  m_shader.stop();

  return true;
}

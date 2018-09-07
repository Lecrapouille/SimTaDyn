#include "Example06.hpp"

//! \file Example06.cpp
//! Use Example06.cpp but add HMI made in OpenGL

bool GLImGUI::render()
{
  ImGui::Begin("Hello, world!");
  ImGui::Text("This is some useful text.");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  if (ImGui::Button("Hello"))
    {
      std::cout << "Hello" << std::endl;
    }
  ImGui::End();

  return true;
}

GLExample06::~GLExample06()
{
  delete m_root;
}

// FIXME
void GLExample06::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id  = glCheck(glGetUniformLocation(m_shader, name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

bool GLExample06::setup()
{
  LOGI("GLExample06::setup()");

  // Init the context of the library
  if (false == m_gui.setup(*this))
    return false;

  // Enable the depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Compile a shader program
  if (0U == m_shader.load("Example06.vertex", "Example06.fragment"))
    return false;

  // Tell to OpenGL how to manage VBO values. This fixes the size
  // of the VBO container to its current capacity (ie. now the VBO
  // size no longer be larger): the GPU have allocated static memory.
  m_robot1->setup(m_shader);
  m_robot2->setup(m_shader);
  m_robot3->setup(m_shader);

  m_shader.start();
  {
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

// Draw recursively a node from a scene graph
void GLExample06::drawNode(SceneNode_t &node)
{
  LOGI("Renderer:drawNode '%s'", node.m_name.c_str());

  Mesh *m = node.mesh();
  if (nullptr != m)
    {
      Matrix44f transform = matrix::scale(node.worldTransform(), node.localScale());
      setUniform("model", transform);
      m->draw(GL_TRIANGLES);
    }

  std::vector<SceneNode_t*> const &children = node.children();
  for (auto i: children)
    {
      drawNode(*i);
    }
}

// Draw scene graph (made of robots)
bool GLExample06::draw()
{
  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  if (nullptr == m_root)
    return false;
  m_root->update(dt());

  m_shader.start();
  {
    drawNode(*m_root);
  }
  m_shader.stop();

  if (false == m_gui.draw())
    return false;

  return true;
}

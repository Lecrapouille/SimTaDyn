#ifndef EXAMPLE_06_HPP_
#  define EXAMPLE_06_HPP_

#  include "GLImGUI.hpp"
#  include "Cube.hpp"
#  include "SceneGraph.tpp"
#  include "Example05.hpp"

class GLImGUI: public IGLImGUI
{
public:

  GLImGUI();
  inline void setNode(SceneNode_t &root)
  {
    m_root = &root;
  }

protected:

  virtual bool render() override;
  void drawNode(SceneNode_t &node);

private:

  SceneNode_t *m_root;
};

class GLExample06: public IGLWindow
{
public:

  GLExample06()
    : m_shader("Shader05")
  {
    m_root = new SceneNode_t("Root");
    m_robot1 = new CubicRobot("CubicRobot1");
    m_robot2 = new CubicRobot("CubicRobot2");
    m_robot3 = new CubicRobot("CubicRobot3");

    m_root->add(*m_robot1);
    m_root->add(*m_robot2);
    m_root->add(*m_robot3);
    m_robot2->position(Vector3f(30.0f, 0.0f, 0.0f));
    m_robot3->position(Vector3f(60.0f, 0.0f, 0.0f));
  }

  ~GLExample06();

  virtual bool setup() override;
  virtual bool draw() override;
  void hmi();
  void drawNode(SceneNode_t &node);
  void setUniform(const char *name, Matrix44f const &mat);

  GLShader     m_shader;
  CubicRobot  *m_robot1;
  CubicRobot  *m_robot2;
  CubicRobot  *m_robot3;
  SceneNode_t *m_root;
  GLImGUI      m_gui;
};

#  endif /* EXAMPLE_06_HPP_ */

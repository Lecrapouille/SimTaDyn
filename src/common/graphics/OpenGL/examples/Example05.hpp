#ifndef EXAMPLE_05_HPP_
#  define EXAMPLE_05_HPP_

#  include "GLWindow.hpp"
#  include "GLShader.hpp"
#  include "Cube.hpp"
#  include "SceneGraph.tpp"

class CubicRobot: public SceneNode<float, 3U>
{
public:

  CubicRobot(const char *name)
    : SceneNode(nullptr, name)
  {
    // Body
    m_body = new SceneNode(m_cube, "Body");
    m_body->localScale(Vector3f(10.0f, 15.0f, 5.0f));
    m_body->position(Vector3f(0.0f, 35.0f, 0.0f));
    add(*m_body);

    // Head
    m_head = new SceneNode(m_cube, "Head");
    m_head->localScale(Vector3f(5.0f));
    m_head->position(Vector3f(0.0f, 30.0f, 0.0f));
    m_body->add(*m_head);

    // Left arm
    m_leftArm = new SceneNode(m_cube, "Left Arm");
    m_leftArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
    m_leftArm->position(Vector3f(-12.0f, 30.0f, -1.0f));
    m_body->add(*m_leftArm);

    // Right arm
    m_rightArm = new SceneNode(m_cube, "Right Arm");
    m_rightArm->localScale(Vector3f(3.0f, -18.0f, 3.0f));
    m_rightArm->position(Vector3f(12.0f, 30.0f, -1.0f));
    m_body->add(*m_rightArm);

    // Left leg
    m_leftLeg = new SceneNode(m_cube, "Left Leg");
    m_leftLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
    m_leftLeg->position(Vector3f(-8.0f, 0.0f, 0.0f));
    m_body->add(*m_leftLeg);

    // Right leg
    m_rightLeg = new SceneNode(m_cube, "Right Leg");
    m_rightLeg->localScale(Vector3f(3.0f, -17.5f, 3.0f));
    m_rightLeg->position(Vector3f(8.0f, 0.0f, 0.0f));
    m_body->add(*m_rightLeg);
  }

  ~CubicRobot()
  {
  }

  inline void setup(GLShader &shader)
  {
    // Cube
    m_cube.setup(shader);
  }

  virtual void update(float const dt) override
  {
    LOGI("Robot::update");

    const GLfloat degreesPerSecond = 1.0f;
    degreesRotated += dt * degreesPerSecond;
    degreesRotated = maths::wrapTo180(degreesRotated);

    rotate(degreesRotated, Vector3f(0.0f, 1.0f, 0.0f));
    m_head->rotate(-degreesRotated, Vector3f(0.0f, 1.0f, 0.0f));
    m_leftArm->rotate(-degreesRotated, Vector3f(1.0f, 0.0f, 0.0f));
    m_rightArm->rotate(degreesRotated, Vector3f(1.0f, 0.0f, 0.0f));
    SceneNode::update(dt);
  }

protected:

  MeshCube              m_cube;
  SceneNode<float, 3U> *m_body;
  SceneNode<float, 3U> *m_head;
  SceneNode<float, 3U> *m_leftArm;
  SceneNode<float, 3U> *m_rightArm;
  SceneNode<float, 3U> *m_leftLeg;
  SceneNode<float, 3U> *m_rightLeg;
  float degreesRotated = 0.0f;
};

class GLExample05: public IGLWindow
{
public:

  GLExample05()
    : m_shader("Shader05")
  {
    m_root = new SceneNode<float, 3U>("Root");
    m_robot1 = new CubicRobot("CubicRobot1");
    m_robot2 = new CubicRobot("CubicRobot2");
    m_robot3 = new CubicRobot("CubicRobot3");

    m_root->add(*m_robot1);
    m_root->add(*m_robot2);
    m_root->add(*m_robot3);
    m_robot2->position(Vector3f(30.0f, 0.0f, 0.0f));
    m_robot3->position(Vector3f(60.0f, 0.0f, 0.0f));
  }

  ~GLExample05()
  {
    delete m_root;
    std::cout << "Bye" << std::endl;
  }

  virtual bool setup() override;
  virtual bool draw() override;
  void drawNode(SceneNode<float, 3U> &node);
  void setUniform(const char *name, Matrix44f const &mat);

  GLShader m_shader;
  CubicRobot *m_robot1;
  CubicRobot *m_robot2;
  CubicRobot *m_robot3;
  SceneNode<float, 3U> *m_root;
};

#  endif /* EXAMPLE_05_HPP_ */

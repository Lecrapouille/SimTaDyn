#include "Example01.hpp"
#include <math.h>


bool GLExample01::setup()
{
  LOGD("GLExample01::setup");

  // Enable the depth buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glLineWidth(10.0f);

  m_vs.loadFromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/Example01.vertex");
  m_fs.loadFromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/Example01.fragment");
  m_quad.attachShader(m_vs).attachShader(m_fs);
  m_quad.begin(); // FIXME a enlever

// TODO tester index
  // TODO Faire qu'automatiquement on charge une liste de float en vector3f
  m_quad.attribute<Vector3f>("a_position") =
    {
	  //  X     Y     Z
	  // bottom
	  Vector3f(-1.0f,-1.0f,-1.0f),
	  Vector3f(1.0f,-1.0f,-1.0f),
	  Vector3f(-1.0f,-1.0f, 1.0f),
	  Vector3f(1.0f,-1.0f,-1.0f),
	  Vector3f(1.0f,-1.0f, 1.0f),
	  Vector3f(-1.0f,-1.0f, 1.0f),

	  // top
	  Vector3f(-1.0f, 1.0f,-1.0f),
	  Vector3f(-1.0f, 1.0f, 1.0f),
	  Vector3f(1.0f, 1.0f,-1.0f),
	  Vector3f(1.0f, 1.0f,-1.0f),
	  Vector3f(-1.0f, 1.0f, 1.0f),
	  Vector3f(1.0f, 1.0f, 1.0f),

	  // front
	  Vector3f(-1.0f,-1.0f, 1.0f),
	  Vector3f(1.0f,-1.0f, 1.0f),
	  Vector3f(-1.0f, 1.0f, 1.0f),
	  Vector3f(1.0f,-1.0f, 1.0f),
	  Vector3f(1.0f, 1.0f, 1.0f),
	  Vector3f(-1.0f, 1.0f, 1.0f),

	  // back
	  Vector3f(-1.0f,-1.0f,-1.0f),
	  Vector3f(-1.0f, 1.0f,-1.0f),
	  Vector3f(1.0f,-1.0f,-1.0f),
	  Vector3f(1.0f,-1.0f,-1.0f),
	  Vector3f(-1.0f, 1.0f,-1.0f),
	  Vector3f(1.0f, 1.0f,-1.0f),

	  // left
	  Vector3f(-1.0f,-1.0f, 1.0f),
	  Vector3f(-1.0f, 1.0f,-1.0f),
	  Vector3f(-1.0f,-1.0f,-1.0f),
	  Vector3f(-1.0f,-1.0f, 1.0f),
	  Vector3f(-1.0f, 1.0f, 1.0f),
	  Vector3f(-1.0f, 1.0f,-1.0f),

	  // right
	  Vector3f(1.0f,-1.0f, 1.0f),
	  Vector3f(1.0f,-1.0f,-1.0f),
	  Vector3f(1.0f, 1.0f,-1.0f),
	  Vector3f(1.0f,-1.0f, 1.0f),
	  Vector3f(1.0f, 1.0f,-1.0f),
	  Vector3f(1.0f, 1.0f, 1.0f),
   };

  m_quad.attribute<Vector2f>("a_texcoord") =
  {
	  //  U     V
	  // bottom
	  Vector2f(0.0f, 0.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(1.0f, 1.0f),
	  Vector2f(0.0f, 1.0f),

	  // top
	  Vector2f(0.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 1.0f),

	  // front
	  Vector2f(1.0f, 0.0f),
	  Vector2f(0.0f, 0.0f),
	  Vector2f(1.0f, 1.0f),
	  Vector2f(0.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 1.0f),

	  // back
	  Vector2f(0.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 1.0f),

	  // left
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(0.0f, 0.0f),
	  Vector2f(0.0f, 1.0f),
	  Vector2f(1.0f, 1.0f),
	  Vector2f(1.0f, 0.0f),

	  // right
	  Vector2f(1.0f, 1.0f),
	  Vector2f(1.0f, 0.0f),
	  Vector2f(0.0f, 0.0f),
	  Vector2f(1.0f, 1.0f),
	  Vector2f(0.0f, 0.0f),
	  Vector2f(0.0f, 1.0f)
   };

// FIXME ugly
  m_quad.uniform<float>("u_texture").m_tex.interpolation(GL_LINEAR);
  m_quad.uniform<float>("u_texture").m_tex.wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_quad.uniform<float>("u_texture").m_tex.load("wooden-crate.jpg"))
    return false;
// FIXME ugly

  m_quad.uniform<float>("u_scale") = 1.0f;
  float ratio = ((float) m_width) / ((float) m_height);
  m_quad.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_quad.uniform<Matrix44f>("u_model") = m_movable.transform();
  m_quad.uniform<Matrix44f>("u_view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  return true;
}

bool GLExample01::draw()
{
  LOGD("GLExample01::draw");
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  static float time = 0.0f;
  time += dt();
  m_movable.rotate(4.0f * cosf(time), Vector3f(0, 1, 0));
  m_quad.uniform<Matrix44f>("u_model") = m_movable.transform();

  // Filled cube
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  m_quad.uniform<Vector4f>("u_color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  m_quad.draw(GL_TRIANGLES, 0, 36);

  // Outlined cube
  glDisable(GL_POLYGON_OFFSET_FILL);
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
  m_quad.uniform<Vector4f>("u_color") = Vector4f(1, 0, 0, 1);
  m_quad.draw(GL_LINES, 0, 36);
  glDepthMask(GL_TRUE);

  return true;
}

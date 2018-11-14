#include "Example01.hpp"
#include <math.h>

void GLExample01::onWindowSizeChanged(const float width, const float height)
{
  float ratio = width / height;

  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

bool GLExample01::setup()
{
  LOGD("GLExample01::setup");

  // Enable the depth buffer
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glEnable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  vs.fromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/Example01.vertex");
  fs.fromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/Example01.fragment");
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // Model #1
  m_vao_quad = m_prog.createVAO("VAO_quad"); // FIXME m_prog.initVAO(&m_vao_quad); // ou alors m_prog.bind(m_vao_quad)
  //m_vao_quad->VBO<Vector3f>("a_position").m_container = // FIXME 
  m_prog.attribute<Vector3f>("a_position") =
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
          Vector3f(1.0f, 1.0f, 1.0f)
    };
  m_vao_quad->VBO<Vector2f>("a_texcoord").m_container =
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

  // Model #2
  m_vao_floor = m_prog.createVAO("VAO_floor");
  m_vao_floor->VBO<Vector3f>("a_position").m_container =
    {
          Vector3f(5, -0.5,  5), Vector3f(-5, -0.5,  5), Vector3f(-5, -0.5, -5),
          Vector3f(5, -0.5,  5), Vector3f(-5, -0.5, -5), Vector3f(5, -0.5, -5)
    };
  m_vao_floor->VBO<Vector2f>("a_texcoord").m_container =
    {
          Vector2f(0.0f, 0.0f), Vector2f(1.0f, 0.0f), Vector2f(0.0f, 1.0f),
          Vector2f(1.0f, 0.0f), Vector2f(1.0f, 1.0f), Vector2f(0.0f, 1.0f),
    };

  // Texture FIXME 1 texture par VAO
  m_prog.uniform<GLTexture2D>("u_texture").interpolation(GL_LINEAR);
  m_prog.uniform<GLTexture2D>("u_texture").wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("wooden-crate.jpg"))
    return false;

  // Uniforms
  m_prog.uniform<float>("u_scale") = 1.0f;
  float ratio = static_cast<float>(m_width) / (static_cast<float>(m_height) + 0.1f);
  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog.uniform<Matrix44f>("u_model") = m_movable.transform();
  m_prog.uniform<Matrix44f>("u_view") =
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
  float ct = cosf(time);

  m_movable.rotate(4.0f * ct, Vector3f(0, 1, 0));
  m_prog.uniform<Matrix44f>("u_model") = m_movable.transform();

  // Draw cube
  m_prog.bind(*m_vao_quad);
  m_prog.draw(GL_TRIANGLES, 0, 36);

  // Draw floor
  m_prog.bind(*m_vao_floor);
  m_prog.draw(GL_TRIANGLES, 0, 2);

  return true;
}

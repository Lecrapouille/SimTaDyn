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
  LOGD("*********************** GLExample01::setup");

  // Enable the depth buffer
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glDisable(GL_BLEND));
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
  m_prog.bind(m_vao_quad);
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
  m_prog.attribute<Vector2f>("a_texcoord") =
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
  m_prog.bind(m_vao_floor);
  m_prog.attribute<Vector3f>("a_position") =
    {
      Vector3f(5, -1.5,  5), Vector3f(-5, -1.5,  5), Vector3f(-5, -1.5, -5),
      Vector3f(5, -1.5,  5), Vector3f(-5, -1.5, -5), Vector3f(5, -1.5, -5)
    };
  m_prog.attribute<Vector3f>("a_position") *= Vector3f(2.0f, 1.0, 2.0f);
  m_prog.attribute<Vector2f>("a_texcoord") =
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
  m_prog.uniform<Matrix44f>("u_model") = m_movable1.transform();
  m_prog.uniform<Matrix44f>("u_view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // Debug
  LOGD("Instropection:");
  std::vector<std::string> vbos = m_vao_quad.VBONames();
  for (auto& it: vbos)
    {
      std::cout << "VAO has VBO named '" << it << "'" << std::endl;
    }

  // TODO Check if everything is ok (attrib/uniform are set, prog compiled ...)

  return true;
}

bool GLExample01::draw()
{
  LOGD("*********************** GLExample01::draw");
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  static float time = 0.0f;
  time += dt();
  float ct = cosf(time);

  // FIXME: a ameliorer
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("hazard.png"))
    return false;

  // Draw turning "pinkished" cube
  m_prog.bind(m_vao_quad);
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.8f, 0.2f, 0.8f, 0.8f);
  m_movable1.rotate(4.0f * ct, Vector3f(0, 1, 0));
  m_movable1.position(Vector3f(-1.0f, 0.0f, -1.0f));
  m_prog.uniform<Matrix44f>("u_model") = m_movable1.transform();
  m_prog.draw(GL_TRIANGLES, 0, 36);

  // Draw fixed "darkished" cube
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  m_movable2.reset();
  m_movable2.position(Vector3f(3.0f, 0.0f, 0.0f));
  m_prog.uniform<Matrix44f>("u_model") = m_movable2.transform();
  m_prog.bind(m_vao_quad);
  m_prog.draw(GL_TRIANGLES, 0, 36);

  // Draw floor
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load("wooden-crate.jpg"))
    return false;

  m_prog.bind(m_vao_floor);
  m_prog.uniform<Vector4f>("u_color") = Vector4f(0.2f, 0.2f, 0.2f, 0.2f);
  m_movable3.reset();
  m_movable3.position(Vector3f(0.0f, 0.0f, 0.0f));
  m_prog.uniform<Matrix44f>("u_model") = m_movable3.transform();
  m_prog.draw(GL_TRIANGLES, 0, 6);

  return true;
}

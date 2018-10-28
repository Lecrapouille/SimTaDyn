#include "Example02.hpp"
#include <math.h>

// https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/5.1.framebuffers/framebuffers.cpp

const float cubeVertices[] =
  {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

const float planeVertices[] =
  {
    // positions          // texture Coords
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
  };

// vertex attributes for a quad that fills the entire screen in
// Normalized Device Coordinates.
const float quadVertices[] =
  {
  // positions   // texCoords
  -1.0f,  1.0f,  0.0f, 1.0f,
  -1.0f, -1.0f,  0.0f, 0.0f,
   1.0f, -1.0f,  1.0f, 0.0f,

  -1.0f,  1.0f,  0.0f, 1.0f,
   1.0f, -1.0f,  1.0f, 0.0f,
   1.0f,  1.0f,  1.0f, 1.0f
};

bool GLExample02::setup()
{
  LOGD("GLExample02::setup");

  // Enable the depth buffer
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glDepthFunc(GL_LESS));
  glCheck(glEnable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  m_vs[FRMBFF].fromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/framebuffers.vs");
  m_fs[FRMBFF].fromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/framebuffers.fs");
  m_prog0.attachShaders(m_vs[FRMBFF], m_fs[FRMBFF]);

  std::cout << "Prog0 Shaders:" << std::endl;
  m_prog0.begin();
   for (auto& it: m_prog0.shaderNames())
    {
      std::cout << "    " << it << std::endl;
    }
   std::cout << "Prog0 Uniforms:" << std::endl;
   for (auto& it: m_prog0.uniformNames())
    {
      std::cout << "    " << it << std::endl;
    }
   std::cout << "Prog0 Attributes:" << std::endl;
  for (auto& it: m_prog0.attributeNames())
    {
      std::cout << "    " << it << std::endl;
    }

  // Camera
  m_prog0.uniform<Matrix44f>("projection") =
    matrix::perspective(maths::radians(50.0f), 1.0f, 0.1f, 10.0f);
  m_prog0.uniform<Matrix44f>("model") = m_movable.transform();
  m_prog0.uniform<Matrix44f>("view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // Texture
  m_prog0.uniform<GLTexture2D>("texture1").interpolation(GL_LINEAR);
  m_prog0.uniform<GLTexture2D>("texture1").wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_prog0.uniform<GLTexture2D>("texture1").load("wooden-crate.jpg"))
    return false;

  // Meshes
  auto pos = m_prog0.attribute<Vector3f>("aPos");
  pos = cubeVertices;
  pos.m_stride = 5 * sizeof (float);
  pos.m_offset = 0;

  auto tex = m_prog0.attribute<Vector2f>("aTexCoords");
  tex = cubeVertices;
  tex.m_stride = 5 * sizeof (float);
  tex.m_offset = 3 * sizeof (float);

#if 0
  m_vs[SCREEN].fromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/framebuffers_screen.vs");
  m_fs[SCREEN].fromFile("/home/qq/SimTaDyn/src/common/graphics/OpenGL/examples/shaders/framebuffers_screen.fs");
  m_prog1.attachShaders(m_vs[SCREEN], m_fs[SCREEN]);

  std::cout << std::endl << "Prog1 Shaders:" << std::endl;
  m_prog1.begin();
   for (auto& it: m_prog1.shaderNames())
    {
      std::cout << "    " << it << std::endl;
    }
   std::cout << "Prog1 Uniforms:" << std::endl;
   for (auto& it: m_prog1.uniformNames())
    {
      std::cout << "    " << it << std::endl;
    }
   std::cout << "Prog1 Attributes:" << std::endl;
  for (auto& it: m_prog1.attributeNames())
    {
      std::cout << "    " << it << std::endl;
    }
#endif

  return true;
}

void GLExample02::onWindowSizeChanged(const float width, const float height)
{
  float ratio = width / height;

  m_prog0.uniform<Matrix44f>("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
}

bool GLExample02::draw()
{
  LOGD("GLExample02::draw");
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  auto model = m_prog.uniform<Matrix44f>("model");

  // Cube 1
  m_prog0.bind(m_cubeVAO);
  m_movable.position(Vector3f(-1.0f, 0.0f, -1.0f));
  model = m_movable.transform();
  m_prog0.draw(GL_TRIANGLES, 0, 36);

  // Cube 2
  m_movable.position(Vector3f(2.0f, 0.0f, 0.0f));
  model = m_movable.transform();
  m_prog0.draw(GL_TRIANGLES, 0, 36);

  // Floor
  m_prog0.bind(m_floorVAO);
  m_movable.position(Vector3f(1.0f, 1.0f, 1.0f));
  model = m_movable.transform();
  m_prog0.draw(GL_TRIANGLES, 0, 36);

  return true;
}

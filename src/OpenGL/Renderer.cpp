#include "Renderer.hpp"
#include "SimTaDynContext.hpp"
#include "GraphMemory.hpp"

// https://github.com/progschj/OpenGL-Examples/blob/master/08map_buffer.cpp

static container<Vector3D> con;
const Vector3D* vec;

GLRenderer::~GLRenderer()
{
  glCheck(glDeleteVertexArrays(1, &m_vao));
  glCheck(glDeleteBuffers(1, &m_vbo[0]));
}

bool GLRenderer::setupGraphics()
{
  bool res = true;

  // FIXME: this is an awful hack but this is to be sure to flush
  // OpenGL errors before using this function on real OpenGL routines
  // else a fake error is returned on the first OpenGL routines while
  // valid.
  glGetError();

  // FIXME temporary
  con.append(Vector3D(0.0f,  0.5f));
  con.append(Vector3D(0.5f, -0.366f));
  con.append(Vector3D(-0.5f, -0.366f));
  con.append(Vector3D(0.2f,  0.7f));
  con.append(Vector3D(0.7f, -0.166f));
  con.append(Vector3D(-0.3f, -0.166f));

  // Configure OpenGL
  activateTransparency();
  activateDepthBuffer();

  glCheck(glClearDepth(1.0f));
  glCheck(glEnable(GL_LINE_SMOOTH));
  glCheck(glEnable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCheck(glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE));
  glCheck(glLineWidth(10.0f));
  glCheck(glPointSize(10.0f));
  glCheck(glEnable(GL_ALPHA_TEST));

  // Shader program
  GLuint program = m_shader.createShaderProgram(SIMTADYN().data_path("node.vertex"),
                                                SIMTADYN().data_path("node.fragment"));
  if (0 != program)
    {
      // Create Vertex Array Object
      glCheck(glGenVertexArrays(1, &m_vao));
      glCheck(glBindVertexArray(m_vao));

      // Create a Vertex Buffer Object and copy the vertex data to it
      glCheck(glGenBuffers(1, &m_vbo[0]));
      glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]));
      glCheck(glBufferData(GL_ARRAY_BUFFER, con.poolSizeAllocation() * sizeof (Vector3D), con.slot(0), GL_DYNAMIC_DRAW));

      m_shader.begin();

      //
      m_mvpAttrib = glCheck(glGetUniformLocation(program, "mvp"));
      if (-1 == m_mvpAttrib)
        {
          std::cerr << "[FAILED] getting shader location for mvp" << std::endl;
        }

      // Specify the layout of the vertex data
      m_posAttrib = glCheck(glGetAttribLocation(program, "position"));
      if (-1 == m_posAttrib)
        {
          std::cerr << "[FAILED] getting shader location for position" << std::endl;
        }
      glEnableVertexAttribArray(m_posAttrib);
      glCheck(glVertexAttribPointer(m_posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr));

      // Get the location of the color uniform
      // ...

      //m_shader.end();

      // Models, textures and fonts
      //m_fonts[0].open(SimTaDynFont::MappedTexture, "../data/Font.tga");
      //m_fonts[0].open(SimTaDynFont::SystemFont, "fixed");

      // Camera
      m_default_camera.lookAt(static_cast<float32_t>(screenWidth()) / 2.0f,
                              static_cast<float32_t>(screenHeight()) / 2.0f,
                              static_cast<float32_t>(screenWidth()),
                              static_cast<float32_t>(screenHeight()));
      m_current_camera = m_default_camera;
    }
  else
    {
      res = false;
      std::cerr << "[FAILED] Init OpenGL renderer" << std::endl;
    }
  return res;
}

static void compute_mvp(float *res,
                        float phi,
                        float theta,
                        float psi)
{
  float x       {phi * ((float)G_PI / 180.f)};
  float y       {theta * ((float)G_PI / 180.f)};
  float z       {psi * ((float)G_PI / 180.f)};
  float c1      {cosf (x)};
  float s1      {sinf (x)};
  float c2      {cosf (y)};
  float s2      {sinf (y)};
  float c3      {cosf (z)};
  float s3      {sinf (z)};
  float c3c2    {c3 * c2};
  float s3c1    {s3 * c1};
  float c3s2s1  {c3 * s2 * s1};
  float s3s1    {s3 * s1};
  float c3s2c1  {c3 * s2 * c1};
  float s3c2    {s3 * c2};
  float c3c1    {c3 * c1};
  float s3s2s1  {s3 * s2 * s1};
  float c3s1    {c3 * s1};
  float s3s2c1  {s3 * s2 * c1};
  float c2s1    {c2 * s1};
  float c2c1    {c2 * c1};

  /* apply all three rotations using the three matrices:
   *
   * ⎡  c3 s3 0 ⎤ ⎡ c2  0 -s2 ⎤ ⎡ 1   0  0 ⎤
   * ⎢ -s3 c3 0 ⎥ ⎢  0  1   0 ⎥ ⎢ 0  c1 s1 ⎥
   * ⎣   0  0 1 ⎦ ⎣ s2  0  c2 ⎦ ⎣ 0 -s1 c1 ⎦
   */
  res[0] = c3c2;  res[4] = s3c1 + c3s2s1;  res[8] = s3s1 - c3s2c1; res[12] = 0.f;
  res[1] = -s3c2; res[5] = c3c1 - s3s2s1;  res[9] = c3s1 + s3s2c1; res[13] = 0.f;
  res[2] = s2;    res[6] = -c2s1;         res[10] = c2c1;          res[14] = 0.f;
  res[3] = 0.f;   res[7] = 0.f;           res[11] = 0.f;           res[15] = 1.f;
}

void GLRenderer::draw(/*SimTaDynGraph_t const& graph*/) const
{
  static float m_matrix_mvp[16];

  // FIXME ajouter un flag pour eviter de faire des calculs
  // if (mvp_need_refresh)
  // {
  // mvp_need_refresh = false;
  compute_mvp(m_matrix_mvp,
              m_RotationAngles[X_AXIS],
              m_RotationAngles[Y_AXIS],
              m_RotationAngles[Z_AXIS]);
  // }

  m_shader.begin();
  glCheck(glUniformMatrix4fv(m_mvpAttrib, 1, GL_FALSE, &m_matrix_mvp[0]));

  //glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_vao));
  //glCheck(glEnableVertexAttribArray(0));

  // if () {
  glCheck(glDrawArrays(GL_POINTS, 0, con.poolSizeAllocation()));
  // }

  //glCheck(glDisableVertexAttribArray(0));
  glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
  m_shader.end();
}

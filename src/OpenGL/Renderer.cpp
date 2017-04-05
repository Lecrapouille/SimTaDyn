#include "Renderer.hpp"

// FIXME Temporary example
static GLVertexBuffer<Vector3D, 8U> pos;
static GLVertexBuffer<Color, 8U> col;

static void compute_mvp(float32_t *res,
                        float32_t phi,
                        float32_t theta,
                        float32_t psi)
{
  float32_t x       {phi * ((float32_t) 3.1415f / 180.f)};
  float32_t y       {theta * ((float32_t) 3.1415f / 180.f)};
  float32_t z       {psi * ((float32_t) 3.1415f / 180.f)};
  float32_t c1      {cosf (x)};
  float32_t s1      {sinf (x)};
  float32_t c2      {cosf (y)};
  float32_t s2      {sinf (y)};
  float32_t c3      {cosf (z)};
  float32_t s3      {sinf (z)};
  float32_t c3c2    {c3 * c2};
  float32_t s3c1    {s3 * c1};
  float32_t c3s2s1  {c3 * s2 * s1};
  float32_t s3s1    {s3 * s1};
  float32_t c3s2c1  {c3 * s2 * c1};
  float32_t s3c2    {s3 * c2};
  float32_t c3c1    {c3 * c1};
  float32_t s3s2s1  {s3 * s2 * s1};
  float32_t c3s1    {c3 * s1};
  float32_t s3s2c1  {s3 * s2 * c1};
  float32_t c2s1    {c2 * s1};
  float32_t c2c1    {c2 * c1};

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

GLRenderer::~GLRenderer()
{
  glDeleteVertexArrays(1, &m_vao);
}

bool GLRenderer::setupGraphics()
{
  // This is an awful hack but this is to be sure to flush OpenGL
  // errors before using this function on real OpenGL routines else a
  // fake error is returned on the first OpenGL routines while valid.
  glGetError();

  // Configure OpenGL
  activateTransparency();
  activateDepthBuffer();
  glCheck(glClearDepth(1.0f));
  glCheck(glEnable(GL_LINE_SMOOTH));
  glCheck(glEnable(GL_BLEND));
  glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  glCheck(glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE));
  glCheck(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));

  // Models, textures and fonts
  //m_fonts[0].open(SimTaDynFont::MappedTexture, "../data/Font.tga");
  //m_fonts[0].open(SimTaDynFont::SystemFont, "fixed");

  // Camera
  m_default_camera.lookAt(static_cast<float32_t>(screenWidth()) / 2.0f,
                          static_cast<float32_t>(screenHeight()) / 2.0f,
                          static_cast<float32_t>(screenWidth()),
                          static_cast<float32_t>(screenHeight()));
  m_current_camera = m_default_camera;

  pos.append(Vector3D(0.0f,  0.5f));
  pos.append(Vector3D(0.5f, -0.366f));
  pos.append(Vector3D(-0.5f, -0.366f));
  pos.append(Vector3D(0.2f,  0.7f));
  pos.append(Vector3D(0.7f, -0.166f));
  pos.append(Vector3D(-0.3f, -0.166f));

  col.append(Color(1.0f, 0.0f, 0.0f));
  col.append(Color(0.0f, 1.0f, 0.0f));
  col.append(Color(0.0f, 0.0f, 1.0f));
  col.append(Color(1.0f, 0.5f, 0.0f));
  col.append(Color(0.0f, 1.0f, 0.5f));
  col.append(Color(0.5f, 0.0f, 1.0f));

  GLuint program = m_shader.load(Config::instance().data_path("node.vertex"),
                                 Config::instance().data_path("node.fragment"));
  if (0U == program)
    return false;

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  m_shader.begin();
  m_posAttrib = m_shader.locate("a_position");
  m_colAttrib = m_shader.locate("a_color");
  m_timeAttrib = m_shader.locate("u_time");
  m_mvpAttrib = m_shader.locate("u_mvp");

  glCheck(glUniform1f(m_timeAttrib, 1.0f));
  m_shader.end();

  if (-1 == m_posAttrib)
    return false;
  if (-1 == m_colAttrib)
    return false;
  if (-1 == m_timeAttrib)
    return false;
  if (-1 == m_mvpAttrib)
    return false;
  return true;
}

void GLRenderer::draw()
{
  // FIXME: a fusionner avec la camera
  static float32_t m_matrix_mvp[16];

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
  {
    // if (mvp_need_refresh)
    glCheck(glUniformMatrix4fv(m_mvpAttrib, 1, GL_FALSE, &m_matrix_mvp[0]));

    // Color
    glCheck(glEnableVertexAttribArray(m_colAttrib));
    col.m_blocks[0]->begin();
    glCheck(glVertexAttribPointer(m_colAttrib, 4, GL_FLOAT, GL_FALSE, 0, nullptr));
    col.m_blocks[0]->end();
    glCheck(glDisableVertexAttribArray(0));

    // Position
    glCheck(glEnableVertexAttribArray(m_posAttrib));
    pos.m_blocks[0]->begin();
    glCheck(glVertexAttribPointer(m_posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    glCheck(glDrawArrays(GL_POINTS, 0, 6));
    pos.m_blocks[0]->end();
    glCheck(glDisableVertexAttribArray(0));
  }
  m_shader.end();
}

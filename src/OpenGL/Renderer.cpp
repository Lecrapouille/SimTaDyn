#include "Renderer.hpp"

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

  GLuint program = m_shader.load(Config::instance().data_path("shaders/node.vertex"),
                                 Config::instance().data_path("shaders/node.fragment"));
  if (0U == program)
    return false;

  m_vao.begin();
  m_shader.begin();

  m_posAttrib.setup(m_shader, "a_position", 3, GL_FLOAT);
  m_colAttrib.setup(m_shader, "a_color", 4, GL_FLOAT);
  m_timeAttrib.setup(m_shader, "u_time");
  m_mvpAttrib.setup(m_shader, "u_mvp");

  glCheck(glUniform1f(m_timeAttrib, 1.0f));
  m_shader.end();
  return true;
}

void GLRenderer::begin()
{
  clearScreen();
  m_shader.begin();

  // FIXME ajouter un flag pour eviter de faire des calculs
  // if (mvp_need_refresh)
  // {
  // mvp_need_refresh = false;
  compute_mvp(m_matrix_mvp,
              m_RotationAngles[X_AXIS],
              m_RotationAngles[Y_AXIS],
              m_RotationAngles[Z_AXIS]);
  glCheck(glUniformMatrix4fv(m_mvpAttrib, 1, GL_FALSE, &m_matrix_mvp[0]));
  // }
}

void GLRenderer::draw()
{
  LOGI("GLRenderer::draw()");
  // FIXME: vider la queue
  if (nullptr != m_model)
    {
      m_model->drawnBy(*this);
    }
}

void GLRenderer::end()
{
  m_shader.end();
  glFlush();
}

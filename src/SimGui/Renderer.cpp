#include "Renderer.hpp"
#include "SimTaDynContext.hpp"

static const GLfloat vertex_data[] =
  {
     0.0f,  0.5f,   0.f, 1.f,
     0.5f, -0.366f, 0.f, 1.f,
    -0.5f, -0.366f, 0.f, 1.f,
  };

void Renderer::start()
{
  // Configure OpenGL
  glClearDepth(1.0f);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glLineWidth(1.0f);
  glEnable(GL_ALPHA_TEST);

  // Camera
  m_default_camera.lookAt(static_cast<float32_t>(screenWidth()) / 2.0f,
                          static_cast<float32_t>(screenHeight()) / 2.0f,
                          static_cast<float32_t>(screenWidth()),
                          static_cast<float32_t>(screenHeight()));
  m_current_camera = m_default_camera;

  // Models, textures and fonts
  //font_list_[0].open(SimTaDynFont::MappedTexture, "../data/Font.tga");
  //font_list_[0].open(SimTaDynFont::SystemFont, "fixed");

  // Shaders, vertex array objects
  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);
  glGenBuffers(1, &m_Buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_Program = simGL::createShaderProgram(SIMTADYN().data_path("vertex.glsl"),
                                         SIMTADYN().data_path("fragment.glsl"));
  if (0 != m_Program)
    {
      m_Mvp = glGetUniformLocation(m_Program, "mvp");
    }
}

void Renderer::end()
{
  // Delete buffers and program
  glDeleteBuffers(1, &m_Vao);
  glDeleteProgram(m_Program);
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

void Renderer::render() const
{
  float mvp[16];
  enum { X_AXIS, Y_AXIS, Z_AXIS };

  clearScreen();

  compute_mvp(mvp,
              m_RotationAngles[X_AXIS],
              m_RotationAngles[Y_AXIS],
              m_RotationAngles[Z_AXIS]);

  glUseProgram(m_Program);
  glUniformMatrix4fv(m_Mvp, 1, GL_FALSE, &mvp[0]);
  glBindBuffer(GL_ARRAY_BUFFER, m_Vao);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);

  glFlush();
}

#include "Example03.hpp"
#include <glm/glm.hpp>

static GLuint gVBO = 0;
static GLfloat vertexData[] = {
        //  X     Y     Z       U     V
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };
static GLuint TextureID = 0;

bool GLExample03::setup()
{
  LOGI("GLExample03::setup()");

  if (0U == m_shader.load("Example03.vertex", "Example03.fragment"))
    return false;

  if (false == m_texture.load("wooden-crate.jpg"))
    return false;
  m_texture.interpolation(GL_LINEAR);
  m_texture.wrapping(GL_CLAMP_TO_EDGE);

  m_vao.start();

  glCheck(glGenBuffers(1, &gVBO));
  glCheck(glBindBuffer(GL_ARRAY_BUFFER, gVBO));
  glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW));

  glCheck(glEnableVertexAttribArray(m_shader.locateAttrib("vert")));
  glCheck(glVertexAttribPointer(m_shader.locateAttrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL));

  // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
  glCheck(glEnableVertexAttribArray(m_shader.locateAttrib("vertTexCoord")));
  glCheck(glVertexAttribPointer(m_shader.locateAttrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat))));

GLuint ModelID  = glCheck(glGetUniformLocation(m_shader, "model"));
glUniform4f(ModelID, glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0,1,0)));

  //glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
  m_vao.stop();
  return true;
}

bool GLExample03::draw()
{
  LOGI("GLExample03::draw()");

  // clear everything
  glCheck(glClearColor(0.0f, 0.0f, 0.4f, 0.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
  m_shader.start();
  m_vao.start();

  TextureID  = glCheck(glGetUniformLocation(m_shader, "tex"));
  glCheck(glUniform1i(TextureID, 0)); //set to 0 because the texture is bound to GL_TEXTURE0

  glCheck(glActiveTexture(GL_TEXTURE0));
  m_texture.start();
  //glCheck(glBindTexture(GL_TEXTURE_2D, _object);
  glCheck(glDrawArrays(GL_TRIANGLES, 0, 6*2*3));  

  m_texture.stop();
  //glBindTexture(GL_TEXTURE_2D, 0);
  m_vao.stop();
  m_shader.stop();

  return true;
}

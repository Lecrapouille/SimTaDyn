//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Renderer.hpp"
#include "MapEditor.hpp"

#  ifndef ARRAY_SIZE
#    define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#  endif

// Vertices for drawing a cube
static GLfloat vertexData[] = {
  //  X     Y     Z
  // bottom
  -1.0f,-1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f, 1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f,-1.0f, 1.0f,
  -1.0f,-1.0f, 1.0f,

  // top
  -1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,

  // front
  -1.0f,-1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  1.0f,-1.0f, 1.0f,
  1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,

  // back
  -1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f,-1.0f,-1.0f,
  -1.0f, 1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,

  // left
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,
  -1.0f,-1.0f,-1.0f,
  -1.0f,-1.0f, 1.0f,
  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f,-1.0f,

  // right
  1.0f,-1.0f, 1.0f,
  1.0f,-1.0f,-1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f,-1.0f, 1.0f,
  1.0f, 1.0f,-1.0f,
  1.0f, 1.0f, 1.0f,
};

// Teaxture position on each vertices
static GLfloat textureData[] = {
  //  U     V
  // bottom
  0.0f, 0.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 1.0f,

  // top
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,

  // front
  1.0f, 0.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,

  // back
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 0.0f,
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,

  // left
  0.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f,
  1.0f, 0.0f,

  // right
  1.0f, 1.0f,
  1.0f, 0.0f,
  0.0f, 0.0f,
  1.0f, 1.0f,
  0.0f, 0.0f,
  0.0f, 1.0f
};

// FIXME: m_shader::setUniform
void GLRenderer::setUniform(const char *name, Matrix44f const &mat)
{
  GLint id = glCheck(glGetUniformLocation(m_shader, name));
  glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
}

// This function can throw OpenGL exception but caught by the class DrawingArea
bool GLRenderer::setupGraphics()
{
  // This is an awful hack but this is to be sure to flush OpenGL
  // errors before using this function on real OpenGL routines else a
  // fake error is returned on the first OpenGL routines while valid.
  glGetError();

  // Configure OpenGL states
  activateTransparency();
  activateDepthBuffer();

  // Copy arrays inside the VBO class
  m_pos.add(vertexData, ARRAY_SIZE(vertexData));
  m_tex.add(textureData, ARRAY_SIZE(textureData));

  // Compile a shader program.
  if (0U == m_shader.load(PathManager::instance().expand("shaders/node.vertex"),
                          PathManager::instance().expand("shaders/node.fragment")))
    return false;

  // Configure the texture
  m_texture.interpolation(GL_LINEAR);
  m_texture.wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_texture.load(PathManager::instance().expand("textures/wooden-crate.jpg")))
    return false;

  // Tell to OpenGL how to manage VBO values. This fixes the size
  // of the VBO container to its current capacity (ie. now the VBO
  // size no longer be larger): the GPU have allocated static memory.
  m_pos.setup(m_shader, 3, GL_FLOAT);
  m_tex.setup(m_shader, 2, GL_FLOAT);

  m_shader.start();
  {
    // Bind VBOs to the VAO. It's now enough for drawing primitives.
    m_vao.start();
    {
      m_pos.start();
      m_tex.start();
    }
    m_vao.stop();
  }
  m_shader.stop();

  // Camera
  /*m_default_camera.lookAt(static_cast<float>(screenWidth()) / 2.0f,
    static_cast<float>(screenHeight()) / 2.0f,
    static_cast<float>(screenWidth()),
    static_cast<float>(screenHeight()));
    m_current_camera = m_default_camera;*/

  // Signals
  MapEditor::instance().loaded_success.connect(
     sigc::mem_fun(*this, &GLRenderer::drawMap));
  MapEditor::instance().m_current_map.signal_changed.connect(
     sigc::mem_fun(*this, &GLRenderer::drawMap));

  return true;
}

void GLRenderer::clearScreen() const
{
  glCheck(glClearColor(m_background_color.r, m_background_color.g,
                       m_background_color.b, m_background_color.a));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GLRenderer::draw()
{
  clearScreen();
  m_shader.start();
  {
    m_vao.start();
    {
      m_pos.start();

      // Model matrix transformation
      setUniform("model", m_movable.transform());

      // Projection matrix transformation
      float ratio = ((float) screenWidth()) / ((float) screenHeight());
      Matrix44f projection = matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
      setUniform("projection", projection);

      // View matrix transformation
      Matrix44f camera = matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));
      setUniform("camera", camera);

      //set to 0 because the texture is bound to GL_TEXTURE0
      GLint TextureID = glCheck(glGetUniformLocation(m_shader, "tex"));
      glCheck(glUniform1i(TextureID, 0));
      glCheck(glActiveTexture(GL_TEXTURE0));
      m_texture.start();


      glCheck(glDrawArrays(GL_TRIANGLES, 0, 6*2*3));

      m_texture.stop();
      m_pos.stop();
    }
    m_vao.stop();
  }
  m_shader.stop();
}

void GLRenderer::drawMap(SimTaDynMapPtr map)
{
  // TODO ---> DrawingArea::onNotify(){>attachModel(*map);} mais PendingData le fait deja
  std::cout << "GLRenderer::drawMap() " << map->m_name << std::endl;
}

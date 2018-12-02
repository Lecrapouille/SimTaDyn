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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Renderer.hpp"
#include "MapEditor.hpp"

//------------------------------------------------------------------
bool GLRenderer::setupGraphics()
{
  LOGD("GLRenderer::setupGraphics()");
  // This is an awful hack but this is to be sure to flush OpenGL
  // errors before using this function on real OpenGL routines else a
  // fake error is returned on the first OpenGL routines while valid.
  glGetError();

  // Configure OpenGL states
  activateTransparency();
  activateDepthBuffer();

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile(PathManager::instance().expand("shaders/Example01.vertex"));
  fs.fromFile(PathManager::instance().expand("shaders/Example01.fragment"));

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.error() << "'" << std::endl;
      return false;
    }

  // Binding empty VAO to OpenGL program will make it be populated
  // with all VBOs needed.
  m_prog.bind(m_vao);

  // Now we have to fill VBOs with data: here vertices. Because in
  // vertex shader a_position is vect3 we have to cast to Vector3f.
  m_prog.attribute<Vector3f>("a_position") =
    {
      // bottom
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f,-1.0f, 1.0f),

      // top
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f, 1.0f),

      // front
      Vector3f(-1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),
      Vector3f( 1.0f,-1.0f, 1.0f),
      Vector3f( 1.0f, 1.0f, 1.0f),
      Vector3f(-1.0f, 1.0f, 1.0f),

      // back
      Vector3f(-1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f( 1.0f,-1.0f,-1.0f),
      Vector3f(-1.0f, 1.0f,-1.0f),
      Vector3f( 1.0f, 1.0f,-1.0f),

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

  // Texture
  m_prog.uniform<GLTexture2D>("u_texture").interpolation(GL_LINEAR);
  m_prog.uniform<GLTexture2D>("u_texture").wrapping(GL_CLAMP_TO_EDGE);
  if (false == m_prog.uniform<GLTexture2D>("u_texture").load(PathManager::instance().expand("textures/wooden-crate.jpg")))
    return false;

  // Uniforms
  m_prog.uniform<float>("u_scale") = 1.0f;
  m_prog.uniform<Vector4f>("u_color") = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

  float ratio = static_cast<float>(screenWidth()) / static_cast<float>(screenHeight());
  m_prog.uniform<Matrix44f>("u_projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog.uniform<Matrix44f>("u_model") = m_movable.transform();
  m_prog.uniform<Matrix44f>("u_view") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  // Signals
  MapEditor::instance().loaded_success.connect(
     sigc::mem_fun(*this, &GLRenderer::drawMap));
  MapEditor::instance().m_current_map.signal_changed.connect(
     sigc::mem_fun(*this, &GLRenderer::drawMap));

  return true;
}

//------------------------------------------------------------------
void GLRenderer::clearScreen() const
{
  glCheck(glClearColor(m_bg_color.r, m_bg_color.g,
                       m_bg_color.b, m_bg_color.a));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

//------------------------------------------------------------------
void GLRenderer::drawMap(SimTaDynMapPtr /*map*/)
{
}

//------------------------------------------------------------------
void GLRenderer::draw2D()
{
  LOGD("Draw2D");
  m_prog.bind(m_vao);
  m_prog.draw(GL_TRIANGLES, 0, 36);
}

//------------------------------------------------------------------
void GLRenderer::draw3D()
{
  LOGD("Draw3D");
  m_prog.bind(m_vao);
  m_prog.draw(GL_TRIANGLES, 0, 36);
}

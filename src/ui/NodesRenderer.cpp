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

#include "NodesRenderer.hpp"
#include "PathManager.hpp"

NodesRenderer::NodesRenderer()
  : m_prog("NodesRenderer")
{
}

bool NodesRenderer::setupGraphics(float const ratio)
{
  GLVertexShader     vs;
  GLFragmentShader   fs;

  // Load from ASCII file the vertex sahder (vs) as well the fragment shader
  vs.fromFile(PathManager::instance().expand("shaders/node.vertex"));
  fs.fromFile(PathManager::instance().expand("shaders/node.fragment"));

  // Compile shader as OpenGL program. This one will instanciate all OpenGL objects for you.
  if (!m_prog.attachShaders(vs, fs).compile())
    {
      std::cerr << "failed compiling OpenGL program. Reason was '"
                << m_prog.getError() << "'" << std::endl;
      return false;
    }

  // Uniforms
  // m_prog.uniform<float>("scale") = 1.0f;
  //m_prog.uniform<Vector3f>("color") = Vector3f(1.0f, 1.0f, 1.0f);

  m_prog.matrix44f("projection") =
    matrix::perspective(maths::radians(50.0f), ratio, 0.1f, 10.0f);
  m_prog.matrix44f("model") = m_movable.transform();
  m_prog.matrix44f("camera") =
    matrix::lookAt(Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0));

  return true;
}

//------------------------------------------------------------------
//! Draw recursively a node from a scene graph
//------------------------------------------------------------------
void NodesRenderer::draw2d(GLVAO& vao, Matrix44f const& transform)
{
  LOGD("NodesRenderer::draw");
  m_prog.matrix44f("model") = transform;
  m_prog.draw(vao, Mode::POINTS);
}

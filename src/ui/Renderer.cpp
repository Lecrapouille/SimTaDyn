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

  float ratio = static_cast<float>(screenWidth()) / static_cast<float>(screenHeight());
  if (!m_nodes_renderer.setupGraphics(ratio))
    return false;

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
// Called by
void GLRenderer::drawSceneNode(SimTaDynSheet& sheet, Matrix44f const& transform)
{
  LOGD("GLRenderer::drawSceneNode");
  if (GLRenderer::Mode2D == GLRenderer::renderMode())
    {
      m_nodes_renderer.draw2d(sheet.vaoNodes(), transform);
      //m_arcs_renderer.drawArcs(sheet.vaoArcs(), transform);
      //m_zones_renderer.drawZones(sheet.vaoZones(), transform);
    }
  else
    {
      LOGW("Draw Map in 3D not yet implemented");
    }
}

//------------------------------------------------------------------
void GLRenderer::draw()
{
  //FIXME MapEditor::instance().drawCurrentMap(/* *this */);
}

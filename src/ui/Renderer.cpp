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
//#include "MapEditor.hpp"

//------------------------------------------------------------------
GLRenderer::GLRenderer()
  : m_bg_color(0.0f, 0.0f, 0.2f, 1.0f),
    m_mode(GLRenderer::Mode2D)
{
}

//------------------------------------------------------------------
bool GLRenderer::setupGraphics()
{
  LOGD("GLRenderer::setupGraphics()");
  // This is an awful hack but this is to be sure to flush OpenGL
  // errors before using this function on real OpenGL routines else a
  // fake error is returned on the first OpenGL routines while valid.
  glGetError();

  m_initial_camera.lookAt(static_cast<float>(screenWidth()) / 2.0f,
                          static_cast<float>(screenHeight()) / 2.0f,
                          static_cast<float>(screenWidth()),
                          static_cast<float>(screenHeight())),
  m_current_camera = m_initial_camera;

  // Configure OpenGL states
  activateTransparency();
  activateDepthBuffer();

  float ratio = static_cast<float>(screenWidth()) / static_cast<float>(screenHeight());
  if (!m_nodes_renderer.setupGraphics(ratio))
    return false;
  //if (!m_arcs_renderer.setupGraphics(ratio))
  //   return false;
  //if (!m_zones_renderer.setupGraphics(ratio))
  //  return false;

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
void GLRenderer::applyViewport(Camera2D& camera)
{
  const float width  = static_cast<float>(screenWidth());
  const float height = static_cast<float>(screenHeight());

  glCheck(glViewport(static_cast<int>(0.5f + width  * camera.m_display_at_x),
                     static_cast<int>(0.5f + height * camera.m_display_at_y),
                     static_cast<int>(0.5f + width  * camera.m_display_at_width),
                     static_cast<int>(0.5f + height * camera.m_display_at_height)));

  glCheck(glMatrixMode(GL_PROJECTION));
  //glCheck(glLoadMatrixf(camera.getTransform()));
  glCheck(glLoadIdentity());
  glCheck(glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f));
  glCheck(glMatrixMode(GL_MODELVIEW));
}

//------------------------------------------------------------------
void GLRenderer::applyViewport()
{
  applyViewport(m_current_camera);
}

//------------------------------------------------------------------
void GLRenderer::selectCamera2D(Camera2D const& camera)
{
  m_current_camera = camera;
}

//------------------------------------------------------------------
Camera2D& GLRenderer::getCamera2D()
{
  return m_current_camera;
}

//------------------------------------------------------------------
void GLRenderer::restoreCamera2D()
{
  m_current_camera = m_initial_camera;
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
void GLRenderer::moveCameraCommand(CameraDirection const direction)
{
  Camera2D& camera = getCamera2D();
  switch (direction)
    {
    case CameraDirection::Up:
      camera.moveOffset(0.0f, -10.0f);
      break;
    case CameraDirection::Down:
      camera.moveOffset(0.0f, 10.0f);
      break;
    case CameraDirection::Left:
      camera.moveOffset(-10.0f, 0.0f);
      break;
    case CameraDirection::Right:
      camera.moveOffset(10.0f, 0.0f);
      break;
    case CameraDirection::Forward:
      camera.zoomOffset(0.01f);
      break;
    case CameraDirection::Backward:
      camera.zoomOffset(-0.01f);
      break;
    }
}

//------------------------------------------------------------------
void GLRenderer::draw()
{
  m_signal_draw.emit();
}

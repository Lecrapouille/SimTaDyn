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

#ifndef RENDERER_HPP_
#  define RENDERER_HPP_

#  include "Color.hpp"
#  include "OpenGL.hpp"
//#  include "Camera2D.hpp"
#  include "Movable.tpp"
#  include "SimTaDynMap.hpp"


// *************************************************************************************************
//! \brief OpenGL renderer
// *************************************************************************************************
class GLRenderer
{
public:

  // FIXME: utiliser le pattern State pour empiler des GLRenderer
  enum RenderMode { Mode2D, Mode3D };

  //------------------------------------------------------------------
  //! \brief Empty constructor. It does not setup graphics because the
  //! OpenGL can be not yet initialized.
  //------------------------------------------------------------------
  GLRenderer()
    : m_vao("VAO"),
      m_prog("prog"),
      m_bg_color(0.0f, 0.0f, 0.4f, 1.0f),
      m_mode(GLRenderer::Mode3D)
  {
  }

  //------------------------------------------------------------------
  //! \brief Destructor. OpenGL objects are automaticaly released from
  //! GPU.
  //------------------------------------------------------------------
  virtual ~GLRenderer()
  {
  }

  //------------------------------------------------------------------
  //! \brief Set the background color. This color is used for clearing
  //! the screen.
  //------------------------------------------------------------------
  inline void backgroundColor(const Color& color)
  {
    m_bg_color = color;
  }

  //------------------------------------------------------------------
  //! \brief Get the background color. This color is used for clearing
  //! the screen.
  //------------------------------------------------------------------
  inline Color backgroundColor() const
  {
    return m_bg_color;
  }

  //------------------------------------------------------------------
  //! \brief Change the renderer mode.
  //------------------------------------------------------------------
  inline void renderMode(const RenderMode mode)
  {
    m_mode = mode;
  }

  //------------------------------------------------------------------
  //! \brief Get the current renderer mode.
  //------------------------------------------------------------------
  inline RenderMode renderMode() const
  {
    return m_mode;
  }

  //------------------------------------------------------------------
  //! \brief Get the number of pixels of the viewport. This will be
  //! done by GTK in derived class.
  //------------------------------------------------------------------
  virtual uint32_t screenWidth() const = 0;

  //------------------------------------------------------------------
  //! \brief Get the number of pixels of the viewport. This will be
  //! done by GTK in derived class.
  //------------------------------------------------------------------
  virtual uint32_t screenHeight() const = 0;

  void drawMap(SimTaDynMapPtr map);

private:

  //------------------------------------------------------------------
  //! \brief Activate OpenGL GL_BLEND. This will be done by GTK in
  //! derived class.
  //------------------------------------------------------------------
  virtual void activateTransparency() = 0;

  //------------------------------------------------------------------
  //! \brief Activate OpenGL GL_DEPTH_TEST. This will be done by GTK in
  //! derived class.
  //------------------------------------------------------------------
  virtual void activateDepthBuffer() = 0;

protected:

  //------------------------------------------------------------------
  //! \brief Same role than a constructor but here we are sure than
  //! and OpenGL context is created before.
  //! \throw OpenGLException if OpenGL Wrapper detects a failure.
  //------------------------------------------------------------------
  bool setupGraphics();

  //------------------------------------------------------------------
  //! \brief Clear the OpenGL scene before painting it.
  //------------------------------------------------------------------
  void clearScreen() const;

  //------------------------------------------------------------------
  //! \brief Draw the scene. This method is called by the DrawingArea
  //! class triggered by the GTK event onRender().
  //! \throw OpenGLException if OpenGL Wrapper detects a failure.
  //------------------------------------------------------------------
  void draw2D();
  void draw3D();

  //------------------------------------------------------------------
  //! \brief Set OpenGL core version needed by GTK+.
  //------------------------------------------------------------------
  virtual void setCoreVersion() = 0;

private:

  GLVertexShader     vs;
  GLFragmentShader   fs;
  GLVAO              m_vao;
  GLProgram          m_prog;
  Movable<float, 3U> m_movable;
  Color              m_bg_color;
  RenderMode         m_mode;
};

#endif /* RENDERER_HPP_ */

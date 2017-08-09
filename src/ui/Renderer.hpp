//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
#  include "GLVertexArray.hpp"
#  include "GLVertexBuffer.hpp"
//#  include "Camera2D.hpp"
#  include "GLTextures.hpp"
#  include "Movable.tpp"

/*
 * OpenGL renderer.
 */
class GLRenderer
{
public:

  // FIXME: utiliser le pattern State pour empiler des GLRenderer
  enum RenderMode { Mode2D, Mode3D };

  //! \brief Empty constructor. It does not setup graphics because the
  //! OpenGL can be not yet initialized.
  GLRenderer()
    : //m_default_camera(),
      //m_current_camera(),
      m_background_color(Color::Black),
      m_shader("Shader01"),
      m_vao("VAO"),
      m_pos("vert"),
      m_tex("vertTexCoord"),
      m_mode(GLRenderer::Mode2D)
  {
  }

  //! \brief Delete buffers, textures and program.
  ~GLRenderer()
  {
  }

  //! \brief Set the background color. This color is used for clearing
  //! the screen.
  inline void backgroundColor(const Color& color)
  {
    m_background_color = color;
  }

  //! \brief Get the background color. This color is used for clearing
  //! the screen.
  inline Color backgroundColor() const
  {
    return m_background_color;
  }

  //! \brief Change the renderer mode
  inline void renderStyle(const RenderMode style)
  {
    m_mode = style;
  }

  //! \brief Get the current renderer mode
  inline RenderMode renderStyle() const
  {
    return m_mode;
  }

  //! Get the number of pixels of the viewport
  virtual uint32_t screenWidth() const = 0;

  //! Get the number of pixels of the viewport
  virtual uint32_t screenHeight() const = 0;

protected:

  //! \brief Same role than a constructor but here we are sure than
  //! and OpenGL context is created before.
  bool setupGraphics();

  //! \brief
  inline void clearScreen() const;

  //! \brief
  void draw();

  virtual void setContext() = 0;
  virtual void activateTransparency() = 0;
  virtual void activateDepthBuffer() = 0;

  //FIXME
  void setUniform(const char *name, Matrix44f const &mat);

protected:

  Color m_background_color;
  GLShader m_shader;
  GLVertexArray m_vao;
  GLVertexBuffer<float> m_pos;
  //GLVertexBuffer<float> m_col;
  GLVertexBuffer<float> m_tex;
  Movable<float, 3U> m_movable;
  GLTexture2D m_texture;
  RenderMode m_mode;
};

#endif /* RENDERER_HPP_ */

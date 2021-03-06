//=====================================================================
// OpenGLCppWrapper: A C++11 OpenGL 'Core' wrapper.
// Copyright 2018-2019 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of OpenGLCppWrapper.
//
// OpenGLCppWrapper is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenGLCppWrapper is distributedin the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenGLCppWrapper.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef EXAMPLE_06_SPHERE_EBO_HPP
#  define EXAMPLE_06_SPHERE_EBO_HPP

#  include "OpenGLCppWrapper.hpp"
#  include <iostream>

class GLExample06: public IGLWindow
{
public:

  GLExample06()
    : m_sphere("VAO_sphere"),
      m_prog("prog")
  {}

  ~GLExample06()
  {}

private:

  void createSphere();
  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual bool setup() override;
  virtual bool draw() override;

private:

  GLVertexShader     m_vertex_shader;
  GLFragmentShader   m_fragment_shader;
  GLVAO              m_sphere;
  GLProgram          m_prog;
  Movable<float, 3U> m_movable;
};

#endif // EXAMPLE_06_SPHERE_EBO_HPP

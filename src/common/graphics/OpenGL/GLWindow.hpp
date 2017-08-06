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

#ifndef GLWINDOW_HPP_
#  define GLWINDOW_HPP_

#  include <iostream>
#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#  include "GLObject.hpp"

// Return the number of elements in an array
#  ifndef ARRAY_SIZE
#    define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#  endif

class IGLWindow
{
public:

  virtual ~IGLWindow() {}
  int create();
  virtual bool setup() = 0;
  virtual bool draw() = 0;
  void update();
  void close();
  inline float dt() const
  {
    return m_deltaTime;
  }

private:

  void FPS();

  double m_lastTime;
  double m_lastFrameTime;
  int m_nbFrames;
  float m_deltaTime;

protected:

  const int m_width = 1024;
  const int m_height = 768;
  const char *m_title = "Example";
  GLFWwindow *m_window;
};

#endif /* GLWINDOW_HPP_ */

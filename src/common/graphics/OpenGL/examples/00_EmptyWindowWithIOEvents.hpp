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

#ifndef EXAMPLE_00_EMPTY_WINDOW_WITH_IO_EVENTS_HPP
#  define EXAMPLE_00_EMPTY_WINDOW_WITH_IO_EVENTS_HPP

#  include "OpenGLCppWrapper.hpp"
#  include <iostream>

class GLExample00: public IGLWindow
{
public:

  GLExample00()
  {
    std::cout << "Hello" << std::endl;
  }

  ~GLExample00()
  {
    std::cout << "Bye" << std::endl;
  }

protected:

  virtual void onWindowSizeChanged(const float width, const float height) override;
  virtual void onMouseMoved(const double xpos, const double ypos) override;
  virtual bool setup() override;
  virtual bool draw() override;
};

#endif // EXAMPLE_00_EMPTY_WINDOW_WITH_IO_EVENTS_HPP

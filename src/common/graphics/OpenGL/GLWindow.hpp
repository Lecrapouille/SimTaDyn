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

#ifndef GLWINDOW_HPP_
#  define GLWINDOW_HPP_

#  include <GL/glew.h>
#  include <GLFW/glfw3.h>
#  include "GLObject.hpp"

// FIXME: move outside this code
// Return the number of elements in an array
#  ifndef ARRAY_SIZE
#    define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#  endif

// **************************************************************
//! \brief
// **************************************************************
class IGLWindow
{
public:

  //------------------------------------------------------------------
  //! \brief Dummy constructor. Does not start the OpenGL context by
  //! security. To do it call start()
  //------------------------------------------------------------------
  IGLWindow();

  //------------------------------------------------------------------
  //! \brief Destructor. Release the OpenGL context. Call the virtual
  //! methode stop().
  //------------------------------------------------------------------
  virtual ~IGLWindow();

  //------------------------------------------------------------------
  //! \brief Start the OpenGL context and starts the rendering loop.
  //------------------------------------------------------------------
  bool start();

  //------------------------------------------------------------------
  //! \brief Return the delta time (in ms) with.
  //------------------------------------------------------------------
  inline float dt() const
  {
    return m_deltaTime;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline GLFWwindow *obj()
  {
    return m_window;
  }

private:

  //------------------------------------------------------------------
  //! \brief Private function needed by dt(). Compute and display in
  //! in the window title the number of frame per seconds (FPS).
  //------------------------------------------------------------------
  void FPS();

  double m_lastTime;
  double m_lastFrameTime;
  int m_fps;
  float m_deltaTime;
  bool m_opengl_context = false;

protected:

  //------------------------------------------------------------------
  //! \brief Virtual method. Add here all stuffs concerning the init
  //! of your 3D game.
  //! \return false for aborting start(), else true for continuing.
  //------------------------------------------------------------------
  virtual bool setup() = 0;

  //------------------------------------------------------------------
  //! \brief Virtual method. Add here all stuffs painting your 3D
  //! world to be displayed. This method is called by the udpdate()
  //! method.
  //! \return false for halting update(), else true for continuing.
  //------------------------------------------------------------------
  virtual bool draw() = 0;

  //------------------------------------------------------------------
  //! \brief Main loop for displaying the 3D world. Call draw().
  //------------------------------------------------------------------
  void update();

  //------------------------------------------------------------------
  //! \brief Virtual method. Release your 3D models from memory.
  //! Called by the destructor.
  //------------------------------------------------------------------
  virtual void release();

  const int m_width = 1024;
  const int m_height = 768;
  const char *m_title = "Example";
  GLFWwindow *m_window = nullptr;
};

#endif /* GLWINDOW_HPP_ */

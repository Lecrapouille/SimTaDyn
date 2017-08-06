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

#include "GLWindow.hpp"
#include <sstream>

static void onError(int /*errorCode*/, const char* msg)
{
  throw std::runtime_error(msg);
}

void IGLWindow::FPS()
{
  double currentTime = glfwGetTime();
  m_deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
  m_lastFrameTime = currentTime;
  ++m_nbFrames;

  if (currentTime - m_lastTime >= 1.0)
    {
      // If last prinf() was more than 1sec ago printf and reset
      std::ostringstream oss;
      int fps = static_cast<int>(1000.0 / static_cast<double>(m_nbFrames));
      oss << '[' << fps << " FPS] " << m_title;
      glfwSetWindowTitle(m_window, oss.str().c_str());
      m_nbFrames = 0;
      m_lastTime += 1.0;
    }
}

int IGLWindow::create()
{
  int res;

  // Initialise GLFW
  glfwSetErrorCallback(onError);
  if (!glfwInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      getchar();
      return -1;
    }

  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Open a window and create its OpenGL context
  m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
  if (nullptr == m_window)
    {
      std::cerr << "Failed to open GLFW window" << std::endl;
      getchar();
      glfwTerminate();
      return -1;
    }
  glfwMakeContextCurrent(m_window);

  // Initialize GLEW
  glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
  if (GLEW_OK != glewInit())
    {
      std::cerr << "Failed to initialize GLFW" << std::endl;
      getchar();
      glfwTerminate();
      return -1;
    }

  // print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

  // make sure OpenGL version 3.2 API is available
  if (!GLEW_VERSION_3_2)
    {
      std::cerr << "OpenGL 3.2 API is not available." << std::endl;
      getchar();
      glfwTerminate();
      return -1;
    }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
  try
    {
      res = setup();
    }
  catch (const GLObjectException& e)
    {
      LOGIS("%s", e.message().c_str());
      res = false;
    }

  if (false == res)
    {
      std::cerr << "Failed setting-up graphics" << std::endl;
      return -1;
    }

  // init FPS
  m_lastTime = glfwGetTime();
  m_lastFrameTime = m_lastTime;
  m_nbFrames = 0;

  update();
  close();
  return 0;
}

void IGLWindow::update()
{
  try
    {
      do
        {
          FPS();
          if (false == draw())
            {
              std::cerr << "Aborting" << std::endl;
              return ;
            }
          // Swap buffers
          glfwSwapBuffers(m_window);
          glfwPollEvents();
        }
      // Check if the ESC key was pressed or the window was closed
      while ((GLFW_PRESS != glfwGetKey(m_window, GLFW_KEY_ESCAPE)) &&
             (0 == glfwWindowShouldClose(m_window)));
    }
  catch (const GLObjectException& e)
    {
      LOGIS("%s", e.message().c_str());
    }
}

// Close OpenGL window and terminate GLFW
void IGLWindow::close()
{
  glfwTerminate();
}

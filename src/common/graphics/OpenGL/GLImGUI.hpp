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

#ifndef GLIMGUI_HPP
#  define GLIMGUI_HPP

// *****************************************************************************
//! \file GLImGUI.hpp wraps function calls of the ImGUI project.
// *****************************************************************************

#  define IMGUI_IMPL_OPENGL_LOADER_GLEW

#  include "GLWindow.hpp"
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wstrict-overflow"
#  pragma GCC diagnostic ignored "-Wswitch-default"
#  pragma GCC diagnostic ignored "-Wcast-qual"
#  pragma GCC diagnostic ignored "-Waggregate-return"
#  pragma GCC diagnostic ignored "-Wsign-promo"
#  pragma GCC diagnostic ignored "-Wfloat-equal"
#  pragma GCC diagnostic ignored "-Wsign-conversion"
#  pragma GCC diagnostic ignored "-Wconversion"
#    include "imgui/imgui.h"
#    include "imgui/imgui_impl_glfw.h"
#    include "imgui/imgui_impl_opengl3.h"
# pragma GCC diagnostic pop

// **************************************************************
//! \brief Class wrapper for the dear imgui library: an imediate
//! mode (im) graphical user interface (GUI) for OpenGL.
//! https://github.com/ocornut/imgui
// **************************************************************
class IGLImGUI
{
public:

  //------------------------------------------------------------------
  //! \brief Empty constructor. Do nothing.
  //------------------------------------------------------------------
  IGLImGUI() {}

  //------------------------------------------------------------------
  //! \brief Release Dear imgui allocated resources.
  //------------------------------------------------------------------
  virtual ~IGLImGUI()
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  //------------------------------------------------------------------
  //! \brief Start Dear imgui context. Use glfw routines.
  //! \param window the OpenGL window
  //------------------------------------------------------------------
  bool setup(IGLWindow &window)
  {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.window(), true);
    ImGui_ImplOpenGL3_Init(NULL);
    ImGui::StyleColorsDark();

    return true;
  }

  //------------------------------------------------------------------
  //! \brief Prepare the Dear imgui to draw the HMI. This will call the
  //! render() pure virtual method.
  //------------------------------------------------------------------
  bool draw()
  {
    bool res;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    res = render();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return res;
  }

protected:

  //------------------------------------------------------------------
  //! \brief Method for drawing the HMI. This has to be implemented by
  //! the derived class. Derived class shall override this method for
  //! drawing ImGUI objects.
  //! \return false if the rendering encountered an problem. As effect
  //! this will prevent IGLWindow calling draw() which can react to
  //! this problem.
  //------------------------------------------------------------------
  virtual bool render() = 0;
};

#endif

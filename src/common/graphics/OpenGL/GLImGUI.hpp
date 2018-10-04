#ifndef GLIMGUI_HPP
#  define GLIMGUI_HPP

#  include "GLWindow.hpp"
#  include "imgui/imgui.h"
#  include "imgui/imgui_impl_glfw.h"
#  include "imgui/imgui_impl_opengl3.h"

// **************************************************************
//! \brief Class wrapper for the dear imgui library: an imediate mode
//! (im) graphical user interface (GUI) for
//! OpenGL. https://github.com/ocornut/imgui
// **************************************************************
class IGLImGUI
{
public:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  IGLImGUI() {}

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  ~IGLImGUI()
  {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  bool setup(IGLWindow &window)
  {
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.obj(), true);
    ImGui_ImplOpenGL3_Init(NULL);
    ImGui::StyleColorsDark();

    return true;
  }

  //------------------------------------------------------------------
  //! \brief
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
  //! \brief
  //------------------------------------------------------------------
  virtual bool render() = 0;
};

#endif

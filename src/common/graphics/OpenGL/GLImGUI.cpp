#include "GLImGUI.hpp"

IGLImGUI::IGLImGUI()
{
}

IGLImGUI::~IGLImGUI()
{
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

bool IGLImGUI::setup(IGLWindow &window)
{
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window.obj(), true);
  ImGui_ImplOpenGL3_Init(NULL);
  ImGui::StyleColorsDark();

  return true;
}

bool IGLImGUI::draw()
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

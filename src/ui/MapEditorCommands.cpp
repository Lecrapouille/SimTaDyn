#include "MapEditorCommands.hpp"

//------------------------------------------------------------------
void MoveCameraCommand::execute(MapEditorWindow& win)
{
  std::cout << "Move Camera" << std::endl;
  win.mapEditor().moveCamera(m_direction);
}

//------------------------------------------------------------------
void SplitScreenCommand::execute(MapEditorWindow& win)
{
  std::cout << "Split View" << std::endl;
  win.splitView(m_oritentation);
}

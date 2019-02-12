#include "MapEditorCommands.hpp"

//------------------------------------------------------------------
void MoveCameraCommand::execute(MapEditorWindow& editor)
{
  std::cout << "Move Camera" << std::endl;
  editor.moveCamera(m_direction);
}

//------------------------------------------------------------------
void SplitScreenCommand::execute(MapEditorWindow& editor)
{
  std::cout << "Split View" << std::endl;
  editor.splitView(m_oritentation);
}

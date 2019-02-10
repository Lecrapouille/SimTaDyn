//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef MVP_HPP
#define MVP_HPP

#include "DrawingArea.hpp"
#include "SimTaDynMap.hpp"

class MapPresenter
{
public:

  MapPresenter(/*MapEditor& window,*/ SimTaDynMapPtr map = nullptr)
    : //m_window(window),
      m_map(map)
  {
    initTools();
    initCallbacks();
  }

  void changeModel(SimTaDynMapPtr map)
  {
    m_map = map;
  }

  inline GLDrawingArea& view()
  {
    return m_glarea;
  }

  inline SimTaDynMapPtr modelPtr()
  {
    return m_map;
  }

  inline SimTaDynMap& model()
  {
    return *m_map;
  }

  void onMousePressed(GdkEventButton* event)
  {
    if (event->type == GDK_BUTTON_PRESS)
    {
      /*switch (event->button)
        {
        case 1:
          std::cout << "GLDrawingArea::on_button_press_event button1" << std::endl;
          m_edition_tools[window.actionType()]->exec1(event->x, event->y);
          break;
        case 2:
          std::cout << "GLDrawingArea::on_button_press_event button2" << std::endl;
          m_edition_tools[window.actionType()]->exec2(event->x, event->y);
          break;
        case 3:
          std::cout << "GLDrawingArea::on_button_press_event button3" << std::endl;
          m_edition_tools[window.actionType()]->exec3(event->x, event->y);
          break;
        }*/
    }
  }

private:

  void initTools()
  {
    /*m_edition_tools[ActionType::Add] = std::make_unique<AddCellTool>(*this);
    m_edition_tools[ActionType::Remove] = std::make_unique<RemoveCellTool>(*this);
    m_edition_tools[ActionType::Select] = std::make_unique<SelectCellTool>(*this);
    m_edition_tools[ActionType::Move] = std::make_unique<MoveCellTool>(*this);*/
  }

  void initCallbacks()
  {
    /*m_glarea->signal_button_press_event().connect_notify
      (sigc::mem_fun(*this, &MapEditor::onMousePressed));
    Glib::signal_timeout().connect
      (sigc::mem_fun(*this, &MapEditor::onKeyPressed), m_timeout_ms);
    m_glarea->signal_enter_notify_event().connect_notify(sigc::bind<GLDrawingArea&>(
       sigc::mem_fun(window, &MapEditorWindow::onEnterViewEvent), *glarea));
    m_glarea->signal_leave_notify_event().connect_notify(sigc::bind<GLDrawingArea&>(
       sigc::mem_fun(window, &MapEditorWindow::onLeaveViewEvent), *glarea));
*/
  }

  void drawMap()
  {
    //LOGI("Repainting map '%s'", m_map.name().c_str());
    //map.drawnBy(m_glarea);
  }

private:

  GLDrawingArea  m_glarea;
  SimTaDynMapPtr m_map;
  const unsigned int m_timeout_ms = 10u;
};

#endif

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

#include "SimTaDynWindow.hpp"
#include "MapEditor.hpp"

/*MapEditor& mapeditor = SimTaDynContext::mapEditor();
   mapeditor.addButon(Gtk::Stock::NO, "42 42 FOO", "42 42 FOO");
   mapeditor.addButon(Gtk::Stock::NO, "42 42 TOTO", "42 42 TOTO");
   mapeditor.addButon(Gtk::Stock::NO, "42 .", "42 .");*/

int main(int argc, char** argv)
{
  std::cout << "Welcome to SimTaDyn version "
            << config::major_version
            << '.'
            << config::minor_version
            << std::endl;
  Logger::instance();

  LOGI("** Init GTK");
  const Gtk::Main kit(argc, argv);
  Gsv::init();

  LOGI("** Init SimTaDyn");
  ResourceManager<Key>::instance();
  LoaderManager::instance();
  SimForth::instance();
  ForthEditor::instance();
  MapEditor::instance();

  SimTaDynWindow window;
  SimForth::instance().boot();

  LOGI("** End of the init: starting the runtime");
  kit.run(window);

  LOGI("** Leaving SimTaDyn: releasing the memory");
  ForthEditor::destroy();
  MapEditor::destroy();
  SimForth::destroy();
  LoaderManager::destroy();
  ResourceManager<Key>::destroy();
  Logger::destroy();
  return 0;
}

// FIXME: faire une classe avec une methode pure pour lancer les trucs de 'utilsateur

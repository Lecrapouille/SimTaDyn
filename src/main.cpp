#include "SimTaDynWindow.hpp"
#include "SimTaDynLogger.hpp"
#include "MapEditor.hpp"
#include "ForthEditor.hpp"

/*MapEditor& mapeditor = SimTaDynContext::mapEditor();
   mapeditor.addButon(Gtk::Stock::NO, "42 42 FOO", "42 42 FOO");
   mapeditor.addButon(Gtk::Stock::NO, "42 42 TOTO", "42 42 TOTO");
   mapeditor.addButon(Gtk::Stock::NO, "42 .", "42 .");*/

int main(int argc, char** argv)
{
  const Gtk::Main kit(argc, argv);
  Gsv::init();

  SimTaDynLogger::instance();
  SimForth::instance();
  ForthEditor::instance();
  MapEditor::instance();

  SimTaDynWindow window;

  kit.run(window);

  ForthEditor::destroy();
  MapEditor::destroy();
  SimForth::destroy();
  SimTaDynLogger::destroy();
  return 0;
}

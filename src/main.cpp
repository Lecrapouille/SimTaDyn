#include "SimTaDynWindow.hpp"
#include "SimTaDynLogger.hpp"

/*MapEditor& mapeditor = SimTaDynContext::mapEditor();
   mapeditor.addButon(Gtk::Stock::NO, "42 42 FOO", "42 42 FOO");
   mapeditor.addButon(Gtk::Stock::NO, "42 42 TOTO", "42 42 TOTO");
   mapeditor.addButon(Gtk::Stock::NO, "42 .", "42 .");*/

int main(int argc, char** argv)
{
  const Gtk::Main kit(argc, argv);
  Gsv::init();

  SimTaDynWindow window;

  kit.run(window);

  SimTaDynLogger::destroy();
  return 0;
}

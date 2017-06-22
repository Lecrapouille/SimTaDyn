#include "SimTaDynWindow.hpp"
#include "Logger.hpp"
#include "MapEditor.hpp"
#include "ForthEditor.hpp"

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
  SimForth::instance();
  ForthEditor::instance();
  MapEditor::instance();

  SimTaDynWindow window;
  SimForth::instance().boot();

  LOGI("** End of the init. Starting the runtime");
  kit.run(window);

  LOGI("** End of the runtime. Releasing the memory");
  ForthEditor::destroy();
  MapEditor::destroy();
  SimForth::destroy();
  Logger::destroy();
  return 0;
}

// FIXME: faire une classe avec une methode pure pour lancer les trucs de 'utilsateur

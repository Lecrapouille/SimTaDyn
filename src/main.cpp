#include "Context.hpp"

SimTaDynWindow *SimTaDynContext::s_itself = nullptr;

int main(int argc, char** argv)
{
  const Gtk::Main kit(argc, argv);
  Gsv::init();

  SimTaDynWindow *window = SimTaDynContext::mainWindow();
  assert(nullptr != window);


    // FIXME: temporary, just for testing
  ForthEditor& fortheditor = SimTaDynContext::forthEditor();
    fortheditor.addButon(Gtk::Stock::YES, "TRACE.ON", "TRACE.ON ( -- )");
    fortheditor.addButon(Gtk::Stock::NO, "TRACE.OFF", "TRACE.OFF ( -- )");
    /*MapEditor& mapeditor = SimTaDynContext::mapEditor();
    mapeditor.addButon(Gtk::Stock::NO, "42 42 FOO", "42 42 FOO");
    mapeditor.addButon(Gtk::Stock::NO, "42 42 TOTO", "42 42 TOTO");
    mapeditor.addButon(Gtk::Stock::NO, "42 .", "42 .");*/

  kit.run(*window);
  return 0;
}

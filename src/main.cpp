#include "SimTaDynContext.hpp"

// Create the context as singleton
SimTaDynContext SimTaDynContext::s_SimTaDyn = SimTaDynContext();

int main(int argc, char** argv)
{
  const Gtk::Main kit(argc, argv);
  Gsv::init();
  //Gtk::GL::init(argc, argv);

  SimTaDynWindow main_window("SimTaDyn");
  SimTaDynContext& SimTaDyn = SIMTADYN();
  SimTaDyn.m_window = &main_window;

  // FIXME: temporary, just for testing
  SimTaDyn.m_window->addForthScriptButon(Gtk::Stock::YES, "TRACE.ON", "TRACE.ON ( -- )");
  SimTaDyn.m_window->addForthScriptButon(Gtk::Stock::NO, "TRACE.OFF", "TRACE.OFF ( -- )");
  SimTaDyn.m_window->addMapScriptButon(Gtk::Stock::NO, "42 42 FOO", "42 42 FOO");

  kit.run(*(SimTaDyn.m_window));
  return 0;
}

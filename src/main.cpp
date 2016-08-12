#include "gui/gui.hpp"
#include <gtkmm/main.h>

int main (int argc, char** argv)
{
  const Gtk::Main kit(argc, argv);
  Gtk::GL::init(argc, argv);
  SimTaDynWindow main_window("SimTaDyn");
  kit.run(main_window);
  return 0;
}

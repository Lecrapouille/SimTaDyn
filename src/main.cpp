#if 1

#include "SimTaDynCells.hpp"

int main (int argc, char** argv)
{
#if 0
  SimTaDynCell *c = new SimTaDynCell("c");
  SimTaDynCell *n1 = new SimTaDynCell("n1");
  SimTaDynCell *n2 = new SimTaDynCell("n2");

  c->addExplicitRelation(*n1);
  c->addExplicitRelation(*c);
  n1->addExplicitRelation(*c);
  n1->addExplicitRelation(*n2);
  n2->addExplicitRelation(*c);
  delete c;
  n1->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));


  return 0;
#endif

#if 0
  SimTaDynCell *c = new SimTaDynCell("c");
  SimTaDynCell *n1 = new SimTaDynCell("n1");
  SimTaDynCell *n2 = new SimTaDynCell("n2");

  c->addExplicitRelation(*n1);
  c->addExplicitRelation(*n2);
  n1->addExplicitRelation(*c);
  n1->addExplicitRelation(*n2);

  c->debugDisplayAllObservers();
  n1->debugDisplayAllObservers();
  n2->debugDisplayAllObservers();

  std::cout << "Delete-------------\n";
  delete c;

  std::cout << "DEBUG-------------\n";
  //n1->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));
  //c->debugDisplayAllObservers();
  n1->debugDisplayAllObservers();
  n2->debugDisplayAllObservers();

  return 0;
#endif

  SimTaDynCell *a = new SimTaDynCell("a");
  SimTaDynCell *c = new SimTaDynCell("c");
  SimTaDynCell *n1 = new SimTaDynCell("n1");
  SimTaDynCell *n2 = new SimTaDynCell("n2");

  c->whoAmI();
  n1->whoAmI();
  n2->whoAmI();

  //c->addImplicitRelation(*a);
  c->addExplicitRelation(*n1);
  c->addExplicitRelation(*n2);
  c->addExplicitRelation(*c);

  std::cout << "-------------\n";
  n1->addExplicitRelation(*c);
  // n1->removeExplicitRelation(*c);
  delete n1;

  std::cout << "-------------\n";
  c->whoAmI();
  std::cout << "-------------\n";
  c->debugDisplayAllObservers();

  std::cout << "-------------\n";
  c->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));

  std::cout << "-------------\n";
  delete c;
  delete a;
  //delete n1;
  delete n2;
  //c->debugDisplayAllObservers();

}

#else

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
#endif

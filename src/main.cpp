#if 1

#include "SimTaDynCells.hpp"

int main (int argc, char** argv)
{

  SimTaDynCell *g1 = new SimTaDynCell("g1");
  SimTaDynCell *g2 = new SimTaDynCell("g2");
  SimTaDynCell *d1 = new SimTaDynCell("d1");

  g1->addImplicitRelation(*d1);
  //g2->addImplicitRelation(*d1);
  //g1->debugDisplayAllObservers();
  //g1->removeImplicitRelation(*d1);
  //g1->destroy();
  delete g1;
  d1->debugDisplayAllObservers();

  std::cout << "---------------------\n";

  //g1->removeImplicitRelation(*d1);
  g2->debugDisplayAllObservers();
  d1->debugDisplayAllObservers();

  std::cout << "QQ---------------------\n";
  //delete g1;

  std::cout << "---------------------\n";
  delete g2;
  return 0;


  #if 0
  SimTaDynCell *g1 = new SimTaDynCell("g1");
  SimTaDynCell *g2 = new SimTaDynCell("g2");

  SimTaDynCell *d1 = new SimTaDynCell("d1");
  SimTaDynCell *d2 = new SimTaDynCell("d2");
  SimTaDynCell *d3 = new SimTaDynCell("d3");

  g1->addImplicitRelation(*d1);
  g1->addImplicitRelation(*d2);
  g1->addImplicitRelation(*d3);
  g1->addImplicitRelation(*d3);

  g2->addImplicitRelation(*d1);
  g2->addImplicitRelation(*d2);
  g2->addImplicitRelation(*d3);

  g1->debugDisplayAllObservers();
  g2->debugDisplayAllObservers();

  d1->debugDisplayAllObservers();
  d2->debugDisplayAllObservers();
  d3->debugDisplayAllObservers();

  g1->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));

  g1->whoAmI();
  g2->whoAmI();
  d1->whoAmI();
  d2->whoAmI();
  d3->whoAmI();

  std::cout << "--------------------- Main::destroy\n";
  delete g1;

  //g1->removeImplicitRelation(*d1);

  std::cout << "Main::debug\n";
  //g1->debugDisplayAllObservers();
  g2->debugDisplayAllObservers();
  d1->debugDisplayAllObservers();
  d2->debugDisplayAllObservers();
  d3->debugDisplayAllObservers();

  return 0;
#endif
#if 0
  // Test KO
  SimTaDynCell *g1 = new SimTaDynCell("g1");
  SimTaDynCell *g2 = new SimTaDynCell("g2");

  SimTaDynCell *d1 = new SimTaDynCell("d1");
  SimTaDynCell *d2 = new SimTaDynCell("d2");
  SimTaDynCell *d3 = new SimTaDynCell("d3");

  g1->addExplicitRelation(*d1);
  g1->addExplicitRelation(*d2);
  g1->addExplicitRelation(*d3);

  g2->addExplicitRelation(*d1);
  g2->addExplicitRelation(*d2);
  g2->addExplicitRelation(*d3);

  g1->debugDisplayAllObservers();
  g2->debugDisplayAllObservers();

  return 0;

#endif

#if 0

  SimTaDynCell *c = new SimTaDynCell("c");
  SimTaDynCell *n1 = new SimTaDynCell("n1");
  SimTaDynCell *n2 = new SimTaDynCell("n2");

  c->addExplicitRelation(*n1);
  c->addExplicitRelation(*n2);
  n1->addExplicitRelation(*c);
  n1->removeExplicitRelation(*c);
  std::cout << "kkkkkkkkkkk\n";
  delete n1;
  c->debugDisplayAllObservers(); // c --> n1 et n2
  c->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));

  return 0;

#endif

# if 0
  // BUG
  SimTaDynCell *c = new SimTaDynCell("c");
  SimTaDynCell *n1 = new SimTaDynCell("n1");
  SimTaDynCell *n2 = new SimTaDynCell("n2");

  c->addExplicitRelation(*n1);
  c->addExplicitRelation(*n2);
  n1->addExplicitRelation(*c);

  std::cout << "QQ-------------\n";
  //n1->removeExplicitRelation(*c);
  delete c;
  std::cout << "BB-------------\n";
  n1->debugDisplayAllObservers();
  n1->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));

  return 0;

#endif
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

#if 0
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
  // c->addExplicitRelation(*c);

  std::cout << "-------------\n";
  n1->addExplicitRelation(*c);
  n1->removeExplicitRelation(*c); // Ce code fait planter a la ligne

  std::cout << "DEBUG n1\n";
  n1->debugDisplayAllObservers();
  delete n1;

  std::cout << "C who-------------\n";
  c->whoAmI();

  //std::cout << "Merde1-------------\n";
  c->debugDisplayAllObservers(); // ------------- ICI bug n1 toujours dispo
  //std::cout << "Merde2-------------\n";
  //n1->debugDisplayAllObservers();
  //std::cout << "Merde3-------------\n";
  n2->debugDisplayAllObservers();

  std::cout << "BUUUUU-------------\n";
  c->moveToPosition(Position3D(1.0f, 2.0f, 3.0f));

  std::cout << "-------------\n";
  delete c;
  delete a;
  //delete n1;
  delete n2;
  //c->debugDisplayAllObservers();
#endif
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

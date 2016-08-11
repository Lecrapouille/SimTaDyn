#include "shapefile.hpp"

int main(void)
{
  SimTaDynCell* c1;
  SimTaDynGraph graph("Graph1");
  ShapefileLoader loader;

  cout << "Graph Name: " << graph.getName() << endl;
  loader.loadShapefile("3dpoints.shp", graph);
  cout << endl;
  cout << endl;
  cout << "Graph Name: " << graph.getName() << endl;
  loader.loadShapefile("3dpoints.shp", graph);
  cout << endl;
  cout << endl;
  cout << "Graph Name: " << graph.getName() << endl;

  cout << endl;
  cout << "Il y a " << SimTaDynCell::howMany() << " SimTaDynCell" << endl;
  cout << "Il y a " << SimTaDynNode::howMany() << " SimTaDynNode" << endl;
  c1 = graph.getNode(1);
  c1->showExplicitNeighbor();
  c1->addExplicitNeighbor(graph.getNode(2));
  c1->addExplicitNeighbor(graph.getNode(3));

  c1->showExplicitNeighbor();
  c1->moveToPosition(Position3D(0.0f, 0.0f, 0.0f));

#if 0
  Position3D p1(1.0f, 1.0f, 1.0f);
  Position3D p2(2.0f, 2.0f, 2.0f);
  Position3D p3(3.0f, 3.0f, 3.0f);
  Position3D p4(4.0f, 4.0f, 4.0f);

  SimTaDynCell* S1 = new SimTaDynNode(p1);
  SimTaDynCell* S2 = new SimTaDynNode("c02", p2);
  SimTaDynCell* S3 = new SimTaDynNode("c03", p3,"DUP DUP");
  SimTaDynCell* S4 = new SimTaDynNode(p4);

  cout << "Il y a " << SimTaDynCell::howMany() << " SimTaDynCell" << endl;
  cout << "Il y a " << SimTaDynNode::howMany() << " SimTaDynNode" << endl;
  S1->showExplicitNeighbor();

  cout << "Creation des arcs avec S1" << endl;
  S1->addExplicitNeighbor(S4);
  S1->addExplicitNeighbor(S3);
  S1->addExplicitNeighbor(S2);
  S1->addExplicitNeighbor(S1);
  S1->showExplicitNeighbor();

  cout << "Creation des arcs avec S3" << endl;
  S3->addExplicitNeighbor(S4);
  S3->addExplicitNeighbor(S1);
  S3->showExplicitNeighbor();

  cout << "Notification" << endl;
  S1->moveToPosition(p1);

  cout << "Destruction de S3" << endl;
  delete S3;
  S1->showExplicitNeighbor();

  cout << "Destruction des autres" << endl;
  delete S1;
  delete S2;
  delete S4;
#endif

#if 0
  SimTaDynGraph G("Graph1");
  SimTaDynCell* c1;

  G.addNode(Position3D(1.0f, 1.0f, 1.0f));
  G.addNode(Position3D(2.0f, 2.0f, 2.0f));
  G.addNode(Position3D(3.0f, 3.0f, 3.0f));

  cout << "Il y a " << SimTaDynCell::howMany() << " SimTaDynCell" << endl;
  cout << "Il y a " << SimTaDynNode::howMany() << " SimTaDynNode" << endl;

  c1 = G.getNode(1);
  c1->addExplicitNeighbor(G.getNode(2));
  c1->addExplicitNeighbor(G.getNode(3));

  c1->showExplicitNeighbor();
  c1->moveToPosition(Position3D(0.0f, 0.0f, 0.0f));
#endif

  return 0;
}

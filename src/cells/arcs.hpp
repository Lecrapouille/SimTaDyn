#ifndef ARCS_HPP_
#  define ARCS_HPP_

#include "nodes.hpp"

// *************************************************************************************************
// Define an Arc (aka Edge)
// ************************************************************************************************
class SimTaDynArc: public SimTaDynCell, private ClassCounter<SimTaDynArc>
{
  // FIXME: risque de bug:
  // SimTaDynCell* N1 = new SimTaDynNode();
  // N2.addExplicitNeighbor(N1) --> ne sera pas connu du graphe
public:
  SimTaDynCell* node_head_;
  SimTaDynCell* node_tail_;
  bool oriented_;

public:
  /*
   * Transition cost (ponderation)
   */
  int cost_;

  // Quand on relie a la souris deux noeuds
  // FIXME utiliser des &
  SimTaDynArc(SimTaDynCell* node_tail, SimTaDynCell* node_head, string name, bool oriented, int cost);
  //SimTaDynArc(Key k1, Key k2, string name, bool oriented = false, int cost = 0);

  // Pas possible: uniquement depuis la classe Grpahe
  //SimTaDynArc(Key key_first, Key key_end, string pname, bool oriented = false, int cost = 0)
  //{
  //  node_head_ = Graph.node_search_(node_tail);
  //  node_tail_ = Graph.node_search_(node_head);
  //  private_constructor(oriented, name, cost);
  //}
  // Quand on est en mode edition creer un arc
  SimTaDynArc(Position3D pf, Position3D pe, string pname, bool oriented, int cost);
  //SimTaDynArc(int Cost_ = 0);
  ~SimTaDynArc() { }

  void moveToPosition(const Position3D& p)
  {
    node_head_->moveToPosition(node_head_->position_ + p);
    node_tail_->moveToPosition(node_tail_->position_ + p);
    position_ = p;
  }

  Position3D& getPosition()
  {
    return position_;
  }

  void setOriented(bool new_bool);

  Key WhoAmI()
  {
    cout << "I am the SimTaDynArc #" << id_ << " Named " << name << endl;
    return id_;
  }

  static size_t howMany() { return ClassCounter<SimTaDynArc>::howMany(); }

private:
  void private_constructor(string pname, bool oriented, int cost);
};

#define SimTaDynEdge SimTaDynArc

#endif /* ARCS_HPP_ */

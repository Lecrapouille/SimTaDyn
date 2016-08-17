#ifndef NODES_HPP_
#  define NODES_HPP_

#  include "cells.hpp"

// *************************************************************************************************
// Define a Node (aka Vertex)
// *************************************************************************************************
class SimTaDynNode: public SimTaDynCell, private ClassCounter<SimTaDynNode>
{
public:
  //protected:
  /*
   * Position in space
   */

  // MovedEvent --> register

public:

  /*
   * Constructor: give a name (else use a generic one) and a position in space.
   */
  SimTaDynNode(string pname, const Position3D& p, string pcode_forth)
  { name = pname; code_forth = pcode_forth; registerEventFunc(this, &SimTaDynNode::onMoved); position_ = p; }
  SimTaDynNode(string pname, const Position3D& p)
  { name = pname; registerEventFunc(this, &SimTaDynNode::onMoved); position_ = p; }
  SimTaDynNode(const Position3D& p)
  { name = "Node_" + to_string(id_); registerEventFunc(this, &SimTaDynNode::onMoved); position_ = p; }
  ~SimTaDynNode() { }

  /*
   * Static member returning the number of SimTaDynNode instances created
   */
  static size_t howMany() { return ClassCounter<SimTaDynNode>::howMany(); }

  Key whoAmI();
  Position3D& getPosition(); // FIXME: const
  void moveToPosition(const Position3D& p);

protected:
  void onMoved(const EventMoved* movement);
};

#define SimTaDynVertex SimTaDynNode

#endif /* NODES_HPP_ */

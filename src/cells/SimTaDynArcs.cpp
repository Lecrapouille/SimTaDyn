#include "SimTaDynArcs.hpp"

// *************************************************************************************************
// Define an Arc (aka Edge)
// ************************************************************************************************
SimTaDynArc::SimTaDynArc(SimTaDynCell* node_tail, SimTaDynCell* node_head, string name, bool oriented, int cost)
{
  node_head_ = node_head;
  node_tail_ = node_tail;
  private_constructor(name, oriented, cost);
  whoAmI();
}

SimTaDynArc::SimTaDynArc(Position3D pf, Position3D pe, string name, bool oriented, int cost)
{
  node_head_ = new SimTaDynNode(pf);
  node_tail_ = new SimTaDynNode(pe);
  private_constructor(name, oriented, cost);
}

/*SimTaDynArc::SimTaDynArc(Key k1, Key k2, string name, bool oriented = false, int cost = 0)
{
  node_head_ = SimTaDyn IMPOSIBLE
  }*/

/*SimTaDynArc::SimTaDynArc(int Cost_ = 0)
{
  private_constructor(oriented, name, cost);
  name = "Node_" + to_string(id);
  Cost = Cost_;
  }*/

void SimTaDynArc::setOriented(bool new_bool)
{
  if (oriented_ != new_bool)
    {
      if (new_bool)
        {
          // Ajoute une relation si elle existe
          node_head_->addExplicitNeighbor(node_tail_);
        }
      else
        {
          // Retire une relation
          node_head_->removeExplicitNeighbor(node_tail_);
        }
      oriented_ = new_bool;
    }
}

void SimTaDynArc::private_constructor(string pname, bool oriented, int cost)
{
  name = pname;
  oriented_ = oriented;
  cost_ = cost;
  node_tail_->addExplicitNeighbor(node_head_);
  if (oriented_)
    {
      node_head_->addExplicitNeighbor(node_tail_);
    }
  position_ = node_head_->position_ + node_tail_->position_;
  position_.x /= 2.0f;
  position_.y /= 2.0f;
}

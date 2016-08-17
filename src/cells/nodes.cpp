#include "nodes.hpp"

void SimTaDynNode::onMoved(const EventMoved* movement)
{
  //cout << __PRETTY_FUNCTION__ << " ";
  cout << "    * "; whoAmI();
  cout << "      My neighbor the cell #" << movement->cell_->privateId() << " nammed \"" << movement->cell_->name
       << "\" moved to position " << movement->cell_->position_ << endl;
}

/*
 * Change the position and notify it to all neighbours
 */
void SimTaDynNode::moveToPosition(const Position3D& p)
{
  //cout << __PRETTY_FUNCTION__ << endl;
  if (position_ != p)
    {
      position_ = p;
      EventMoved m(this);
      notifyNeighbors(&m);
    }
}

Position3D& SimTaDynNode::getPosition() // FIXME: const
{
  return position_;
}

/*
 * Debug
 */
Key SimTaDynNode::whoAmI()
{
  cout << "I am the SimTaDynNode #" << id_ << " named \"" << name << "\":" << endl;
  return id_;
}

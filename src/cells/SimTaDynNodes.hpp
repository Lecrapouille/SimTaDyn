#ifndef NODES_HPP_
#  define NODES_HPP_

#  include "SimTaDynCells.hpp"

// *************************************************************************************************
// Define a Node (aka Vertex)
// *************************************************************************************************
class SimTaDynNode
  : public SimTaDynCell,
    private ClassCounter<SimTaDynNode>
{
public:

  /*
   * Constructors
   */
  SimTaDynNode(Position3D const& position)
    : SimTaDynCell(),
      Position3D(position),
  {
    id_ = howMany() - 1U;
    name = "Node_" + std::to_string(id_);
  }

  /*
   * Constructors
   */
  SimTaDynNode(Position3D const& position,
               string const& new_name,
               string const& new_code_forth = "",
               void *const new_data = NULL)
    : SimTaDynCell(new_name, new_code_forth, new_data),
      Position3D(position),
      AABB(position, position)
  {
    box_.scale(1.5f);
  }

  ~SimTaDynNode()
  {
  }

  /*
   * Static member returning the number of SimTaDynNode instances created
   */
  static size_t howMany()
  {
    return ClassCounter<SimTaDynNode>::howMany();
  }

  /*
   * For debug
   */
  Key whoAmI()
   {
     std::cout << "I am the SimTaDynNode #" << id_ << " named \"" << name << "\":" << endl;
     return id_;
   }

protected:
  void onMoved(const EventMoved* movement);
};

#define SimTaDynVertex SimTaDynNode

#endif /* NODES_HPP_ */

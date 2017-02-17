#ifndef SIMTADYNGRAPH_HPP_
#  define SIMTADYNGRAPH_HPP_

#include "CellForth.hpp"
#include "BasicGraph.hpp"
#include "BoundingBox.hpp"

// **************************************************************
//
// **************************************************************
class SimTaDynNode : public BasicNode, public CellForth
{
public:
  SimTaDynNode(BasicNode const& node)
    : BasicNode(node), CellForth()
  {
    setName();
  }

  SimTaDynNode(const Key nodeID)
    : BasicNode(nodeID), CellForth()
  {
    setName();
  }

  const std::string& name() const
  {
    return m_name;
  }

protected:
  inline void setName()
  {
    m_name = "N#" + std::to_string(m_id);
  }

  std::string m_name;

public:
  Key dataID;
};

// **************************************************************
//
// **************************************************************
class SimTaDynArc : public BasicArc, public CellForth
{
public:
  SimTaDynArc(const Key id, BasicNode& fromNode, BasicNode& toNode)
    : BasicArc(id, fromNode, toNode), CellForth()
  {
    setName();
  }

  SimTaDynArc(BasicArc const& arc)
    : BasicArc(arc), CellForth()
  {
    setName();
  }

  const std::string& name() const
  {
    return m_name;
  }

protected:
  inline void setName()
  {
    m_name = "A#" + std::to_string(m_id);
  }

  std::string m_name;

public:
  Key dataID;
};

// **************************************************************
//
// **************************************************************
template <class N, class A>
class SimTaDynGraph : public BasicGraph<N,A>
{
public:
  SimTaDynGraph(const bool directed = true)
    : BasicGraph<N,A>(directed)
  {
  }

  SimTaDynGraph(const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : BasicGraph<N,A>(noNodes, noArcs, directed)
  {
  }

  //! Return the last inserted node.
  inline N *node() const
  {
    return m_last_node;
  }

  //! Return the last inserted arc
  inline A *arc() const
  {
    return m_last_arc;
  }

protected:

  //! \brief Common function for allocating a new node.
  inline virtual N *newNode(const Key nodeID)
  {
    std::cout << "newNode" << std::endl;
    N *n = new N(nodeID);
    m_last_node = n;
    return n;
  }

  /* removeNode() {
   * positions.swap(current, lastElement)
   * nodes.last.key(current.key)
   * nodes.swap(current, lastElement)
   */

  //! \brief Common function for allocating a new arc.
  inline virtual A *newArc(BasicNode &fromNode, BasicNode &toNode)
  {
    std::cout << "newArc" << std::endl;
    A *a = new A(m_arc_unique++, fromNode, toNode);
    m_last_arc = a;
    return a;
  }

  N* m_last_node = nullptr;
  A* m_last_arc = nullptr;
  Key m_arc_unique = 0;
};

typedef SimTaDynGraph<SimTaDynNode, SimTaDynArc> SimTaDynGraph_t;

#endif /* SIMTADYNGRAPH_HPP_ */

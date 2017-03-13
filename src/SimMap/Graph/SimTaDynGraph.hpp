#ifndef SIMTADYNGRAPH_HPP_
#  define SIMTADYNGRAPH_HPP_

#  include "CellForth.hpp"
#  include "BasicGraph.hpp"
#  include "BoundingBox.hpp"
#  include "ClassCounter.hpp"

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

  // FIXME: temporary
  Cell32 m_cost = 42;

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

  // FIXME: temporary
  Cell32 m_cost = 24;

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
class SimTaDynGraph : public BasicGraph<N,A>, private ClassCounter<SimTaDynGraph<N,A>>
{
public:
  SimTaDynGraph(const bool directed = true)
    : BasicGraph<N,A>(directed), m_id(howMany())
  {
    m_name = "Graph_" + std::to_string(id());
  }

  SimTaDynGraph(const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : BasicGraph<N,A>(noNodes, noArcs, directed), m_id(howMany())
  {
    m_name = "Graph_" + std::to_string(id());
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

  //! \brief Instances counter.
  static Key howMany()
  {
    return ClassCounter<SimTaDynGraph>::howMany();
  }

  //! \brief For debug purpose.
  virtual void debug()
  {
    std::cout << "I am SimTaDynGraph #" << id() << " named '" << m_name << "'"
              << std::endl;
  }

  inline Key id() const
  {
    return m_id;
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
  Key m_id;

public:

  std::string m_name;
};

//
typedef SimTaDynGraph<SimTaDynNode, SimTaDynArc> SimTaDynGraph_t;

inline std::ostream& operator<<(std::ostream& os, const SimTaDynGraph_t& g)
{
  os << "Graph(" << g.id() << ':' << g.m_name << ')';
  return os;
}

#endif /* SIMTADYNGRAPH_HPP_ */

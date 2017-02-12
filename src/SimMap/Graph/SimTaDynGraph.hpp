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
    m_name = "Node" + std::to_string(m_id);
  }

  std::string m_name;

  //! \brief The following pointer can point to any extra data needed by the
  //! cell. This field is used by Forth to extend the implementation.
  void *m_data;

  //! \brief Position in the map/view (pixel). Set as protected to be sure to
  //! use accessors which can prevent other cells that this cell moved.
  Position3D m_position;
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
    m_name = "Arc" + std::to_string(m_id);
  }

  std::string m_name;
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
};

typedef SimTaDynGraph<SimTaDynNode, SimTaDynArc> SimTaDynGraph_t;

#endif /* SIMTADYNGRAPH_HPP_ */

#ifndef SIMTADYNGRAPH_HPP_
#  define SIMTADYNGRAPH_HPP_

#  include "CellForth.hpp"
#  include "Graph.hpp"
//#  include "BoundingBox.tpp"
#  include "ClassCounter.hpp"

// **************************************************************
//
// **************************************************************
class SimTaDynNode: public BasicNode, public CellForth
{
public:

  SimTaDynNode()
    : BasicNode(), CellForth()
  {
    init();
  }

  SimTaDynNode(const Key nodeID)
    : BasicNode(nodeID), CellForth()
  {
    init();
  }

  SimTaDynNode(BasicNode const& node)
    : BasicNode(node), CellForth()
  {
    init();
  }

protected:

  inline void setName() override
  {
    m_name = "Node" + std::to_string(id());
  }

private:

  inline void init()
  {
    setName();
    m_dataKey = id();
  }

public:

  //! \brief Key to database
  Key m_dataKey;
};

// **************************************************************
//
// **************************************************************
class SimTaDynArc: public BasicArc, public CellForth
{
public:

  SimTaDynArc()
    : BasicArc(), CellForth()
  {
    init();
  }

  SimTaDynArc(const Key id, BasicNode& fromNode, BasicNode& toNode)
    : BasicArc(id, fromNode, toNode), CellForth()
  {
    init();
  }

  SimTaDynArc(BasicArc const& arc)
    : BasicArc(arc), CellForth()
  {
    init();
  }

protected:

  inline void setName()
  {
    m_name = "Arc" + std::to_string(id());
  }

private:

  inline void init()
  {
    setName();
    m_dataKey = id();
  }

public:

  //! \brief Key to database
  Key m_dataKey;
};

template <class N, class A> class SimTaDynGraph: public BasicGraph<N, A>
{
public:

  SimTaDynGraph(const bool directed = true)
    : BasicGraph<N, A>(directed)
  {
  }

  SimTaDynGraph(std::string const& name,
                const bool directed = true)
    : BasicGraph<N, A>(name, directed)
  {
  }

  SimTaDynGraph(const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : BasicGraph<N, A>(noNodes, noArcs, directed)
  {
  }


  SimTaDynGraph(std::string const& name,
                const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : BasicGraph<N, A>(name, noNodes, noArcs, directed)
  {
  }
};

//
typedef SimTaDynGraph<SimTaDynNode, SimTaDynArc> SimTaDynGraph_t;

#endif /* SIMTADYNGRAPH_HPP_ */

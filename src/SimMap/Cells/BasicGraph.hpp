#ifndef BASIC_GRAPH_HPP_
#  define BASIC_GRAPH_HPP_

//! Special container
#  include "GraphMemory.hpp"
#  include "simtypes.hpp"

class BasicNode;
class BasicArc;

// *************************************************************************************************
//
// *************************************************************************************************
class BasicArc
{
public:
  BasicArc(Key id, BasicNode *fromNode, BasicNode *toNode)
    : m_id(id),
      m_fromNode(fromNode),
      m_toNode(toNode)
  {
  }

  /*BasicArc(BasicArc const& arc)
    : m_id(arc.id),
      m_fromNode(arc.m_fromNode),
      m_toNode(arc.m_toNode)
  {
  }*/

  BasicArc* clone() const
  {
    return new BasicArc(*this);
  }

  virtual ~BasicArc() {}

  //! \brief Return the unique identifier.
  inline Key id() const { return m_id; }
  inline BasicNode *from() const { return m_fromNode; }
  inline void from(BasicNode *fromNode) { m_fromNode = fromNode; }
  inline BasicNode *to() const { return m_toNode; }
  inline void to(BasicNode *toNode) { m_toNode = toNode; }

protected:
  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;

  BasicNode *m_fromNode;
  BasicNode *m_toNode;
};

// *************************************************************************************************
//
// *************************************************************************************************
class BasicNode
{
public:
  BasicNode(const Key nodeID)
  {
    m_arcs.reserve(4);
    m_id = nodeID;
  }

  BasicNode(BasicNode const& node)
    : m_id(node.m_id), m_arcs(node.m_arcs)
  {
    m_arcs.reserve(4);
  }

  BasicNode* clone() const
  {
    return new BasicNode(*this);
  }

  virtual ~BasicNode() {}

  //! \brief Return the unique identifier.
  inline Key id() const
  {
    return m_id;
  }

  inline const std::vector<BasicArc*> &arcs() const
  {
    return m_arcs;
  }

  inline void addArc(BasicArc *arc)
  {
    m_arcs.push_back(arc);
  }

  inline void removeArc(const uint32_t nth)
  {
    m_arcs.erase(m_arcs.begin() + nth);
  }

  inline BasicArc *neighbor(const uint32_t i)
  {
    return m_arcs.at(i);
  }

  inline const BasicArc *neighbor(const uint32_t i) const
  {
    return m_arcs.at(i);
  }

  inline uint32_t degree() const
  {
    return static_cast<uint32_t>(m_arcs.size());
  }

protected:
  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;
  //!
  std::vector<BasicArc*> m_arcs;
};

inline std::ostream& operator<<(std::ostream& os, const BasicNode& n)
{
  os << "Node(" << n.id() << "){ ";
  uint32_t i = n.degree();
  while (i--)
    {
      os << n.neighbor(i)->id() << " ";
    }
  os << "}";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicNode* n)
{
  os << *n;
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicArc& e)
{
  os << "Arc(" << e.id() << "){" << e.from() << ", " << e.to() << "}";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicArc *e)
{
  os << *e;
  return os;
}

// *************************************************************************************************
//
// *************************************************************************************************
template <class N, class A> class BasicGraph
{
public:

  BasicGraph()
  {
  }

  virtual ~BasicGraph()
  {
    clearNodes();
    clearArcs();
  }

  // ---- NODES ------------------------------------------------------------------------------------

  void addNode(const Key nodeID)
  {
    if (!hasNode(nodeID))
      {
        N *node = newNode(nodeID);
        m_nodes.insert(node->id(), node);
      }
  }

  //! Add a node created not using the graph method
  void addNode(BasicNode const& n)
  {
    if (!hasNode(n.id()))
      {
        N *node = n.clone();
        m_nodes.insert(node->id(), node);
      }
  }

  inline bool hasNode(const Key nodeID) const
  {
    return m_nodes.isValidIndex(nodeID) && m_nodes.isOccupied(nodeID);
  }

  inline N* getNode(const Key nodeID) const
  {
    return m_nodes.get(nodeID);
  }

  inline void removeNode(const Key nodeID)
  {
    if (hasNode(nodeID))
      {
        N* node = m_nodes.get(nodeID); // FIXME direct access c'est ok
        assert(nullptr != node);

        const std::vector<A*> arcs = node->arcs();
        auto end = arcs.end();
        for (auto it = arcs.begin(); it != end; ++it)
          {
            removeArc((*it)->id());
          }

        delete node;
        m_nodes.remove(nodeID);
      }
  }

  inline Key howManyNodes() const
  {
    return m_nodes.occupation();
  }

  void clearNodes()
  {
    auto end = m_nodes.end();
    for (auto it = m_nodes.begin(); it != end; ++it)
      {
        //removeNode((*it)->id());
      }
    m_nodes.clear();
  }

  inline void debugNodes() const
  {
    std::cout << "List of nodes:" << std::endl;
    m_nodes.debug();
  }

  // ---- LINKS ------------------------------------------------------------------------------------

  void addArc(const Key fromID, const Key toID)
  {
    addNode(fromID);
    addNode(toID);
    private_addArc(getNode(fromID), getNode(toID));
  }

  void addArc(BasicNode const& fromNode,
              BasicNode const& toNode)
  {
    addNode(fromNode);
    addNode(toNode);
    private_addArc(fromNode, toNode);
  }

  inline bool hasArc(const Key arcID) const
  {
    return m_arcs.isValidIndex(arcID) && m_arcs.isOccupied(arcID);
  }

  inline A* getArc(const Key arcID) const
  {
    return m_arcs.get(arcID);
  }

  A* getArc(const Key fromNodeID, const Key toNodeID) const
  {
    N* node = m_nodes.get(fromNodeID);
    if (nullptr == node)
      return nullptr;

    uint32_t i = node->degree();
    while (i--)
      {
        A* arc = m_arcs.get(node->neighbor(i)->id());
        if (nullptr == arc)
          return nullptr;

        if ((arc->from()->id() == fromNodeID) &&
            (arc->to()->id() == toNodeID))
          {
            return arc;
          }
      }
    return nullptr;
  }

  // TBD: vector<Key> or vector<IArcs*>
  const std::vector<A*> *neighbors(const Key nodeID) const
  {
    N* node = getNode(nodeID);
    if (nullptr == node)
      return nullptr;
    return &node->arcs();
  }

  void removeArc(const Key arcID)
  {
    A *arc = getArc(arcID);
    if (nullptr != arc)
      {
        N *fromNode = getNode(arc->from()->id());
        N *toNode = getNode(arc->to()->id());

        if (nullptr != fromNode)
          {
            fromNode->removeArc(arcID);
          }

        if (nullptr != toNode)
          {
            toNode->removeArc(arcID);
          }

        delete arc;
        m_arcs.remove(arcID);
      }
  }

  Key howManyArcs() const
  {
    return m_arcs.occupation();
  }

  void clearArcs()
  {
    auto end = m_arcs.end();
    for (auto it = m_arcs.begin(); it != end; ++it)
      {
        //removeArc((*it)->id());
      }
  }

  inline void debugArcs() const
  {
    std::cout << "List of arcs:" << std::endl;
    m_arcs.debug();
  }

private:

  void private_addArc(BasicNode *fromNode, BasicNode *toNode)
  {
    assert(nullptr != fromNode);
    assert(nullptr != toNode);

    A *arc = newArc(fromNode, toNode);
    m_arcs.append(arc);
    fromNode->addArc(arc);
    if (fromNode->id() != toNode->id())
      {
        toNode->addArc(arc);
      }
  }

protected:

  N *newNode()
  {
    return new N();
  }

  N *newNode(const Key nodeID)
  {
    return new N(nodeID);
  }

  A *newArc(BasicNode *fromNode, BasicNode *toNode)
  {
    return new A(m_arc_unique++, fromNode, toNode);
  }

  container<N*> m_nodes;
  container<A*> m_arcs;
  Key m_arc_unique = 0;
};

#endif /* BASIC_GRAPH_HPP_ */

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
  BasicArc(const Key id, BasicNode& fromNode, BasicNode& toNode)
    : m_id(id),
      m_fromNode(&fromNode),
      m_toNode(&toNode)
  {
  }

  BasicArc(BasicArc const& arc)
    : m_id(arc.id()),
      m_fromNode(&arc.from()),
      m_toNode(&arc.to())
  {
  }

  BasicArc* clone() const
  {
    return new BasicArc(*this);
  }

  virtual ~BasicArc() {}

  //! \brief Return the unique identifier.
  inline Key id() const { return m_id; }
  inline BasicNode &from() const { return *m_fromNode; }
  inline void from(BasicNode& fromNode) { m_fromNode = &fromNode; }
  inline BasicNode &to() const { return *m_toNode; }
  inline void to(BasicNode& toNode) { m_toNode = &toNode; }

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
    : m_id(nodeID)
  {
    m_arcs.reserve(4);
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

  inline void addNeighbor(BasicArc *arc)
  {
    m_arcs.push_back(arc);
  }

  inline void removeNeighbor(const Key arcID)
  {
    struct isValue
    {
      Key m_id;
      isValue(const Key id) : m_id(id) {}
      bool operator()(const BasicArc *arc) const
      {
        return (arc->id() == m_id);
      }
    };
    auto end = m_arcs.end();
    auto it = std::find_if(m_arcs.begin(), end, isValue(arcID));
    if (it != end)
      {
        remove(it);
      }
  }

  inline void removeNthNeighbor(const uint32_t nth)
  {
    if (nth < m_arcs.size())
      {
        remove(m_arcs.begin() + nth);
      }
  }

  inline BasicArc *neighbor(const uint32_t i)
  {
    return m_arcs.at(i);
  }

  inline const BasicArc *neighbor(const uint32_t i) const
  {
    return m_arcs.at(i);
  }

  inline const std::vector<BasicArc*> &neighbors() const
  {
    return m_arcs;
  }

  inline uint32_t degree() const
  {
    return m_arcs.size();
  }

  inline bool operator==(const BasicNode& rhs) const
  {
    return m_id == rhs.m_id;
  }

  inline bool operator!=(const BasicNode& rhs) const
  {
    return !(*this == rhs);
  }

  inline bool operator==(const Key& rhs) const
  {
    return m_id == rhs;
  }

  inline bool operator!=(const Key& rhs) const
  {
    return !(*this == rhs);
  }

protected:
  //! \brief Swap element to be removed with the last element and then
  //! remove the item at the end of the caontainer prevent moving all
  //! items after the one removed.
  void remove(std::vector<BasicArc*>::iterator const& it)
  {
    *it = m_arcs.back();
    m_arcs.pop_back();
  }

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
    clearArcs();
    clearNodes();
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

        const std::vector<A*> arcs = node->neighbors();
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
        delete (*it);
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
    private_addArc(*getNode(fromID), *getNode(toID));
  }

  void addArc(BasicNode const& fromNode,
              BasicNode const& toNode)
  {
    addNode(fromNode);
    addNode(toNode);
    private_addArc(*getNode(fromNode.id()), *getNode(toNode.id()));
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

        if ((arc->from() == fromNodeID) && (arc->to() == toNodeID))
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
    return &node->neighbors();
  }

  void removeArc(const Key arcID)
  {
    if (hasArc(arcID))
      {
        A *arc = getArc(arcID);
        assert(nullptr != arc);

        arc->from().removeNeighbor(arcID);
        arc->to().removeNeighbor(arcID);

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
        delete (*it);
      }
    m_arcs.clear();
  }

  inline void debugArcs() const
  {
    std::cout << "List of arcs:" << std::endl;
    m_arcs.debug();
  }

private:

  void private_addArc(BasicNode &fromNode, BasicNode &toNode)
  {
    A *arc = newArc(fromNode, toNode);
    m_arcs.append(arc);
    fromNode.addNeighbor(arc);
    if (fromNode != toNode)
      {
        toNode.addNeighbor(arc);
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

  A *newArc(BasicNode &fromNode, BasicNode &toNode)
  {
    return new A(m_arc_unique++, fromNode, toNode);
  }

  container<N*> m_nodes;
  container<A*> m_arcs;
  Key m_arc_unique = 0;
};

#endif /* BASIC_GRAPH_HPP_ */

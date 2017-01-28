#ifndef BASIC_GRAPH_HPP_
#  define BASIC_GRAPH_HPP_

//! Special container
#include "GraphMemory.hpp"
//! Node and Arc unique identifier
#include "ClassCounter.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class BasicArc
{
public:
  BasicArc(const Key fromID, const Key toID)
    : m_fromNodeID(fromID), m_toNodeID(toID)
  {
  }

  virtual ~BasicArc() {}

  //! \brief Return the unique identifier.
  inline Key id() const { return m_id; }
  inline Key from() const { return m_fromNodeID; }
  inline void from(const Key fromID) { m_fromNodeID = fromID; }
  inline Key to() const { return m_toNodeID; }
  inline void to(const Key toID) { m_toNodeID = toID; }

protected:
  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;

  // TBD: Key or shareptr<BasicNode> ?
  Key m_fromNodeID;
  Key m_toNodeID;
};

inline std::ostream& operator<<(std::ostream& os, const BasicArc& e)
{
  os << "Edge(" << e.from() << ", " << e.to() << ")";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicArc *e)
{
  os << "Edge(" << e->from() << ", " << e->to() << ")";
  return os;
}

// *************************************************************************************************
//
// *************************************************************************************************
class BasicNode
{
public:
  BasicNode()
  {
    m_arcs.reserve(4);
  }

  BasicNode(const Key nodeID)
  {
    m_arcs.reserve(4);
    m_id = nodeID;
  }

  virtual ~BasicNode() {}

  //! \brief Return the unique identifier.
  inline Key id() const
  {
    return m_id;
  }

  inline const std::vector<Key> &arcs() const
  {
    return m_arcs;
  }

  inline void addArc(const Key arcID)
  {
    m_arcs.push_back(arcID);
  }

  inline void removeArc(const Key arcID)
  {
    m_arcs.erase(m_arcs.begin() + arcID);
  }

  inline Key at(const uint32_t i) const
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
  std::vector<Key> m_arcs;
};

inline std::ostream& operator<<(std::ostream& os, const BasicNode& n)
{
  os << "Node(" << n.id() << ")";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicNode* n)
{
  os << "Node(" << n->id() << ")";
  return os;
}

// *************************************************************************************************
//
// *************************************************************************************************
template <class N, class A> class BasicGraph
  : private ClassCounter<N>, private ClassCounter<A>
{
public:

  BasicGraph()
  {
  }

  virtual ~BasicGraph()
  {
    auto end = m_nodes.end();
    for (auto it = m_nodes.begin(); it != end; ++it)
      {
        removeNode((*it)->id());
      }
    for (auto it = m_arcs.begin(); it != m_arcs.end(); ++it)
      {
        removeArc((*it)->id());
      }
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

  inline bool hasNode(const Key nodeID) const
  {
    return m_nodes.isValidIndex(nodeID) && m_nodes.isOccupied(nodeID);
  }

  N* getNode(const Key nodeID) const // TODO shareptr
  {
    N* node = nullptr;

    if (hasNode(nodeID))
      {
        node = *(m_nodes.get(nodeID));
        assert(nullptr != node);
      }
    return node;
  }

  inline void removeNode(const Key nodeID)
  {
    if (hasNode(nodeID))
      {
        N* node = *(m_nodes.get(nodeID)); // FIXME degeux

        const std::vector<Key> &arcs = node->arcs();
        for (auto it = arcs.begin(); it != arcs.end(); ++it)
          {
            removeArc(*it);
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
    ClassCounter<N>::reset();
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

    A *arc = newArc(fromID, toID);
    N* fromNode = *(m_nodes.get(fromID));
    N* toNode = *(m_nodes.get(toID));

    assert(nullptr != fromNode);
    assert(nullptr != toNode);

    m_arcs.append(arc);
    fromNode->addArc(arc->id());
    if (fromID != toID)
      {
        toNode->addArc(arc->id());
      }
  }

  inline bool hasArc(const Key arcID) const
  {
    std::cout << "hasArc" << std::endl;
    return m_arcs.isOccupied(arcID);
  }

  A* getArc(const Key arcID) const
  {
    if (hasArc(arcID))
      {
        return *(m_arcs.get(arcID));
      }
    return nullptr;
  }

  A* getArc(const Key fromNodeID, const Key toNodeID) const
  {
    N *node = getNode(fromNodeID);

    if (nullptr == node)
      return nullptr;

    for (Key i = 0; i < node->degree(); ++i)
      {
        A *arc = getArc(node->at(i));
        assert(nullptr != arc);
        if ((arc->from() == fromNodeID) && (arc->to() == toNodeID))
          {
            return arc;
          }
      }
    return nullptr;
  }

  // TBD: vector<Key> or vector<IArcs*>
  const std::vector<Key> &getArcsfromNode(const Key nodeID) const
  {
    N* node = getNode(nodeID);
    if (nullptr == node)
      {
        // FIXME throw exception
      }
    return node->arcs();
  }

  void removeArc(const Key arcID)
  {
    std::cout << "removeArc" << std::endl;
    A *arc = getArc(arcID);
    if (nullptr != arc)
      {
        N *fromNode = getNode(arc->from());
        N *toNode = getNode(arc->to());

        if (nullptr != fromNode)
          {
            fromNode->removeArc(arcID);
          }

        if (nullptr != toNode)
          {
            toNode->removeArc(arcID);
          }
      }
  }

  Key howManyArcs() const
  {
    return m_arcs.occupation();
  }

  void clearArcs()
  {
    ClassCounter<A>::reset();
    m_arcs.clear();
  }

  inline void debugArcs() const
  {
    std::cout << "List of arcs:" << std::endl;
    m_arcs.debug();
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

  A *newArc(const Key fromID, const Key toID)
  {
    return new A(fromID, toID);
  }

  container<N*> m_nodes;
  container<A*> m_arcs;
};

#endif /* BASIC_GRAPH_HPP_ */

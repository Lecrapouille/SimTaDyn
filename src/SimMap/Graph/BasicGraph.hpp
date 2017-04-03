#ifndef BASIC_GRAPH_HPP_
#  define BASIC_GRAPH_HPP_

//! Special container
#  include "GraphContainer.tpp"
#  include "Types.hpp"
#  include <algorithm>

class BasicNode;
class BasicArc;

// *************************************************************************************************
//
// *************************************************************************************************
class BasicArc
{
public:
  //! \brief! Constructor.
  //! \param id the unique indentifier.
  //! \param fromNode the reference of an existing node for the tail.
  //! \param toNode the reference of an existing node for the head.
  BasicArc(const Key id, BasicNode& fromNode, BasicNode& toNode)
    : m_id(id),
      m_fromNode(&fromNode),
      m_toNode(&toNode)
  {
  }

  //! \brief Constructor by copy.
  //! \param arc the reference of an existing arc.
  BasicArc(BasicArc const& arc)
    : m_id(arc.id()),
      m_fromNode(&arc.from()),
      m_toNode(&arc.to())
  {
  }

  //! \brief Self copy. Used for the constructor by copy.
  BasicArc* clone() const
  {
    return new BasicArc(*this);
  }

  //! \brief Virtual destructor.
  virtual ~BasicArc() {}

  //! \brief Return the unique identifier.
  inline Key id() const { return m_id; }
  //! \brief Return the reference of the tail of the arc.
  inline BasicNode &from() const { return *m_fromNode; }
  //! \brief Change the tail of the arc.
  inline void from(BasicNode& fromNode) { m_fromNode = &fromNode; }
  //! \brief Return the reference of the head of the arc.
  inline BasicNode &to() const { return *m_toNode; }
  //! \brief Change the head of the arc.
  inline void to(BasicNode& toNode) { m_toNode = &toNode; }

protected:
  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;
  //! \brief the tail of the arc.
  BasicNode *m_fromNode;
  //! \brief the head of the arc.
  BasicNode *m_toNode;
};

// *************************************************************************************************
//
// *************************************************************************************************
class BasicNode
{
public:
  //! \brief Constructor.
  //! \param nodeID the unique indentifier.
  BasicNode(const Key nodeID)
    : m_id(nodeID)
  {
    LOGI("New BasicNode with ID %u\n", m_id);
    m_arcs.reserve(4);
  }

  //! \brief Constructor by copy.
  //! \param node the reference of an existing node.
  BasicNode(BasicNode const& node)
    : m_id(node.m_id), m_arcs(node.m_arcs)
  {
    LOGI("Copy BasicNode with ID %u\n", m_id);
    m_arcs.reserve(4);
  }

  //! \brief Self copy. Used for the constructor by copy.
  BasicNode* clone() const
  {
    return new BasicNode(*this);
  }

  //! \brief Virtual destructor.
  virtual ~BasicNode() {}

  //! \brief Return the unique identifier.
  inline Key id() const
  {
    return m_id;
  }

  //! \brief add a new node neighbor refered by its link
  inline void addNeighbor(BasicArc *arc)
  {
    LOGI("Node ID %u: Add the arc ID %u as neighbor\n", m_id, arc->id());
    m_arcs.push_back(arc);
  }

  //! \brief Remove the arc neighbor of a node refered by its unique
  //! identifier. If index is incorrect nothing is done and no error
  //! is returned. Complexity is O(n) where n is the number of
  //! neighbors.
  //! \param arcID the unique identifier of the arc to be removed.
  inline void removeNeighbor(const Key arcID)
  {
    LOGI("Node ID %u: Remove the arc ID %u as neighbor\n", m_id, arcID);

    // Temporary structure for comparing uniques identifiers.
    struct isValue
    {
      Key m_id;
      isValue(const Key id) : m_id(id) {}
      bool operator()(const BasicArc *arc) const
      {
        return (arc->id() == m_id);
      }
    };

    // Search and remove
    auto end = m_arcs.end();
    auto it = std::find_if(m_arcs.begin(), end, isValue(arcID));
    if (it != end)
      {
        remove(it);
      }
  }

  //! \brief Remove the nth arc neighbor of a node. No memory are
  //! released and do not do it before calling this function. If the
  //! index if incorrect no error is returned and nothing is
  //! done. Complexity is O(1).
  //! \param nth the nth element of the list of neighbor.
  inline void removeNthNeighbor(const uint32_t nth)
  {
    LOGI("Node ID %u: Remove its %u'nth ID neighbor\n", nth);
    if (nth < m_arcs.size())
      {
        remove(m_arcs.begin() + nth);
      }
  }

  //! \brief Return the address of the nth neighbor refered by its
  //! arc. Complexity is O(1).
  //! \param nth the nth element of the list of neighbor.
  //! \return the address of the arc, else nullptr if the index is
  //! incorrect.
  inline BasicArc *neighbor(const uint32_t nth)
  {
    if (nth < m_arcs.size())
      {
        return m_arcs.at(nth);
      }
    return nullptr;
  }

  //! \brief Return the address of the nth neighbor refered by its
  //! arc. Complexity is O(1).
  //! \param nth the nth element of the list of neighbor.
  //! \return the address of the arc, else nullptr if the index is
  //! incorrect.
  inline const BasicArc *neighbor(const uint32_t nth) const
  {
    if (nth < m_arcs.size())
      {
        return m_arcs.at(nth);
      }
    return nullptr;
  }

  //! \brief Return the list of neighbors.
  inline const std::vector<BasicArc*> &neighbors() const
  {
    return m_arcs;
  }

  //! \brief Return the number of neighbors. Complexity is O(1).
  inline uint32_t degree() const
  {
    return m_arcs.size();
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator==(const BasicNode& rhs) const
  {
    return m_id == rhs.m_id;
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator!=(const BasicNode& rhs) const
  {
    return !(*this == rhs);
  }

  //! \brief Compare the unique identifier of this node with a given
  // identifier.
  inline bool operator==(const Key& rhs) const
  {
    return m_id == rhs;
  }

  //! \brief Compare the unique identifier of this node with a given
  // identifier.
  inline bool operator!=(const Key& rhs) const
  {
    return !(*this == rhs);
  }

protected:
  //! \brief Swap element to be removed with the last element and then
  //! remove the item at the end of the container. This prevents
  //! moving all items after the one removed (complexity
  //! O(n)). Complexity is O(1).
  void remove(std::vector<BasicArc*>::iterator const& it)
  {
    *it = m_arcs.back();
    m_arcs.pop_back();
  }

  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;
  //! \brief the list of node neighbors refered by their arc starting
  //! from this node. We prefer sacrificed memory than reducing
  //! computations for looking for neighbors.
  std::vector<BasicArc*> m_arcs;
};

inline std::ostream& operator<<(std::ostream& os, const BasicNode& n)
{
  os << "Node " << (char) n.id() << ":" << std::endl;

  for (uint32_t i = 0; i < n.degree(); ++i)
    {
      os << "  --" << n.neighbor(i)->id()
         << "--> " << (char) n.neighbor(i)->to().id()
         << std::endl;
    }
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

  //! \brief Empty constructor. Reserve the memory with the
  //! default number of elements.
  BasicGraph(const bool directed = true)
    : m_directed(directed)
  {
  }

  //! \brief Constructor. Reserve memory for the given
  //! number of nodes and arcs.
  BasicGraph(const uint32_t noNodes,
             const uint32_t noArcs,
             const bool directed = true)
    : m_nodes(noNodes), m_arcs(noArcs), m_directed(directed)
  {
  }

  //! \brief Destructor. Release allocated nodes and arcs.
  virtual ~BasicGraph()
  {
    reset();
  }

  inline void reset()
  {
    clearArcs();
    clearNodes();
  }

  //! \brief Return if the graph has zero nodes.
  //! \return true if the graph is empty, else false.
  inline bool empty() const
  {
    return m_nodes.empty();
  }

  //! \brief
  inline N *addNode()
  {
    N *node = newNode(m_node_unique);
    m_nodes.insert(m_node_unique, node);
    ++m_node_unique;
    return node;
  }

  N *addAndGetNode(const Key nodeID)
  {
    N *node = getNode(nodeID);
    if (nullptr == node)
      {
        node = newNode(nodeID);
        m_nodes.insert(node->id(), node);
      }
    return node;
  }

  //! \brief Add a node to the graph with the given identifier.
  //! If a node with the same identifier already exists nothing
  //! is made; else a new node is allocated. Complexity is O(1).
  //! \param nodeID the unique identifier of the future node.
  void addNode(const Key nodeID)
  {
    if (!hasNode(nodeID))
      {
// FIXME eviter les new: changeNode(nodeID) { m_nodes[nodeID]. xx = .. } au lieu de new et insert
// FIXME par contre on perd le polymorphisme ancestre->xxx
        N *node = newNode(nodeID);
        m_nodes.insert(node->id(), node);
      }
  }

  //! \brief Add a node to the graph. This methode assumes that the
  //! client has already created the node and simply adds it to the
  //! graph. Complexity is O(1).
  //! \param node the reference of existing node.
  //! \param clone boolean to duplicate the node.
  void addNode(BasicNode& node, const bool clone=false)
  {
    if (!hasNode(node.id()))
      {
        if (clone)
          {
            N *n = node.clone();
            m_nodes.insert(n->id(), n);
          }
        else
          {
            m_nodes.insert(node.id(), &node);
          }
      }
  }

  //! \brief Return if the node exists in the graph. Complexity is
  //! O(1).
  //! \param nodeID the unique identifier of the node to test.
  inline bool hasNode(const Key nodeID) const
  {
    return m_nodes.isValidIndex(nodeID) &&
      m_nodes.isOccupied(nodeID);
  }

  //! \brief Return the address of the node given its unique
  //! identifier. Complexity is O(1).
  //! \param nodeID the unique identifier of the node to find.
  //! \return the address of the node if it exists else nullptr.
  inline N* getNode(const Key nodeID) const
  {
    return m_nodes.get(nodeID);
  }

  //! \brief Remove from the graph the node specified by its unique
  //! identifier. If the node does not exist no error is
  //! returned. Complexity is O(1) for removing the node itself and
  //! O(n) for removing neighbors informations.
  //! \param nodeID the unique identifier of the node to remove.
  inline void removeNode(const Key nodeID)
  {
    if (hasNode(nodeID))
      {
        N* node = m_nodes.get(nodeID); // FIXME direct access c'est ok
        assert(nullptr != node);

        const std::vector<A*> &neighbors = node->neighbors();
        auto end = neighbors.end();
        for (auto it = neighbors.begin(); it != end; ++it)
          {
            removeArc((*it)->id());
          }

        delete node;
        m_nodes.remove(nodeID);
      }
  }

  //! \brief Return the number of nodes constituing the
  //! graph. Complexity is O(1).
  inline uint32_t howManyNodes() const
  {
    return m_nodes.occupation();
  }

  //! \brief Remove all nodes from the graph. Complexity is O(n).
  void clearNodes()
  {
    auto end = m_nodes.end();
    for (auto it = m_nodes.begin(); it != end; ++it)
      {
        delete (*it);
      }
    m_nodes.clear();
  }

  //! \brief
  //! Complexity is O(n).
  inline void unmarkAll()
  {
    m_nodes.unmarkAll();
  }

  //! \brief
  //! Complexity is O(1).
  inline void mark(const Key nodeID)
  {
    m_nodes.mark(nodeID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void unmark(const Key nodeID)
  {
    m_nodes.unmark(nodeID);
  }

  //! \brief
  //! Note: no security is made if arcID is invalid
  //! Complexity is O(1).
  inline bool alreadyMarked(const Key nodeID)
  {
    return m_nodes.isMarked(nodeID);
  }

  //! \brief Pretty print all the nodes constituing the graph.
  //! Complexity is O(n).
  inline void debugNodes() const
  {
    std::cout << "List of nodes:" << std::endl;
    m_nodes.debug();
  }

  // ---- LINKS ------------------------------------------------------------------------------------

  //! \brief Create a link between two nodes. Nodes are refered by
  //! their unique identifier. If they do not exist, they are created.
  //! Complexity is O(1) to add the arc and also O(1) for nodes.
  //! \param fromID the unique identifier of the tail of the arc.
  //! \param toID the unique identifier of the head of the arc.
  void addArc(const Key fromID, const Key toID)
  {
    addNode(fromID);
    addNode(toID);
    private_addArc(*getNode(fromID), *getNode(toID));
  }

  //! \brief Create a link between two nodes in the graph. This
  //! methode assumes that the client has already created nodes and
  //! simply adds them to the graph if they don't exist. Complexity is
  //! O(1) both for adding the arc and nodes.
  //! \param fromNode the reference of the node that will be used for
  //! the tail of the arc.
  //! \param toNode the reference of the node that will be used for
  //! the head of the arc.
  void addArc(BasicNode const& fromNode,
              BasicNode const& toNode)
  {
    addNode(fromNode);
    addNode(toNode);
    private_addArc(*getNode(fromNode.id()), *getNode(toNode.id()));
  }

  //! \brief Return if the arc refered by the given unique identifer
  //! exists in the graph. Complexity is O(1).
  //! \param arcID the unique identifier of the arc to look for.
  //! \return true if the arc exists, else false.
  inline bool hasArc(const Key arcID) const
  {
    return m_arcs.isValidIndex(arcID) && m_arcs.isOccupied(arcID);
  }

  //! \brief Return the arc refered by the given unique identifer.
  //! \return the address of the arc class if it was found in the
  //! graph, else return nullptr.
  inline A* getArc(const Key arcID) const
  {
    return m_arcs.get(arcID);
  }

  //! \brief Return the arc refered by the given unique identifer of
  //! its nodes. Complexity is O(1) for finding the tail node and O(n)
  //! for the head where n is the number of neighbors of the tail
  //! node (the degree of a node).
  //! \param fromNodeID the unique identifier of the node serving of tail.
  //! \param toNodeID the unique identifier of the node serving of head.
  //! \return the address of the arc class if it was found in the
  //! graph, else return nullptr.
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

  //! Return the list of neighbors of the node refered by its unique
  //! identifier. Neighbors are giving by a vector of arcs and not by a
  //! vector of nodes. Complexity is O(1).
  //! \param nodeID the unique identifier of the node to look for.
  //! \return the address of the vector else return nullptr.
  const std::vector<A*> *neighbors(const Key nodeID) const
  {
    N* node = getNode(nodeID);
    if (nullptr == node)
      return nullptr;
    return &node->neighbors();
  }

  //! Remove an arc from the graph refered by its unique identifeir.
  //! Memory is released. Complexity is O(1).
  //! \param arcID the unique identifier of the arc to be removed.
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

  //! \brief Return the number of arcs constituing the
  //! graph. Complexity is O(1).
  Key howManyArcs() const
  {
    return m_arcs.occupation();
  }

  //! \brief Remove all arcss from the graph. Complexity is O(n).
  void clearArcs()
  {
    auto end = m_arcs.end();
    for (auto it = m_arcs.begin(); it != end; ++it)
      {
        delete (*it);
      }
    m_arcs.clear();
  }

  //! \brief
  //! Complexity is O(n).
  inline void unmarkAllVisitedArcs()
  {
    m_arcs.unmarkAll();
  }

  //! \brief
  //! Note: no security is made if arcID is invalid
  //! Complexity is O(1).
  inline bool arcHasBeenVisited(const Key arcID)
  {
    return m_arcs.isMarked(arcID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void markVisitedArc(const Key arcID)
  {
    m_arcs.mark(arcID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void unmarkVisitedkArc(const Key arcID)
  {
    m_arcs.unmark(arcID);
  }

  //! \brief Pretty print all the nodes constituing the graph.
  //! Complexity is O(n).
  inline void debugArcs() const
  {
    std::cout << "List of arcs:" << std::endl;
    m_arcs.debug();
  }

private:

  //! Shared function by two public fucntions.
  void private_addArc(BasicNode &fromNode, BasicNode &toNode)
  {
    A *arc = newArc(fromNode, toNode);
    m_arcs.append(arc);
    fromNode.addNeighbor(arc);

    if ((!m_directed) && (fromNode != toNode))
      {
        arc = newArc(toNode, fromNode);
        m_arcs.append(arc);
        toNode.addNeighbor(arc);
      }
  }

protected:

  //! \brief Common function for allocating a new node.
  inline virtual N *newNode(const Key nodeID)
  {
    return new N(nodeID);
  }

  //! \brief Common function for allocating a new arc.
  inline virtual A *newArc(BasicNode &fromNode, BasicNode &toNode)
  {
    return new A(m_arc_unique++, fromNode, toNode);
  }

  //! \brief the list of nodes constituing the graph.
  GraphContainer<N*, simtadyn::graph_container_nb_elements, GraphBlock> m_nodes; // FIXME shared_ptr<N> ? //FIXME: virer le * du N
  //! \brief the list of arcs constituing the graph.
  GraphContainer<A*, simtadyn::graph_container_nb_elements, GraphBlock> m_arcs;
  //! \brief direct or not direct graph ?
  bool m_directed;
  //! \brief create a unique identifier for arcs.
  Key m_arc_unique = 0;
  //! \brief create a unique identifier for nodes.
  Key m_node_unique = 0;
};

#endif /* BASIC_GRAPH_HPP_ */

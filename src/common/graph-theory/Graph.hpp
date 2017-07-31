#ifndef BASIC_GRAPH_HPP_
#  define BASIC_GRAPH_HPP_

//! Special container
#  include "Logger.hpp"
#  include "ClassCounter.hpp"
#  include "GraphContainer.tpp"
#  include <cstdint>
#  include <algorithm>

class BasicNode;
class BasicArc;

typedef unsigned int Key;

// *************************************************************************************************
//
// *************************************************************************************************
class BasicArc: private ClassCounter<BasicArc>
{
public:

  BasicArc();

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
    : ClassCounter<BasicArc>(),
      m_id(arc.id()),
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

  //! \brief Return the unique identifier.
  operator int() { return m_id; }

  //! \brief Return the reference of the tail of the arc.
  inline BasicNode &from() const { return *m_fromNode; }

  //! \brief Change the tail of the arc.
  void from(BasicNode& fromNode);

  //! \brief Return the reference of the head of the arc.
  inline BasicNode &to() const { return *m_toNode; }

  //! \brief Change the head of the arc.
  void to(BasicNode& toNode);

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator==(const BasicArc& rhs) const
  {
    return (m_id == rhs.m_id) && (m_fromNode == rhs.m_fromNode) &&
      (m_toNode == rhs.m_toNode);
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator!=(const BasicArc& rhs) const
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

  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;
  //! \brief the tail of the arc.
  BasicNode* m_fromNode;
  //! \brief the head of the arc.
  BasicNode* m_toNode;
};

// *************************************************************************************************
//
// *************************************************************************************************
class BasicNode: private ClassCounter<BasicNode>
{
public:

  BasicNode();

  //! \brief Constructor.
  //! \param nodeID the unique indentifier.
  BasicNode(const Key nodeID)
    : m_id(nodeID)
  {
    //LOGIS("New BasicNode with ID %u\n", m_id);
    m_arcs.reserve(4);
  }

  //! \brief Constructor by copy.
  //! \param node the reference of an existing node.
  BasicNode(BasicNode const& node)
    : ClassCounter<BasicNode>(),
      m_id(node.m_id),
      m_arcs(node.m_arcs)
  {
    //LOGIS("Copy BasicNode with ID %u\n", m_id);
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
  inline Key id() const { return m_id; }
  //! \brief Return the unique identifier.
  operator int() { return m_id; }

  //! \brief add a new node neighbor refered by its link
  inline void addNeighbor(BasicArc& arc)
  {
    //LOGI("Node ID %u: Add the arc ID %u as neighbor\n", m_id, arc);
    m_arcs.push_back(&arc);
  }

  //! \brief Remove all neighbors
  inline void removeAllNeighbors()
  {
    m_arcs.clear();
  }

  //! \brief Remove the arc neighbor of a node refered by its unique
  //! identifier. If index is incorrect nothing is done and no error
  //! is returned. Complexity is O(n) where n is the number of
  //! neighbors.
  //! \param arcID the unique identifier of the arc to be removed.
  inline void removeNeighbor(const Key arcID)
  {
    LOGIS("Node ID %u: Remove the arc ID %u as neighbor\n", m_id, arcID);

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
    LOGIS("Node ID %u: Remove its %u'nth ID neighbor\n", nth);
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

  inline const BasicArc &nthNeighbor(const uint32_t nth) const
  {
    return *(m_arcs[nth]);
  }

  inline BasicArc &nthNeighbor(const uint32_t nth)
  {
    return *(m_arcs[nth]);
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
  os << "Node " << n.id();

  uint32_t deg = n.degree();
  if (0 == deg)
    return os;

  os << " { ";
  for (uint32_t i = 0; i < deg; ++i)
    {
      os << n.nthNeighbor(i)/*.to()*/.id() << ' ';
    }
  os << '}';
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicNode* n)
{
  os << *n;
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicArc& e)
{
  os << "Arc " << e.id() << ": From" << e.from() << ", To" << e.to();
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

  typedef GraphContainer<N, config::graph_container_nb_elements, GraphBlock> blocknodes_t;
  typedef GraphContainer<A, config::graph_container_nb_elements, GraphBlock> blockarcs_t; // FIXME should be a Set not a Collection

  //! \brief Empty constructor. Reserve the memory with the
  //! default number of elements.
  BasicGraph(const bool directed = true)
    : m_directed(directed)
  {
    ClassCounter<BasicNode>::reset();
    ClassCounter<BasicArc>::reset();
  }

  BasicGraph(const char* name, const bool directed = true)
    : m_directed(directed), m_name(name)
  {
    ClassCounter<BasicNode>::reset();
    ClassCounter<BasicArc>::reset();
  }

  BasicGraph(std::string const& name, const bool directed = true)
    : m_directed(directed), m_name(name)
  {
    ClassCounter<BasicNode>::reset();
    ClassCounter<BasicArc>::reset();
  }

  //! \brief Constructor. Reserve memory for the given
  //! number of nodes and arcs.
  BasicGraph(const uint32_t noNodes,
             const uint32_t noArcs,
             const bool directed = true)
    : m_nodes(noNodes), m_arcs(noArcs), m_directed(directed)
  {
    ClassCounter<BasicNode>::reset();
    ClassCounter<BasicArc>::reset();
  }

  BasicGraph(std::string const& name,
             const uint32_t noNodes,
             const uint32_t noArcs,
             const bool directed = true)
    : m_nodes(noNodes), m_arcs(noArcs), m_directed(directed), m_name(name)
  {
    ClassCounter<BasicNode>::reset();
    ClassCounter<BasicArc>::reset();
  }

  BasicGraph(const char* name,
             const uint32_t noNodes,
             const uint32_t noArcs,
             const bool directed = true)
    : m_nodes(noNodes), m_arcs(noArcs), m_directed(directed), m_name(name)
  {
    ClassCounter<BasicNode>::reset();
    ClassCounter<BasicArc>::reset();
  }

  //! \brief Destructor. Release allocated nodes and arcs.
  virtual ~BasicGraph()
  {
    reset();
  }

  inline void reset()
  {
    m_nodes.clear();
    m_arcs.clear();
  }

  void garbage()
  {
    m_nodes.garbage();
    m_arcs.garbage();
  }

  inline bool directed() const
  {
    return m_directed;
  }

  //! \brief Return if the graph has zero nodes.
  //! \return true if the graph is empty, else false.
  inline bool empty() const
  {
    return m_nodes.empty();
  }

  //! \brief
  N& addNode()
  {
    const uint32_t last = m_nodes.last() + 1U;
    m_nodes.insert(last);
    return getNode(last);
  }

  N& addNode(BasicNode const& nodeID)
  {
    m_nodes.insert(nodeID.id(), nodeID);
    return getNode(nodeID.id());
  }

  N& addNode(const Key nodeID)
  {
    if (m_nodes.outofbound(nodeID))
      {
        // call BasicNode::BasicNode(Key)
        m_nodes.insert(nodeID, nodeID);
      }
    else
      {
        m_nodes.occupy(nodeID);
      }
    return getNode(nodeID);
  }

  //! \brief Return if the node exists in the graph. Complexity is
  //! O(1).
  //! \param nodeID the unique identifier of the node to test.
  inline bool hasNode(const Key nodeID) const
  {
    if (m_nodes.outofbound(nodeID))
      return false;
    return m_nodes.occupied(nodeID);
  }

  //! \brief Return the address of the node given its unique
  //! identifier. Complexity is O(1).
  //! \param nodeID the unique identifier of the node to find.
  //! \return the address of the node if it exists else nullptr.
  inline const N& getNode(const Key nodeID) const
  {
    return m_nodes[nodeID];
  }

  inline N& getNode(const Key nodeID)
  {
    return m_nodes[nodeID];
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
        m_nodes[nodeID].removeAllNeighbors();
        m_nodes.remove(nodeID);
      }
  }

  //! \brief Return the number of nodes constituing the
  //! graph. Complexity is O(1).
  inline uint32_t howManyNodes() const
  {
    return m_nodes.used();
  }

  //! \brief
  //! Complexity is O(n).
  inline void unmarkAllNodes()
  {
    m_nodes.unmarkAll();
  }

  //! \brief
  //! Complexity is O(1).
  inline void markNode(const Key nodeID)
  {
    m_nodes.mark(nodeID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void unmarkNode(const Key nodeID)
  {
    m_nodes.unmark(nodeID);
  }

  //! \brief
  //! Note: no security is made if arcID is invalid
  //! Complexity is O(1).
  inline bool markedNode(const Key nodeID)
  {
    return m_nodes.marked(nodeID);
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
    std::cout << "AddArc " << fromID << " --> " << toID << "\n";
    addNode(fromID);
    addNode(toID);
    private_addArc(getNode(fromID), getNode(toID));
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
    std::cout << "AddArc " << fromNode << " --> " << toNode << "\n";
    addNode(fromNode);
    addNode(toNode);
    private_addArc(getNode(fromNode.id()), getNode(toNode.id()));
  }

  //! \brief Return if the arc refered by the given unique identifer
  //! exists in the graph. Complexity is O(1).
  //! \param arcID the unique identifier of the arc to look for.
  //! \return true if the arc exists, else false.
  inline bool hasArc(const Key arcID) const
  {
    if (m_arcs.outofbound(arcID))
      return false;
    return m_arcs.occupied(arcID);
  }

  //! \brief
  bool hasArc(const Key fromNodeID, const Key toNodeID) const
  {
    if (!hasNode(fromNodeID))
      return false;

    N const& node = m_nodes[fromNodeID];
    uint32_t i = node.degree();
    while (i--)
      {
        A const& arc = m_arcs[node.neighbor(i)->id()];
        if ((arc.from() == fromNodeID) && (arc.to() == toNodeID))
          {
            return true;
          }
      }
    return false;
  }

  //! \brief Return the arc refered by the given unique identifer.
  //! No safe guards are applied so call the method hasArc() to be
  //! sure that arcID exists.
  //! \return the address of the arc class if it was found in the
  //! graph, else return nullptr.
  inline const A& getArc(const Key arcID) const
  {
    return m_arcs.get(arcID);
  }

  inline A& getArc(const Key arcID)
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
  A* getArc(const Key fromNodeID, const Key toNodeID)
  {
    if (!hasNode(fromNodeID))
      return nullptr;

    N const& node = m_nodes[fromNodeID];
    uint32_t i = node.degree();
    while (i--)
      {
        A& arc = m_arcs[node.neighbor(i)->id()];
        if ((arc.from() == fromNodeID) && (arc.to() == toNodeID))
          {
            return &arc;
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
    if (!hasNode(nodeID))
      return nullptr;

    N const& node = getNode(nodeID);
    return &node.neighbors();
  }

  //! Remove an arc from the graph refered by its unique identifeir.
  //! Memory is released. Complexity is O(1).
  //! \param arcID the unique identifier of the arc to be removed.
  void removeArc(const Key arcID)
  {
    if (hasArc(arcID))
      {
        A& arc = m_arcs[arcID];
        arc.from().removeNeighbor(arcID);
        arc.to().removeNeighbor(arcID);
        m_arcs.remove(arcID);
      }
  }

  //! \brief Return the number of arcs constituing the
  //! graph. Complexity is O(1).
  Key howManyArcs() const
  {
    return m_arcs.used();
  }

  //! \brief
  //! Complexity is O(n).
  inline void unmarkAllArcs()
  {
    m_arcs.unmarkAll();
  }

  //! \brief
  //! Note: no security is made if arcID is invalid
  //! Complexity is O(1).
  inline bool markedArc(const Key arcID)
  {
    return m_arcs.marked(arcID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void markArc(const Key arcID)
  {
    m_arcs.mark(arcID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void unmarkArc(const Key arcID)
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

  inline blockarcs_t& arcs()//FIXME const
  {
    return m_arcs;
  }

  inline blocknodes_t& nodes()//FIXME const
  {
    return m_nodes;
  }

private:

  //! Shared function by two public functions.
  void private_addArc(BasicNode &fromNode, BasicNode &toNode)
  {
    const uint32_t last = m_arcs.last() + 1U;// FIXME degeux car Collection est non pas Set
    m_arcs.insert(A(last, fromNode, toNode)); // FIXME degeux: faire Set.append
    A& arc = m_arcs.get(last);// FIXME degeux

    fromNode.addNeighbor(arc);
    if ((!m_directed) && (fromNode != toNode))
      {
        toNode.addNeighbor(arc); // FIXME: il faudrait inverser l'arc mais en n'ajoutant pas un nouveau arc.
                                 // FIXME: car on perd le fait de savoir qui vaut to et qui vaut from
                                 // FIXME: ducoup depthFirstSearch a efet de bord: les noeuds tentent de revenir vers eux memes
      }
  }

protected:

  //! \brief the list of nodes constituing the graph.
  blocknodes_t m_nodes;
  //! \brief the list of arcs constituing the graph.
  blockarcs_t m_arcs;
  //! \brief direct or not direct graph ?
  bool m_directed;

public:

  std::string m_name;
};

typedef BasicGraph<BasicNode, BasicArc> BasicGraph_t;

/*inline std::ostream& operator<<(std::ostream& os, const BasicGraph<N,A>& g)
{
  os << "List of nodes:" <<  std::endl << g.m_nodes << std::endl
     << "List of arcs:" <<  std::endl << g.m_arcs << std::endl;
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const BasicGraph<N,A>* g)
{
  os << *g;
  return os;
  }*/

#endif /* BASIC_GRAPH_HPP_ */

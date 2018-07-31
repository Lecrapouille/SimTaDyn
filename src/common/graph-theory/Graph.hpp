//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef CLASSIC_GRAPH_HPP_
#  define CLASSIC_GRAPH_HPP_

#  include "Logger.hpp"
#  include "ClassCounter.tpp"
#  include "GraphContainer.tpp"
#  include "Config.hpp"
#  include <algorithm>

namespace graphtheory
{

  enum /*class*/ GraphElementId : uint32_t
    {
      ANCESTOR     = 0x00000000,
      NODE         = 0x10000000,
      ARC          = 0x20000000,
      ZONE         = 0x40000000,
      MASK         = 0xf0000000,
    };

  class Arc;
  class Node;

// *************************************************************************************************
//
// *************************************************************************************************
class GraphElement
{
public:

  GraphElement()
  {
  }

  GraphElement(Key const id)
    : m_id(id)
  {
  }

  virtual ~GraphElement()
  {
  }

  //! \brief Return the unique identifier.
  inline virtual Key id() const { return m_id; }

  //! \brief Return the unique identifier.
  operator size_t() { return m_id; }

  //! \brief Return the unique type.
  inline virtual GraphElementId type() const
  {
    return GraphElementId::ANCESTOR;
  }

  /*std::vector<GraphElement*>& neighbors()
  {
    return m_neighbors;
    }*/

  const std::vector<const GraphElement*>& neighbors() const
  {
    return m_neighbors;
  }

  //! \brief Return the address of the nth neighbor refered by its
  //! elt. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  //! \return the address of the elt, else nullptr if the index is
  //! incorrect.
  /*inline GraphElement *neighbor(const size_t nth)
  {
    if (nth < m_neighbors.size())
      {
        return m_neighbors.at(nth);
      }
    return nullptr;
    }*/

  //! \brief Return the address of the nth neighbor refered by its
  //! elt. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  //! \return the address of the elt, else nullptr if the index is
  //! incorrect.
  inline const GraphElement *neighbor(const size_t nth) const
  {
    if (nth < m_neighbors.size())
      {
        return m_neighbors.at(nth);
      }
    return nullptr;
  }

  inline const GraphElement &nthNeighbor(const size_t nth) const
  {
    return *(m_neighbors[nth]);
  }

  /*inline GraphElement &nthNeighbor(const size_t nth)
  {
    return *(m_neighbors[nth]);
    }*/

  inline size_t howManyNeighbors() const
  {
    return m_neighbors.size();
  }

  /*std::vector<GraphElement*>& borders()
  {
    return m_borders;
    }*/

  const std::vector<const GraphElement*>& borders() const
  {
    return m_borders;
  }

    //! \brief Return the address of the nth neighbor refered by its
  //! elt. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  //! \return the address of the elt, else nullptr if the index is
  //! incorrect.
  /*inline GraphElement *border(const size_t nth)
  {
    if (nth < m_borders.size())
      {
        return m_borders.at(nth);
      }
    return nullptr;
    }*/

  //! \brief Return the address of the nth neighbor refered by its
  //! elt. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  //! \return the address of the elt, else nullptr if the index is
  //! incorrect.
  inline const GraphElement *border(const size_t nth) const
  {
    if (nth < m_borders.size())
      {
        return m_borders.at(nth);
      }
    return nullptr;
  }

  inline const GraphElement &nthBorder(const size_t nth) const
  {
    return *(m_borders[nth]);
  }

  /*inline GraphElement &nthBorder(const size_t nth)
  {
    return *(m_borders[nth]);
    }*/

  inline size_t howManyBorders() const
  {
    return m_borders.size();
  }

protected:

  std::vector<const GraphElement*> m_neighbors;
  std::vector<const GraphElement*> m_borders;

  //! \brief Make an instance unique with this identifier.
  //! Used it for comparing element in a container.
  Key m_id;
};

// *************************************************************************************************
//
// *************************************************************************************************
class GraphElementWithNeighbors: public GraphElement
{
public:

  GraphElementWithNeighbors()
    : GraphElement()
  {
  }

  GraphElementWithNeighbors(const Key id)
    : GraphElement(id)
  {
  }

  virtual ~GraphElementWithNeighbors() {}

  //! \brief add a new node neighbor refered by its link
  inline void addNeighbor(GraphElement& elt)
  {
    //LOGI("Node ID %u: Add the elt ID %u as neighbor\n", m_id, elt);
    m_neighbors.push_back(&elt);
  }

  //! \brief Remove all neighbors
  inline void removeAllNeighbors()
  {
    m_neighbors.clear();
  }

  //! \brief Remove the elt neighbor of a node refered by its unique
  //! identifier. If index is incorrect nothing is done and no error
  //! is returned. Complexity is O(n) where n is the number of
  //! neighbors.
  //! \param eltID the unique identifier of the elt to be removed.
  inline void removeNeighbor(const Key eltID)
  {
    LOGIS("Node ID %u: Remove the elt ID %u as neighbor\n", m_id, eltID);

    // Temporary structure for comparing uniques identifiers.
    struct isValue
    {
      Key m_id;
      isValue(const Key id) : m_id(id) {}
      bool operator()(const GraphElement *elt) const
      {
        return (elt->id() == m_id);
      }
    };

    // Seelth and remove
    std::vector<const GraphElement*>::iterator const& end
      = m_neighbors.end();
    std::vector<const GraphElement*>::iterator const& it
      = std::find_if(m_neighbors.begin(), end, isValue(eltID));
    if (it != end)
      {
        privateRemoveNeighbors(it);
      }
  }

  //! \brief Remove the nth elt neighbor of a node. No memory are
  //! released and do not do it before calling this function. If the
  //! index if incorrect no error is returned and nothing is
  //! done. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  inline void removeNthNeighbor(const size_t nth)
  {
    LOGIS("Node ID %u: Remove its %u'nth ID neighbor\n", m_id, nth);
    if (nth < m_neighbors.size())
      {
        // FIXME: enlever cast
        privateRemoveNeighbors(m_neighbors.begin() + static_cast<int>(nth));
      }
  }

private:

  //! \brief Swap element to be removed with the last element and then
  //! remove the item at the end of the container. This prevents
  //! moving all items after the one removed (complexity
  //! O(n)). Complexity is O(1).
  void privateRemoveNeighbors(std::vector<const GraphElement*>::iterator const& it)
  {
    *it = m_neighbors.back();
    m_neighbors.pop_back();
  }
};

// *************************************************************************************************
//
// *************************************************************************************************
class GraphElementWithBorders: public GraphElement
{
public:

  GraphElementWithBorders()
    : GraphElement()
  {
  }

  GraphElementWithBorders(const Key id)
    : GraphElement(id)
  {
  }

  virtual ~GraphElementWithBorders() {}

  //! \brief add a new node neighbor refered by its link
  inline void addBorder(GraphElement& elt)
  {
    //LOGI("Node ID %u: Add the elt ID %u as neighbor\n", m_id, elt);
    m_borders.push_back(&elt);
  }

  //! \brief Remove all borders
  inline void removeAllBorders()
  {
    m_borders.clear();
  }

  //! \brief Remove the elt neighbor of a node refered by its unique
  //! identifier. If index is incorrect nothing is done and no error
  //! is returned. Complexity is O(n) where n is the number of
  //! borders.
  //! \param eltID the unique identifier of the elt to be removed.
  inline void removeBorder(const Key eltID)
  {
    LOGIS("Node ID %u: Remove the elt ID %u as neighbor\n", m_id, eltID);

    // Temporary structure for comparing uniques identifiers.
    struct isValue
    {
      Key m_id;
      isValue(const Key id) : m_id(id) {}
      bool operator()(const GraphElement *elt) const
      {
        return (elt->id() == m_id);
      }
    };

    // Seelth and remove
    std::vector<const GraphElement*>::iterator const& end
      = m_borders.end();
    std::vector<const GraphElement*>::iterator const& it
      = std::find_if(m_borders.begin(), end, isValue(eltID));
    if (it != end)
      {
        privateRemoveBorders(it);
      }
  }

  //! \brief Remove the nth elt neighbor of a node. No memory are
  //! released and do not do it before calling this function. If the
  //! index if incorrect no error is returned and nothing is
  //! done. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  inline void removeNthBorder(const size_t nth)
  {
    LOGIS("Node ID %u: Remove its %u'nth ID neighbor\n", m_id, nth);
    if (nth < m_borders.size())
      {
        // FIXME: enlever cast
        privateRemoveBorders(m_borders.begin() + static_cast<int>(nth));
      }
  }

private:

  //! \brief Swap element to be removed with the last element and then
  //! remove the item at the end of the container. This prevents
  //! moving all items after the one removed (complexity
  //! O(n)). Complexity is O(1).
  void privateRemoveBorders(std::vector<const GraphElement*>::iterator const& it)
  {
    *it = m_borders.back();
    m_borders.pop_back();
  }
};


// *************************************************************************************************
//
// *************************************************************************************************
class Node
  : private UniqueID<Node>,
    public GraphElementWithNeighbors
{
public:

  Node();

  //! \brief Constructor.
  //! \param nodeID the unique indentifier.
  Node(const Key nodeID)
    : GraphElementWithNeighbors(nodeID)
  {
    //LOGIS("New Node with ID %u\n", m_id);
  }

  //! \brief Constructor by copy.
  //! \param node the reference of an existing node.
  Node(Node const& other)
    : UniqueID<Node>(),
      GraphElementWithNeighbors(other.m_id)
  {
    //LOGIS("Copy Node with ID %u\n", m_id);
    m_neighbors = other.m_neighbors;
  }

  //! \brief Constructor by move
  Node(Node&& other)
  {
    m_id = other.m_id;
    m_neighbors = other.m_neighbors;
  }

  //! \brief Virtual destructor.
  virtual ~Node()
  {
    //std::cout << "~Node()" << std::endl;
  }

  //! \brief Self copy. Used for the constructor by copy.
  Node* clone() const
  {
    return new Node(*this);
  }

  // Copy assignment operator.
  Node& operator=(const Node& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        m_neighbors = other.m_neighbors;
      }
    return *this;
  }

  // Move assignment operator.
  Node& operator=(Node&& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        m_neighbors = other.m_neighbors;
      }
    return *this;
  }

  //! \brief Return the unique type.
  inline virtual GraphElementId type() const
  {
    return GraphElementId::NODE;
  }

  //! \brief Return the unique name.
  inline const std::string name() const
  {
    return "N" + std::to_string(id());
  }

  //! \brief Return the number of neighbors. Complexity is O(1).
  inline size_t degree() const
  {
    return m_neighbors.size();
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator==(const Node& rhs) const
  {
    return m_id == rhs.m_id;
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator!=(const Node& rhs) const
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
};

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class Arc
  : private UniqueID<Arc>,
    public GraphElement
{
public:

  enum ArcDirection { From, To };

  Arc();

  //! \brief Constructor.
  //! \param id the unique indentifier.
  //! \param fromNode the reference of an existing node for the tail.
  //! \param toNode the reference of an existing node for the head.
  Arc(const Key id, Node& fromNode, Node& toNode)
    : GraphElement(id)
  {
    m_borders[From] = &fromNode;
    m_borders[To] = &toNode;
  }

  //! \brief Constructor by copy.
  //! \param arc the reference of an existing arc.
  Arc(Arc const& arc)
    : UniqueID<Arc>(),
      GraphElement(arc.id())
  {
    m_borders[From] = &(arc.from());
    m_borders[To] = &(arc.to());
  }

  //! \brief Constructor by move
  Arc(Arc&& other)
  {
    m_id = other.m_id;
    m_borders = other.m_borders;
  }

  //! \brief Virtual destructor.
  virtual ~Arc() {}

  //! \brief Self copy. Used for the constructor by copy.
  Arc* clone() const
  {
    return new Arc(*this);
  }

  // Copy assignment operator.
  Arc& operator=(const Arc& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        m_borders = other.m_borders;
      }
    return *this;
  }

  // Move assignment operator.
  Arc& operator=(Arc&& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        m_borders = other.m_borders;
      }
    return *this;
  }

  //! \brief Return the unique type.
  inline virtual GraphElementId type() const
  {
    return GraphElementId::ARC;
  }

  //! \brief Return the unique name.
  inline virtual const std::string name() const
  {
    return "A" + std::to_string(id());
  }

  //! \brief Return the reference of the tail of the arc.
  inline const Node& from() const
  {
    return *(static_cast<const Node*>(m_neighbors[From]));
  }

  //! \brief Change the tail of the arc.
  inline void from(Node& fromNode)
  {
    m_neighbors[From] = &fromNode;
  }

  //! \brief Return the reference of the head of the arc.
  inline const Node& to() const
  {
    return *(static_cast<const Node*>(m_neighbors[To]));
  }

  //! \brief Change the head of the arc.
  inline void to(Node& toNode)
  {
    m_neighbors[To] = &toNode;
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator==(const Arc& rhs) const
  {
    return (id() == rhs.id()) /*&& (m_neighbors[From] == rhs.m_neighbors[From]) &&
                                (m_neighbors[To] == rhs.m_neighbors[To])*/;
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator!=(const Arc& rhs) const
  {
    return !(*this == rhs);
  }

  //! \brief Compare the unique identifier of this node with a given
  // identifier.
  inline bool operator==(const Key& rhs) const
  {
    return id() == rhs;
  }

  //! \brief Compare the unique identifier of this node with a given
  // identifier.
  inline bool operator!=(const Key& rhs) const
  {
    return !(*this == rhs);
  }
};

inline std::ostream& operator<<(std::ostream& os, const Node& n)
{
  os << "Node " << n.id();

  size_t deg = n.degree();
  if (0 == deg)
    return os;

  os << " { ";
  for (size_t i = 0; i < deg; ++i)
    {
      os << n.nthNeighbor(i)/*.to()*/.id() << ' ';
    }
  os << '}';
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Node* n)
{
  os << *n;
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Arc& e)
{
  os << "Arc " << e.id() << ": From" << e.from() << ", To" << e.to();
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Arc *e)
{
  os << *e;
  return os;
}

// *************************************************************************************************
//! \brief Declare a classic graph where N referes to a Node class and A to an Arc class.
// *************************************************************************************************
template <class Node, class Arc>
class Graph
{
public:

  typedef GraphContainer<Node, config::graph_container_nb_elements, GraphBlock> blocknodes_t;
  typedef GraphContainer<Arc, config::graph_container_nb_elements, GraphBlock> blockarcs_t; // FIXME should be a Set not a Collection

  //! \brief Empty constructor. Reserve the memory with the
  //! default number of elements.
  Graph(const bool directed = true)
    : m_directed(directed)
  {
    UniqueID<Node>::resetID();
    UniqueID<Arc>::resetID();
  }

  /*Graph(const char* name, const bool directed = true)
    : m_directed(directed), m_name(name)
  {
    UniqueID<Node>::resetID();
    UniqueID<Arc>::resetID();
  }

  Graph(std::string const& name, const bool directed = true)
    : m_directed(directed), m_name(name)
  {
    UniqueID<Node>::resetID();
    UniqueID<Arc>::resetID();
    }*/

  //! \brief Constructor. Reserve memory for the given
  //! number of nodes and arcs.
  Graph(const size_t noNodes,
        const size_t noArcs,
        const bool directed = true)
    : m_nodes(noNodes), m_neighbors(noArcs), m_directed(directed)
  {
    UniqueID<Node>::resetID();
    UniqueID<Arc>::resetID();
  }

  /*Graph(std::string const& name,
        const size_t noNodes,
        const size_t noArcs,
        const bool directed = true)
    : m_nodes(noNodes), m_neighbors(noArcs), m_directed(directed), m_name(name)
  {
    UniqueID<Node>::resetID();
    UniqueID<Arc>::resetID();
  }

  Graph(const char* name,
        const size_t noNodes,
        const size_t noArcs,
        const bool directed = true)
    : m_nodes(noNodes), m_neighbors(noArcs), m_directed(directed), m_name(name)
  {
    UniqueID<Node>::resetID();
    UniqueID<Arc>::resetID();
  }*/

  //! \brief Destructor. Release allocated nodes and arcs.
  virtual ~Graph()
  {
    reset();
  }

  inline void reset()
  {
    m_nodes.clear();
    m_neighbors.clear();
  }

  void garbage()
  {
    m_nodes.garbage();
    m_neighbors.garbage();
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
  Node& addNode()
  {
    const size_t last = m_nodes.last() + 1U;
    m_nodes.insert(last);
    return getNode(last);
  }

  Node& addNode(Node const& nodeID)
  {
    m_nodes.insert(nodeID.id(), nodeID);
    return getNode(nodeID.id());
  }

  Node& addNode(const Key nodeID)
  {
    if (m_nodes.outofbound(nodeID))
      {
        // call Node::Node(Key)
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
  inline const Node& getNode(const Key nodeID) const
  {
    return m_nodes[nodeID];
  }

  inline Node& getNode(const Key nodeID)
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
        m_nodes.privateRemoveNeighbors(nodeID);
      }
  }

  //! \brief Return the number of nodes constituing the
  //! graph. Complexity is O(1).
  inline size_t howManyNodes() const
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
  void addArc(Node const& fromNode,
              Node const& toNode)
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
    if (m_neighbors.outofbound(arcID))
      return false;
    return m_neighbors.occupied(arcID);
  }

  //! \brief
  bool hasArc(const Key fromNodeID, const Key toNodeID) const
  {
    if (!hasNode(fromNodeID))
      return false;

    Node const& node = m_nodes[fromNodeID];
    size_t i = node.degree();
    while (i--)
      {
        Arc const& arc = m_neighbors[node.neighbor(i)->id()];
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
  inline const Arc& getArc(const Key arcID) const
  {
    return m_neighbors.get(arcID);
  }

  inline Arc& getArc(const Key arcID)
  {
    return m_neighbors.get(arcID);
  }

  //! \brief Return the arc refered by the given unique identifer of
  //! its nodes. Complexity is O(1) for finding the tail node and O(n)
  //! for the head where n is the number of neighbors of the tail
  //! node (the degree of a node).
  //! \param fromNodeID the unique identifier of the node serving of tail.
  //! \param toNodeID the unique identifier of the node serving of head.
  //! \return the address of the arc class if it was found in the
  //! graph, else return nullptr.
  Arc* getArc(const Key fromNodeID, const Key toNodeID)
  {
    if (!hasNode(fromNodeID))
      return nullptr;

    Node const& node = m_nodes[fromNodeID];
    size_t i = node.degree();
    while (i--)
      {
        Arc& arc = m_neighbors[node.neighbor(i)->id()];
        if ((arc.from() == fromNodeID) && (arc.to() == toNodeID))
          {
            return &arc;
          }
      }
    return nullptr;
  }

  //! Return the vector of neighbors of the node refered by its unique
  //! identifier. Neighbors are giving by a vector of arcs and not by a
  //! vector of nodes. Complexity is O(1).
  //! \param nodeID the unique identifier of the node to look for.
  //! \return the address of the vector else return nullptr.
  const std::vector<Arc*> &neighbors(const Key nodeID) const
  {
    if (!hasNode(nodeID))
      return nullptr;

    Node const& node = getNode(nodeID);
    return node.neighbors();
  }

  //! Remove an arc from the graph refered by its unique identifeir.
  //! Memory is released. Complexity is O(1).
  //! \param arcID the unique identifier of the arc to be removed.
  void removeArc(const Key arcID)
  {
    if (hasArc(arcID))
      {
        Arc& arc = m_neighbors[arcID];
        arc.from().removeNeighbor(arcID);
        arc.to().removeNeighbor(arcID);
        m_neighbors.privateRemoveNeighbors(arcID);
      }
  }

  //! \brief Return the number of arcs constituing the
  //! graph. Complexity is O(1).
  Key howManyArcs() const
  {
    return m_neighbors.used();
  }

  //! \brief
  //! Complexity is O(n).
  inline void unmarkAllArcs()
  {
    m_neighbors.unmarkAll();
  }

  //! \brief
  //! Note: no security is made if arcID is invalid
  //! Complexity is O(1).
  inline bool markedArc(const Key arcID)
  {
    return m_neighbors.marked(arcID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void markArc(const Key arcID)
  {
    m_neighbors.mark(arcID);
  }

  //! \brief
  //! Complexity is O(1).
  inline void unmarkArc(const Key arcID)
  {
    m_neighbors.unmark(arcID);
  }

  //! \brief Pretty print all the nodes constituing the graph.
  //! Complexity is O(n).
  inline void debugArcs() const
  {
    std::cout << "List of arcs:" << std::endl;
    m_neighbors.debug();
  }

  inline blockarcs_t& arcs()
  {
    return m_neighbors;
  }

  inline blocknodes_t& nodes()
  {
    return m_nodes;
  }

  inline const blockarcs_t& constArcs() const
  {
    return m_neighbors;
  }

  inline const blocknodes_t& constNodes() const
  {
    return m_nodes;
  }

private:

  //! \brief Shared function by two public functions.
  void private_addArc(Node &fromNode, Node &toNode)
  {
    const size_t last = m_neighbors.last() + 1U;// FIXME degeux car Collection est non pas Set
    m_neighbors.insert(A(last, fromNode, toNode)); // FIXME degeux: faire Set.append
    Arc& arc = m_neighbors.get(last);// FIXME degeux

    fromNode.addNeighbor(arc);
    if ((!m_directed) && (fromNode != toNode))
      {
        toNode.addNeighbor(arc); // FIXME: il faudrait inverser l'arc mais en n'ajoutant pas un nouveau arc.
                                 // FIXME: car on perd le fait de savoir qui vaut to et qui vaut from
                                 // FIXME: ducoup depthFirstSearch a efet de bord: les noeuds tentent de revenir vers eux memes
      }
  }

protected:

  //! \brief the vector of nodes constituing the graph.
  blocknodes_t m_nodes;
  //! \brief the vector of arcs constituing the graph.
  blockarcs_t m_neighbors;
  //! \brief direct or not direct graph ?
  bool m_directed;

public:

  std::string m_name;
};

using Graph_t = Graph<Node, Arc>;

/*inline std::ostream& operator<<(std::ostream& os, const Graph<N,A>& g)
{
  os << "List of nodes:" <<  std::endl << g.m_nodes << std::endl
     << "List of arcs:" <<  std::endl << g.m_neighbors << std::endl;
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const Graph<N,A>* g)
{
  os << *g;
  return os;
  }*/

} // namespace graphtheory

#endif /* CLASSIC_GRAPH_HPP_ */

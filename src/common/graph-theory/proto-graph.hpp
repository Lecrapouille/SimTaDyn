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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef CLASSIC_GRAPH_HPP_
#  define CLASSIC_GRAPH_HPP_

#  include "Logger.hpp"
#  include "ClassCounter.tpp"
#  include "GraphContainer.tpp"
#  include <algorithm>
#  include <vector>

// *************************************************************************************************
//! \brief Store continuous elements of type N as neighbors.
// *************************************************************************************************
template<class N>
class Neighbors
{
  using Iterator = typename std::vector<N>::iterator;

public:

  Neighbors()
  {}

  //-------------------------------------------------------------
  //! \brief Move constructor
  //-------------------------------------------------------------
  Neighbors(Neighbors<N>&& other)
    : m_neighbors(std::move(other.m_neighbors))
  {}

  //-------------------------------------------------------------
  //! \brief Copy constructor
  //-------------------------------------------------------------
  Neighbors(Neighbors<N> const& other)
    : m_neighbors(other.m_neighbors)
  {}

  //-------------------------------------------------------------
  //! \brief Add a new neighbor.
  //! \warning Unique elements are not checked when inserting.
  //! The vector should have unique elements (not mandatory).
  //-------------------------------------------------------------
  inline void addNeighbor(N const& elt)
  {
    m_neighbors.push_back(elt);
  }

  //-------------------------------------------------------------
  //! \brief Remove all neighbors.
  //-------------------------------------------------------------
  inline void eraseNeighbors()
  {
    m_neighbors.clear();
  }

  //-------------------------------------------------------------
  //! \brief Remove the nth elt neighbor of a node. No memory are
  //! released and do not do it before calling this function. If the
  //! index if incorrect no error is returned and nothing is
  //! done. Complexity is O(1).
  //! \param nth the nth element of the vector of neighbor.
  //-------------------------------------------------------------
  inline void removeNthNeighbor(size_t const nth)
  {
    if (nth < m_neighbors.size())
      {
        // FIXME: enlever cast
        privateRemoveNeighbors(m_neighbors.begin() + static_cast<int>(nth));
      }
  }

  //-------------------------------------------------------------
  //! \brief Remove the elt neighbor of a node refered by its unique
  //! identifier. If index is incorrect nothing is done and no error
  //! is returned. Complexity is O(n) where n is the number of
  //! neighbors.
  //! \param eltID the unique identifier of the elt to be removed.
  //-------------------------------------------------------------
  /*template<class T>
  inline void removeNeighbor(T const id)
  {
    // Temporary structure for comparing uniques identifiers.
    struct isValue
    {
      isValue(const T id) : m_id(id) {}
      bool operator()(const N *elt) const
      {
        return (elt->id() == m_id);
      }
      T m_id;
    };

    // Seelth and remove
    auto const& begin = m_neighbors.begin();
    auto const& end = m_neighbors.end();
    auto const& it = std::find_if(begin, end, isValue(id));
    if (it != end)
      {
        privateRemoveNeighbors(it);
      }
      }*/

  //-------------------------------------------------------------
  //! \brief Return the list of neighbors (read only).
  //-------------------------------------------------------------
  inline std::vector<N>& WARN_UNUSED neighbors()
  {
    return m_neighbors;
  }

  //-------------------------------------------------------------
  //! \brief Safe method() returning the Nth neighbor.
  //! If the neighbor does not exist neighor is set with a fake
  //! neighbor taken from a valid memory in the aim to avoid
  //! segfault if the developper used it without taking account
  //! the return code.
  //! \return true if the nth neighbor exists, else false.
  //-------------------------------------------------------------
  inline bool WARN_UNUSED neighbor(size_t const nth, N& neighor)
  {
    if (nth < m_neighbors.size())
      {
        neighor = m_neighbors[nth];
        return true;
      }
    else
      {
        LOGF("The Nth %zu neighbor does not exist", nth);
        static N dummy_neighbor;
        neighor = dummy_neighbor;
        return false;
      }
  }

  //-------------------------------------------------------------
  //! \brief Return the Nth neighbor.
  //! \return true if the nth neighbor exists, else false.
  //-------------------------------------------------------------
  inline N& WARN_UNUSED neighbor(size_t const nth)
  {
    return m_neighbors[nth];
  }

  //-------------------------------------------------------------
  //! \brief Return the number of neighbors.
  //-------------------------------------------------------------
  inline size_t WARN_UNUSED howManyNeighbors() const
  {
    return m_neighbors.size();
  }

private:

  //-------------------------------------------------------------
  //! \brief Replace the element to be removed by the last element of
  //! the container. Then remove the last element of the
  //! container. This prevents moving all items after the one removed
  //! with a complexity O(n) while order of neighbors is not
  //! important. Complexity is O(1).
  //-------------------------------------------------------------
  inline void privateRemoveNeighbors(Iterator const& it)
  {
    *it = std::move(m_neighbors.back());
    m_neighbors.pop_back();
  }

protected:

  std::vector<N> m_neighbors;
};

// *************************************************************************************************
//! \brief Store continuous elements of type B as borders.
// *************************************************************************************************
template<class B>
class Borders
{
public:

  Borders()
  {}

  //-------------------------------------------------------------
  //! \brief Move constructor
  //-------------------------------------------------------------
  Borders(Borders<B>&& other)
    : m_borders(std::move(other.m_borders))
  {}

  //-------------------------------------------------------------
  //! \brief Copy constructor
  //-------------------------------------------------------------
  Borders(Borders<B> const& other)
    : m_borders(other.m_borders)
  {}

  //-------------------------------------------------------------
  //! \brief Add a new border.
  //! \warning Unique elements are not checked when inserting.
  //! The vector should have unique elements (not mandatory).
  //-------------------------------------------------------------
  inline void addBorder(B& elt)
  {
    m_borders.push_back(elt);
  }

  //-------------------------------------------------------------
  //! \brief Remove all borders
  //-------------------------------------------------------------
  inline void eraseBorders()
  {
    m_borders.clear();
  }

  //-------------------------------------------------------------
  //! \brief Return the list of borders (read only).
  //-------------------------------------------------------------
  inline std::vector<B>& WARN_UNUSED borders()
  {
    return m_borders;
  }

  //-------------------------------------------------------------
  //! \brief Return the Nth border.
  //! If the border does not exist neighor is set with a fake
  //! border taken from a valid memory in the aim to avoid
  //! segfault if the developper used it without taking account
  //! the return code.
  //! \return true if the nth border exists, else false.
  //-------------------------------------------------------------
  inline bool WARN_UNUSED border(size_t const nth, B& border)
  {
    if (nth < m_borders.size())
      {
        border = m_borders[nth];
        return true;
      }
    else
      {
        LOGF("The Nth %zu border does not exist", nth);
        static B dummy_border;
        border = dummy_border;
        return false;
      }
  }

  //-------------------------------------------------------------
  //! \brief Return the number of borders.
  //-------------------------------------------------------------
  inline size_t WARN_UNUSED howManyBorders() const
  {
    return m_borders.size();
  }

protected:

  std::vector<B> m_borders;
};

//-------------------------------------------------------------
template<class D> class Arc;
using NodeId = size_t;
using ArcId = size_t;
//-------------------------------------------------------------

// *************************************************************************************************
//! \brief Edge element of a graph.
// *************************************************************************************************
template<class D>
class Node
  : private UniqueID<Node<D>>,
    public Neighbors<Arc<D>>
{
public:

  //-------------------------------------------------------------
  //! \brief Used by the graph container allocating blocks of
  //! continuous elements. Node identifiers are generated
  //! automaticaly.
  //-------------------------------------------------------------
  Node()
    : Neighbors<Arc<D>>(),
    m_id(UniqueID<Node<D>>::getID() - 1_z),
      m_data()
  {}

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  Node(NodeId const id)
    : Neighbors<Arc<D>>(),
      m_id(id),
      m_data()
  {}

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  Node(NodeId const id, D const& data)
    : Neighbors<Arc<D>>(),
      m_id(id),
      m_data(std::move(data))
  {}

  //-------------------------------------------------------------
  //! \brief Move constructor
  //-------------------------------------------------------------
  Node(Node<D>&& other)
    : Neighbors<Arc<D>>(other),
      m_id(other.id()),
      m_data(other.data())
  {}

  //-------------------------------------------------------------
  //! \brief Copy constructor
  //-------------------------------------------------------------
  Node(Node<D> const& other)
    : Neighbors<Arc<D>>(other),
      m_id(other.id()),
      m_data(other.data())
  {}

  //-------------------------------------------------------------
  //! \brief Copy assignment operator.
  //-------------------------------------------------------------
  Node<D>& operator=(Node<D> const& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        this->m_neighbors = other.m_neighbors;
        m_data = other.m_data;
      }
    return *this;
  }

  //-------------------------------------------------------------
  //! \brief Move assignment operator.
  //-------------------------------------------------------------
  Node<D>& operator=(Node<D>&& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        this->m_neighbors = std::move(other.m_neighbors);
        m_data = std::move(other.m_data);
      }
    return *this;
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with the
  //! unique identifier of another node.
  //-------------------------------------------------------------
  inline bool operator==(Node<D> const& rhs) const
  {
    return (id() == rhs.id());
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with the
  //! unique identifier of another node.
  //-------------------------------------------------------------
  inline bool operator!=(Node<D> const& rhs) const
  {
    return !(*this == rhs);
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with a given
  //! identifier.
  //-------------------------------------------------------------
  inline bool operator==(NodeId const& rhs) const
  {
    return m_id == rhs;
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with a given
  //! identifier.
  //-------------------------------------------------------------
  inline bool operator!=(NodeId const& rhs) const
  {
    return !(*this == rhs);
  }

  //-------------------------------------------------------------
  //! \brief Alias name
  //-------------------------------------------------------------
  inline void addArc(Arc<D>& arc)
  {
    Neighbors<Arc<D>>::addNeighbor(arc);
  }

  //-------------------------------------------------------------
  //! \brief Alias name
  //-------------------------------------------------------------
  inline void eraseArcs()
  {
    Neighbors<Arc<D>>::eraseNeighbors();
  }

  //-------------------------------------------------------------
  //! \brief Alias name
  //-------------------------------------------------------------
  inline std::vector<Arc<D>>& WARN_UNUSED arcs()
  {
    return Neighbors<Arc<D>>::neighbors();
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  void getNeighborNodes(std::vector<Node<D>>& nodes);

  //-------------------------------------------------------------
  //! \brief Alias name
  //-------------------------------------------------------------
  inline bool WARN_UNUSED arc(size_t const nth, Arc<D>& arc)
  {
    return Neighbors<Arc<D>>::neighbor(nth, arc);
  }

  //-------------------------------------------------------------
  //! \brief Alias name
  //-------------------------------------------------------------
  inline Arc<D>& WARN_UNUSED arc(size_t const nth)
  {
    return Neighbors<Arc<D>>::neighbor(nth);
  }

  //-------------------------------------------------------------
  //! \brief Return the number of neighbors. Complexity is O(1).
  //-------------------------------------------------------------
  inline uint32_t WARN_UNUSED degree() const
  {
    return Neighbors<Arc<D>>::howManyNeighbors();
  }

  //-------------------------------------------------------------
  //! \brief Return the unique identifier.
  //-------------------------------------------------------------
  inline NodeId WARN_UNUSED id() const
  {
    return m_id;
  }

  //-------------------------------------------------------------
  //! \brief Return the unique name.
  //-------------------------------------------------------------
  inline const std::string WARN_UNUSED name() const
  {
    return "N" + std::to_string(id());
  }

  inline const D& data() const
  {
    return m_data;
  }

  inline D& data()
  {
    return m_data;
  }

  inline void data(D const& d)
  {
    m_data = d;
  }

private:

  NodeId m_id;
  D      m_data;
};

//-------------------------------------------------------------
template<class D>
inline std::ostream& operator<<(std::ostream& os, const Node<D>& n)
{
  os << "Node " << n.id() << " neighbors = {";
  for (auto& it: n.neighbors())
    {
      os << ' ' << it.name();
    }
  os << " }";
  return os;
}

// *************************************************************************************************
//! \brief Link element of a graph.
// *************************************************************************************************
template<class D>
class Arc
  : private UniqueID<Arc<D>>,
    public Borders<Node<D>>
{
public:

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  enum Direction { From, To };

public:

  //-------------------------------------------------------------
  //! \brief Used by the Container allocating continuous elements
  //! This generates Node identifiers.
  //-------------------------------------------------------------
  Arc()
    : Borders<Node<D>>(),
      m_id(UniqueID<Arc<D>>::getID() - 1_z)
  {}

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  Arc(ArcId id, Node<D>& from, Node<D>& to); // TODO Key from, Key to
  Arc(ArcId id, Node<D>& from, Node<D>& to, D const& data);

  //-------------------------------------------------------------
  //! \brief Move constructor
  //-------------------------------------------------------------
  Arc(Arc<D>&& other)
    : Borders<Node<D>>(other),
      m_id(other.id()),
      m_data(other.data())
  {}

  //-------------------------------------------------------------
  //! \brief Copy constructor
  //-------------------------------------------------------------
  Arc(Arc<D> const& other)
    : Borders<Node<D>>(other),
    m_id(other.id()),
    m_data(other.data())
  {}

  //-------------------------------------------------------------
  //! \brief Copy assignment operator.
  //-------------------------------------------------------------
  Arc<D>& operator=(Arc<D> const& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        Borders<Node<D>>::m_borders = other.m_borders;
        m_data = other.m_data;
      }
    return *this;
  }

  //-------------------------------------------------------------
  //! \brief Move assignment operator.
  //-------------------------------------------------------------
  Arc<D>& operator=(Arc<D>&& other)
  {
    if (this != &other)
      {
        m_id = other.m_id;
        Borders<Node<D>>::m_borders = std::move(other.m_borders);
        m_data = std::move(other.m_data);
      }
    return *this;
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with the
  //! unique identifier of another node.
  //-------------------------------------------------------------
  inline bool operator==(Arc<D> const& rhs) const
  {
    return (id() == rhs.id()) &&
      (from().id() == rhs.from().id()) &&
      (to().id() == rhs.to().id());
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with the
  //! unique identifier of another node.
  //-------------------------------------------------------------
  inline bool operator!=(Arc<D> const& rhs) const
  {
    return !(*this == rhs);
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with a given
  //! identifier.
  //-------------------------------------------------------------
  inline bool operator==(ArcId const& rhs) const
  {
    return m_id == rhs;
  }

  //-------------------------------------------------------------
  //! \brief Compare the unique identifier of this node with a given
  //! identifier.
  //-------------------------------------------------------------
  inline bool operator!=(ArcId const& rhs) const
  {
    return !(*this == rhs);
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Return the unique identifier.
  inline NodeId WARN_UNUSED id() const
  {
    return m_id;
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Return the unique name.
  inline const std::string WARN_UNUSED name() const
  {
    return "A" + std::to_string(id());
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Return the reference of the tail of the arc.
  inline Node<D>& WARN_UNUSED from()
  {
    return Borders<Node<D>>::m_borders[Direction::From];
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Return the reference of the tail of the arc.
  inline const Node<D>& WARN_UNUSED from() const
  {
    return Borders<Node<D>>::m_borders[Direction::From];
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Change the tail of the arc.
  inline void from(Node<D> const& fromNode)
  {
    Borders<Node<D>>::m_borders[Direction::From] = fromNode;
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Return the reference of the head of the arc.
  inline Node<D>& WARN_UNUSED to()
  {
    return Borders<Node<D>>::m_borders[Direction::To];
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Return the reference of the head of the arc.
  inline const Node<D>& WARN_UNUSED to() const
  {
    return Borders<Node<D>>::m_borders[Direction::To];
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  //! \brief Change the head of the arc.
  inline void to(Node<D> const& toNode)
  {
    Borders<Node<D>>::m_borders[Direction::To] = toNode;
  }

  inline D& data()
  {
    return m_data;
  }

  inline const D& data() const
  {
    return m_data;
  }

  inline void data(D const& d)
  {
    m_data = d;
  }

private:

  ArcId  m_id;
  D      m_data;
};

//-------------------------------------------------------------
template<class D>
inline std::ostream& operator<<(std::ostream& os, const Arc<D>& n)
{
  os << "Arc " << n.id() << " = {";
  for (auto& it: n.borders())
    {
      os << ' ' << it.name();
    }
  os << " }";
  return os;
}

// *************************************************************************************************
//! \brief Declare a classic graph where N referes to a Node class and A to an Arc class.
// *************************************************************************************************
template<class Node, class Arc, class D>
class Graph
{

  //-------------------------------------------------------------
  //! \brief Sort name
  //-------------------------------------------------------------
  template<class T> using Container =
    graphtheory::GraphContainer<T,
                                config::graph_container_nb_elements,
                                graphtheory::GraphBlock>;

public:

  //-------------------------------------------------------------
  //! \brief Create a directed or undirected graph.
  //-------------------------------------------------------------
  Graph(bool const directed = true)
    : m_directed(directed)
  {}

  //-------------------------------------------------------------
  //! \brief Create a directed or undirected graph and reserve memory
  //! for arcs and nodes.
  //-------------------------------------------------------------
  Graph(size_t const noNodes, size_t const noArcs, bool const directed = true)
    : m_nodes(noNodes), m_arcs(noArcs), m_directed(directed)
  {}

  //-------------------------------------------------------------
  //! \brief Release memory for storing nodes and arcs.
  //-------------------------------------------------------------
  ~Graph() { reset(); }

  //-------------------------------------------------------------
  //! \brief Adds a node to the graph. If node with given id already
  //! exists in the graph its data is extended with whatever comes in
  //! 'data' argument.
  //! Complexity is O(1).
  //! \param id the node's number identifier.
  //! \param data additional data for the node being added. If node
  //! already exists its data object is augmented with the new one.
  //! \return node The newly added node or node with given id if it
  //! already exists.
  //-------------------------------------------------------------
  Node& addNode(NodeId const id, D const& data)
  {
    if (m_nodes.outofbound(id))
      {
        m_nodes.insert(id, Node(id, data));
        return getNode(id);
      }
    else
      {
        m_nodes.occupy(id);
        Node& node = getNode(id);
        node.data(data);
        return node;
      }
  }

  //-------------------------------------------------------------
  //! \brief Check if the given node exists.
  //! Complexity is O(1).
  //-------------------------------------------------------------
  bool WARN_UNUSED hasNode(NodeId const id) const
  {
    if (m_nodes.outofbound(id))
      return false;
    return m_nodes.occupied(id);
  }

  //-------------------------------------------------------------
  //! \brief Gets node with given identifier. If node does not exist
  //! undefined value is returned.
  //! Complexity is O(1).
  //-------------------------------------------------------------
  Node& getNode(NodeId const id)
  {
    return m_nodes[id];
  }

  //-------------------------------------------------------------
  //! \brief Gets node with given identifier. If node does not exist
  //! false is returned as well as a fake node to avoid possible
  //! segfault.
  //! Complexity is O(1).
  //! \return true if the node was found else return false.
  //-------------------------------------------------------------
  bool getNode(NodeId const id, Node& node)
  {
    if (!hasNode(id))
      {
        static Node dummy_node(0);
        node = dummy_node;
        return false;
      }
    else
      {
        node = m_nodes[id];
        return true;
      }
  }

  //-------------------------------------------------------------
  //! \brief Removes node with given id from the graph. If node does
  //! not exist in the graph does nothing.
  //! Complexity is O(1).
  //-------------------------------------------------------------
  bool removeNode(NodeId const& id)
  {
    if (hasNode(id))
      {
        m_nodes[id].removeAllNeighbors();
        m_nodes.privateRemoveNeighbors(id);
      }
  }

  //-------------------------------------------------------------
  //! \brief Gets number of nodes in this graph.
  //! Complexity is O(1).
  //-------------------------------------------------------------
  inline size_t WARN_UNUSED getNodesCount() const
  {
    return m_nodes.used();
  }

  //-------------------------------------------------------------
  //! \brief Clear 'marked' flag to all nodes.
  //! Complexity is O(n).
  //-------------------------------------------------------------
  inline void unmarkAllNodes()
  {
    m_nodes.unmarkAll();
  }

  //-------------------------------------------------------------
  //! \brief Set 'marked' flag to the given node.
  //! Complexity is O(1).
  //! \note No security is made if id exist or not.
  //-------------------------------------------------------------
  inline void markNode(NodeId const id)
  {
    m_nodes.mark(id);
  }

  //-------------------------------------------------------------
  //! \brief Clear 'marked' flag to the given node.
  //! Complexity is O(1).
  //! \note No security is made if id exist or not.
  //-------------------------------------------------------------
  inline void unmarkNode(NodeId const id)
  {
    m_nodes.unmark(id);
  }

  //-------------------------------------------------------------
  //! \brief Get the 'marked' flag of the given node.
  //! Complexity is O(1).
  //! \note No security is made if id exist or not.
  //-------------------------------------------------------------
  inline bool getNodeMark(NodeId const id)
  {
    return m_nodes.marked(id);
  }

  //-------------------------------------------------------------
  //! \brief Pretty print all the nodes constituing the graph.
  //! Complexity is O(n).
  //-------------------------------------------------------------
  inline void debugNodes() const
  {
    std::cout << "List of nodes:" << std::endl;
    m_nodes.debug();
  }

  //-------------------------------------------------------------
  //! \brief Create a link between two nodes. Nodes are refered by
  //! their unique identifier. If they do not exist, they are created.
  //! Complexity is O(1) to add the arc and also O(1) for nodes.
  //! \param fromID the unique identifier of the tail of the arc.
  //! \param toID the unique identifier of the head of the arc.
  //-------------------------------------------------------------
  Arc& addArc(NodeId const from, NodeId const to, D const& data)
  {
    std::cout << "AddArc " << from << " --> " << to << "\n";
    addNode(from, data); // FIXME specific data for nodes
    addNode(to, data); // FIXME specific data for nodes
    return privateAddArc(getNode(from), getNode(to), data);
  }

  //-------------------------------------------------------------
  //! \brief Create a link between two nodes in the graph. This
  //! methode assumes that the client has already created nodes and
  //! simply adds them to the graph if they don't exist. Complexity is
  //! O(1) both for adding the arc and nodes.
  //! \param fromNode the reference of the node that will be used for
  //! the tail of the arc.
  //! \param toNode the reference of the node that will be used for
  //! the head of the arc.
  //-------------------------------------------------------------
  Arc& addArc(Node& from, Node& to, D const& data)
  {
    std::cout << "AddArc " << from.id() << " --> " << to.id() << "\n";
    addNode(from.id(), data); // FIXME specific data for nodes
    addNode(to.id(), data); // FIXME specific data for nodes
    return privateAddArc(from, to, data);
  }

  //-------------------------------------------------------------
  //! \brief Return if the arc refered by the given unique identifer
  //! exists in the graph. Complexity is O(1).
  //! \param arcID the unique identifier of the arc to look for.
  //! \return true if the arc exists, else false.
  //-------------------------------------------------------------
  Arc& WARN_UNUSED getArc(ArcId const id) const
  {
    return m_arcs.get(id);
  }

  //-------------------------------------------------------------
  //! \brief Return if the arc refered by the given nodes exists in
  //! the graph. Complexity is O(n) with n the degree of from.
  //! \param arcID the unique identifier of the arc to look for.
  //! \return true if the arc exists, else false.
  //-------------------------------------------------------------
  bool WARN_UNUSED getArc(NodeId const from, NodeId const to, Arc& arc)
  { // FIXME: arc = fake_arc;
    if (!hasNode(from))
      return false;

    // Optim: comparer degrees of from and to
    Node const& node = m_nodes[from];
    size_t i = node.degree();
    while (i--)
      {
        arc = m_arcs[node.neighbor(i)->id()];
        if ((arc.from().id() == from) && (arc.to().id() == to))
          {
            return true;
          }
      }
    return false;
  }

  //-------------------------------------------------------------
  //! \brief
  //! Complexity is O(n) with n the degree of from.
  //-------------------------------------------------------------
  bool WARN_UNUSED hasArc(NodeId const from, NodeId const to) const
  {
    if (!hasNode(from))
      return false;

    Node const& node = m_nodes[from];
    size_t i = node.degree();
    while (i--)
      {
        Arc const& arc = m_arcs[node.neighbor(i)->id()];
        if ((arc.from().id() == from) && (arc.to().id() == to))
          {
            return true;
          }
      }
    return false;
  }

  //-------------------------------------------------------------
  //! \brief
  //! Complexity is O(1).
  //-------------------------------------------------------------
  bool WARN_UNUSED hasArc(ArcId const id) const
  {
    if (m_arcs.outofbound(id))
      return false;
    return m_arcs.occupied(id);
  }

  //-------------------------------------------------------------
  //! \brief
  //! Complexity is O(1).
  //-------------------------------------------------------------
  bool WARN_UNUSED removeArc(ArcId const id)
  {
    if (hasArc(id))
      {
        Arc& arc = m_arcs[id];
        //arc.from().removeNeighbor(id);
        //arc.to().removeNeighbor(id);
        //FIXME m_arcs.removeNeighbor(id);
        return true;
      }
    return false;
  }

  //-------------------------------------------------------------
  //! \brief
  //! Complexity is O(1).
  //-------------------------------------------------------------
  inline size_t WARN_UNUSED howManyArcs() const
  {
    return m_arcs.used();
  }

  //-------------------------------------------------------------
  //! \brief Clear 'marked' flag to all arcs.
  //! Complexity is O(n).
  //-------------------------------------------------------------
  inline void unmarkAllArcs()
  {
    m_arcs.unmarkAll();
  }

  //-------------------------------------------------------------
  //! \brief Get the 'marked' flag of the given arc.
  //! Complexity is O(1).
  //! \note No security is made if id exist or not.
  //-------------------------------------------------------------
  inline bool getArcMark(ArcId const id)
  {
    return m_arcs.marked(id);
  }

  //-------------------------------------------------------------
  //! \brief Set 'marked' flag to the given arc.
  //! Complexity is O(1).
  //! \note No security is made if id exist or not.
  //-------------------------------------------------------------
  inline void markArc(ArcId const id)
  {
    m_arcs.mark(id);
  }

  //-------------------------------------------------------------
  //! \brief Clear 'marked' flag to the given arc.
  //! Complexity is O(1).
  //! \note No security is made if id exist or not.
  //-------------------------------------------------------------
  inline void unmarkArc(ArcId const id)
  {
    m_arcs.unmark(id);
  }

  //-------------------------------------------------------------
  //! \brief Pretty print all the nodes constituing the graph.
  //! Complexity is O(n).
  //-------------------------------------------------------------
  inline void debugArcs() const
  {
    std::cout << "List of arcs:" << std::endl;
    m_arcs.debug();
  }

  //-------------------------------------------------------------
  //! \brief Erase all nodes and arcs.
  //! Complexity is O(1).
  //-------------------------------------------------------------
  inline void reset()
  {
    m_nodes.clear();
    m_arcs.clear();
  }

  //-------------------------------------------------------------
  //! \brief Garbage memory of containers used for nodes and arcs.
  //! Free blocks of memory no longer used.
  //! Complexity is O(n).
  //-------------------------------------------------------------
  inline void garbage()
  {
    m_nodes.garbage();
    m_arcs.garbage();
  }

  //-------------------------------------------------------------
  //! \brief Return if the graph is directed or undirected.
  //-------------------------------------------------------------
  inline bool WARN_UNUSED directed() const
  {
    return m_directed;
  }

  //-------------------------------------------------------------
  //! \brief Return the arcs container
  //-------------------------------------------------------------
  inline Container<Arc>& arcs()
  {
    return m_arcs;
  }

  //-------------------------------------------------------------
  //! \brief  Return the nodes container
  //-------------------------------------------------------------
  inline Container<Node>& nodes()
  {
    return m_nodes;
  }

private:

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  Arc& privateAddArc(Node& fromNode, Node& toNode, D const& data)
  {
    const size_t last = m_arcs.last() + 1U;// FIXME degeux car Collection est non pas Set
    m_arcs.insert(Arc(last, fromNode, toNode, data)); // FIXME degeux: faire Set.append
    Arc& arc = m_arcs.get(last);// FIXME degeux

    fromNode.addNeighbor(arc);
    if ((!m_directed) && (fromNode.id() != toNode.id()))
      {
        toNode.addNeighbor(arc); // FIXME: il faudrait inverser l'arc mais en n'ajoutant pas un nouveau arc.
                                 // FIXME: car on perd le fait de savoir qui vaut to et qui vaut from
                                 // FIXME: ducoup depthFirstSearch a efet de bord: les noeuds tentent de revenir vers eux memes
      }
    return arc;
  }

private:

  //! \brief the vector of nodes constituing the graph.
  Container<Node> m_nodes;

  //! \brief the vector of arcs constituing the graph.
  //! FIXME: should use Set not Collection
  Container<Arc> m_arcs;

  //! \brief direct or not direct graph ?
  bool m_directed;
};

#endif // CLASSIC_GRAPH_HPP_

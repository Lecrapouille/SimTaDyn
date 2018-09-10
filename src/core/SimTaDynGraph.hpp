#ifndef SIMTADYN_GRAPH_HPP_
#  define SIMTADYN_GRAPH_HPP_

//#  include "Graph.hpp"
#  include "GraphAlgorithm.hpp"
#  include <initializer_list>

// FIXME: faire qu'une cellule puisse etre aussi une carte (url, download)
// map a executer (url, carte contenue dans le SceneGraph).
// Exemple: carte des villes principales et des autoroutes. On clique on
// a une carte detaillee.
//FIXME: renomer neighbor par isborderof
namespace graphtheory
{

class Zone;
class ExtArc;

// **************************************************************
//
// **************************************************************
class Zone: private UniqueID<Zone>
{
public:

  Zone()
  {
  }

  //! \brief! Constructor.
  //! \param id the unique indentifier.
  //! \param fromNode the reference of an existing node for the tail.
  //! \param toNode the reference of an existing node for the head.
  Zone(const Key id, std::initializer_list<Arc*> arcs)
    : m_id((id & ~GraphElementId::MASK) | GraphElementId::ZONE),
      m_arcs(arcs)
  {
  }

  //! \brief Constructor by copy.
  //! \param arc the reference of an existing arc.
  Zone(Zone const& zone)
    : UniqueID<Zone>(),
      m_id(zone.id() | GraphElementId::ZONE),
      m_arcs(zone.m_arcs)
  {
  }

  //! \brief Self copy. Used for the constructor by copy.
  Zone* clone() const
  {
    return new Zone(*this);
  }

  //! \brief Virtual destructor.
  virtual ~Zone() {}

  //! \brief Return the unique identifier.
  inline virtual Key id() const { return m_id; }

  //! \brief Return the unique identifier.
  operator size_t() { return m_id; }

  inline const std::string name() const
  {
    return "Z" + std::to_string(id());
  }

  //! \brief Return the reference of the arc.
  inline Arc *arc(const size_t nth) { return m_arcs.at(nth); }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator==(const Zone& rhs) const
  {
    return (m_id == rhs.m_id) /*&& (m_fromNode == rhs.m_fromNode) &&
                                (m_toNode == rhs.m_toNode)*/
      ;
  }

  //! \brief Compare the unique identifier of this node with the unique
  // identifier of another node.
  inline bool operator!=(const Zone& rhs) const
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
  std::vector<Arc*> m_arcs;
};

// *************************************************************************************************
//! \brief Extended Arc. Knows zones as neighbors.
// *************************************************************************************************
class ExtArc: public Arc
{
public:

  ExtArc()
    : Arc()
  {
  }

  //! \brief! Constructor.
  //! \param id the unique indentifier.
  //! \param fromNode the reference of an existing node for the tail.
  //! \param toNode the reference of an existing node for the head.
  ExtArc(const Key id, Node& fromNode, Node& toNode)
    : Arc(id, fromNode, toNode)
  {
  }

  //! \brief Constructor by copy.
  //! \param arc the reference of an existing arc.
  ExtArc(ExtArc const& arc)
    : Arc(arc)
  {
    m_zones = arc.m_zones;
  }

  //! \brief add a new node neighbor refered by its link
  inline void addNeighbor(Zone& zone)
  {
    //LOGI("Node ID %u: Add the arc ID %u as neighbor\n", m_id, arc);
    m_zones.push_back(&zone);
  }

  //! \brief Remove all neighbors
  inline void removeAllNeighbors()
  {
    m_zones.clear();
  }

  //! \brief Remove the arc neighbor of a node refered by its unique
  //! identifier. If index is incorrect nothing is done and no error
  //! is returned. Complexity is O(n) where n is the number of
  //! neighbors.
  //! \param arcID the unique identifier of the arc to be removed.
  inline void removeNeighbor(const Key zoneID)
  {
    LOGIS("Arc ID %u: Remove the zone ID %u as neighbor\n", m_id, zoneID);

    // Temporary structure for comparing uniques identifiers.
    struct isValue
    {
      Key m_id;
      isValue(const Key id) : m_id(id) {}
      bool operator()(const Zone *zone) const
      {
        return (zone->id() == m_id);
      }
    };

    // Search and remove
    auto end = m_zones.end();
    auto it = std::find_if(m_zones.begin(), end, isValue(zoneID));
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
  inline void removeNthNeighbor(const size_t nth)
  {
    LOGIS("Arc ID %u: Remove its %u'nth zone ID neighbor\n", m_id, nth);
    if (nth < m_zones.size())
      {
        //FIXME: enlever cast
        remove(m_zones.begin() + static_cast<int>(nth));
      }
  }

  //! \brief Return the address of the nth neighbor refered by its
  //! arc. Complexity is O(1).
  //! \param nth the nth element of the list of neighbor.
  //! \return the address of the arc, else nullptr if the index is
  //! incorrect.
  inline Zone *neighbor(const size_t nth)
  {
    if (nth < m_zones.size())
      {
        return m_zones.at(nth);
      }
    return nullptr;
  }

  //! \brief Return the address of the nth neighbor refered by its
  //! arc. Complexity is O(1).
  //! \param nth the nth element of the list of neighbor.
  //! \return the address of the arc, else nullptr if the index is
  //! incorrect.
  inline const Zone *neighbor(const size_t nth) const
  {
    if (nth < m_zones.size())
      {
        return m_zones.at(nth);
      }
    return nullptr;
  }

  inline const Zone &nthNeighbor(const size_t nth) const
  {
    return *(m_zones[nth]);
  }

  inline Zone &nthNeighbor(const size_t nth)
  {
    return *(m_zones[nth]);
  }

  //! \brief Return the list of neighbors.
  inline const std::vector<Zone*> &neighbors() const
  {
    return m_zones;
  }

  //! \brief Return the number of neighbors. Complexity is O(1).
  inline size_t degree() const
  {
    return m_zones.size();
  }

protected:

  //! \brief Swap element to be removed with the last element and then
  //! remove the item at the end of the container. This prevents
  //! moving all items after the one removed (complexity
  //! O(n)). Complexity is O(1).
  void remove(std::vector<Zone*>::iterator const& it)
  {
    *it = m_zones.back();
    m_zones.pop_back();
  }

protected:

  //! \brief the list of node neighbors refered by their arc starting
  //! from this node. We prefer sacrificed memory than reducing
  //! computations for looking for neighbors.
  std::vector<Zone*> m_zones;
};

// *************************************************************************************************
//! \brief SimTaDynGraph is a classic Graph extended to have zones. A zone is delimited by a set of
//! graph arcs.
// *************************************************************************************************
template <class Node, class Arc, class Zone>
class SimTaDynGraph: public Graph<Node, Arc>
{
public:

  typedef GraphContainer<Zone, config::graph_container_nb_elements, GraphBlock> blockzones_t; // FIXME should be a Set not a Collection

  SimTaDynGraph(const bool directed = true)
    : Graph<Node, Arc>(directed)
  {
    UniqueID<Zone>::resetID();
  }

  SimTaDynGraph(const char* name, const bool directed = true)
    : Graph<Node, Arc>(name, directed)
  {
    UniqueID<Zone>::resetID();
  }

  SimTaDynGraph(std::string const& name, const bool directed = true)
    : Graph<Node, Arc>(name, directed)
  {
    UniqueID<Zone>::resetID();
  }

  //! \brief Constructor. Reserve memory for the given
  //! number of nodes and arcs.
  SimTaDynGraph(const size_t noNodes,
          const size_t noArcs,
          const bool directed = true)
    : Graph<Node, Arc>(noNodes, noArcs, directed)
  {
    UniqueID<Zone>::resetID();
  }

  SimTaDynGraph(std::string const& name,
          const size_t noNodes,
          const size_t noArcs,
          const bool directed = true)
    : Graph<Node, Arc>(name, noNodes, noArcs, directed)
  {
    UniqueID<Zone>::resetID();
  }

  SimTaDynGraph(const char* name,
          const size_t noNodes,
          const size_t noArcs,
          const bool directed = true)
    : Graph<Node, Arc>(name, noNodes, noArcs, directed)
  {
    UniqueID<Zone>::resetID();
  }

  Node& addNode(const std::string formulae)
  {
    Node& n = Graph<Node, Arc>::addNode();
    n.formulae(formulae);
    return n;
  }

  // TODO ajouter addZone, removeZone, hasZone, markZone ...

  void addZone(Arc const& a1, Arc const& a2, Arc const& a3)
  {
    std::cout << "AddZone " << a1 << " -- " << a2 << " -- " << a3 << "\n";
    addArc(a1);
    addArc(a2);
    addArc(a3);
    //private_addZone(getNode(fromNode.id()), getNode(toNode.id()));
  }

  inline bool hasZone(const Key zoneID) const
  {
    if (m_zones.outofbound(zoneID))
      return false;
    return m_zones.occupied(zoneID);
  }

  //! \brief Return the number of zones constituing the
  //! graph. Complexity is O(1).
  inline size_t howManyZones() const
  {
    return m_zones.used();
  }

protected:

  //! \brief the list of zones constituing the graph.
  blockzones_t m_zones;
};

  //using SimTaDynGraph_t = SimTaDynGraph<Node, ExtArc, Zone>;

// *************************************************************************************************
//! \brief SimTaDynGraph is a classic Graph extended to have zones. A zone is delimited by a set of
//! graph arcs.
// *************************************************************************************************
//template <class Node, class Arc, class Zone2D, class Zone3D>
//class Graph3D: public SimTaDynGraph<Node,Arc,Zone2D>
//{
//};
//using Graph3D_t = Graph3D<Node, ExtArc, ExtendedZone, BasicZone3D>;

} // namespace graphtheory

#endif

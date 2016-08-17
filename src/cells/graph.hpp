#ifndef GRAPH_HPP_
#  define GRAPH_HPP_

#  include "nodes.hpp"
#  include "arcs.hpp"

// *************************************************************************************************
// Define a SimTaDyn graph
// ************************************************************************************************
// http://mooreccac.com/kcppdoc/Dijkstra.htm
// https://www.cs.cmu.edu/~pavlo/courses/fall2013/static/papers/osdi2012-graphchi.pdf
typedef map<Key, SimTaDynCell*> listNodes;
typedef map<Key, SimTaDynCell*> listArcs;

class SimTaDynGraph: private ClassCounter<SimTaDynGraph>
{
public:
  Position3D bbox_min; // Bounding box min position
  Position3D bbox_max; // Bounding box max position

  string getName() { return name_; }
  void setName(string name) { name_ = name; }
  SimTaDynGraph(string name) { name_ = name; }
  SimTaDynGraph() { name_ = ""; }
  ~SimTaDynGraph() { }

  void addNode(string name, const Position3D& position, string code_forth)
  {
    SimTaDynCell* node = new SimTaDynNode(name, position, code_forth);
    nodes_.insert(make_pair(node->privateId(), node));
  }

  void addNode(string name, const Position3D& position)
  {
    SimTaDynCell* node = new SimTaDynNode(name, position);
    nodes_.insert(make_pair(node->privateId(), node));
  }

  void addNode(const Position3D& position)
  {
    SimTaDynCell* node = new SimTaDynNode(position);
    nodes_.insert(make_pair(node->privateId(), node));
  }

  void addArc(Key k1, Key k2, string name, bool oriented = false, int cost = 0)
  {
    SimTaDynCell* n1 = getNode(k1);
    SimTaDynCell* n2 = getNode(k2);
    SimTaDynCell* arc = new SimTaDynArc(n1, n2, name, oriented, cost);
    arcs_.insert(make_pair(arc->privateId(), arc));
  }

  SimTaDynCell* getNode(const Key& id) // FIXME &
  {
    //std::set<SimTaDynCell*>::iterator it;

    auto it = nodes_.find(id);
    if (it == nodes_.end())
      {
        throw std::out_of_range("getNode(const Key& id): SimTaDynNode " + to_string(id) + " not found in the graph");
      }
    return it->second;
  }

  SimTaDynCell* getArc(const Key& id) // FIXME &
  {
    //std::set<SimTaDynCell*>::iterator it;

    auto it = arcs_.find(id);
    if (it == arcs_.end())
      {
        throw std::out_of_range("getArc(const Key& id): SimTaDynArc " + to_string(id) + " not found in the graph");
      }
    return it->second;
  }

protected:
  /*
   * Give a name to the element which will be displayed in the GUI.
   * Contrary to id_ several cells can have the same name. By default
   * the name is unique.
   */
  string name_;
  listNodes nodes_;
  listArcs arcs_;
};

#endif /* GRAPH_HPP_ */

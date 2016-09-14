#ifndef GRAPH_HPP_
#  define GRAPH_HPP_

#  include "SimTaDynCells.hpp"

class SimTaDynGraph: private ClassCounter<SimTaDynGraph>
{
public:
  /*
   * Give a name to the element which will be displayed in the GUI.
   * Contrary to id_ several cells can have the same name. By default
   * the name is unique.
   */
  string name;

  /*
   * Axis Align Bounding box of the map
   */
  AABB bbox;

  SimTaDynGraph()
  {
    id_ = howMany();
    name = "Graph_" + std::to_string(id_);
  }

  SimTaDynGraph(std::string const& name)
  {
    this->name = name;
  }

  ~SimTaDynGraph()
  {
    std::map<Key, SimTaDynCell*>::iterator it;
    for (it = nodes_.begin(); it != nodes_.end(); ++it)
      {
        delete it->second;
      }
  }

  //bool loadShapefile(ShapefileLoader& loader, std::string const& filename)
  //{
  //  return loader.loadShapefile(filename, *this);
  //}

  SimTaDynCell* addNode(Position3D const& p)
  {
    SimTaDynCell* node = new SimTaDynCell();// FIXMEnew SimTaDynNode();
    node->moveToPosition(p);

    nodes_[node->privateId()] = node;
    return node;
  }

  SimTaDynCell* addNode(Position3D const& position, string name, const string& code_forth = "", void *const data = NULL)
  {
    SimTaDynCell* node = new SimTaDynCell(name, code_forth, data);// FIXMEnew SimTaDynNode();
    node->moveToPosition(position);

    nodes_[node->privateId()] = node;
    return node;
  }

  void removeNode(const Key id)
  {
    delete nodes_[id];
    nodes_.erase(id);
  }

  /*
   * Instances counter
   */
  static Key howMany()
  {
    return ClassCounter<SimTaDynCell>::howMany();
  }

  /*
   * For debug
   */
  virtual Key whoAmI()
  {
    std::cout << "I am the SimTaDynCell #" << id_ << " nammed \"" << name << "\" (" << this << "):" << std::endl;
    return id_;
  }

protected:
  /*
   * For rendering the graph scene
   */
  friend class DrawGraph;

  /*
   * Make this instance unique with this identifier.
   * Used it for comparing element in a container.
   */
  Key id_;

  /*
   * List of nodes
   */
  std::map<Key, SimTaDynCell*> nodes_;
};

#endif /* GRAPH_HPP_ */

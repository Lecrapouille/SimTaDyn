#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "SimTaDynGraph.hpp"
#  include "Color.hpp"
//#  include "RTree.hpp"

class SimTaDynMap
{
public:
  SimTaDynMap()
    : m_name(""), m_graph()
  {
  }

  SimTaDynMap(std::string const& name)
    : m_name(SimTaDynMap::shortName(name)), m_graph()
  {
  }

  ~SimTaDynMap()
  {
  }

  // FIXME: a faire par la suite m_spatial_index.root.bbox()
  AABB const& bbox() const
  {
    return m_bbox;
  }

  SimTaDynNode *addNode(Vector3D const& p);
  bool removeNode(const Key nodeID);

  //! \brief give the file name from a pathe
  static std::string shortName(std::string const& path)
  {
    return path.substr(path.find_last_of("/") + 1);
  }

  // FIXME: faire un
  // enum DataField { Position, Color };
  // std::vector<containers*> m_datum
  container<Vector3D> m_positions;
  container<Color> m_colors;

  //protected:

  // FIXME: TEMPORAIRE a supprimer.
  AABB m_bbox;

  //public: // FIXME: a repenser

  std::string m_name;
  //RTreeNode m_spatial_index;
  SimTaDynGraph_t m_graph;
  // TODO liste de scripts Forth
  // Et qui dit script dit qu'il faudra les gerer dans l'IHM (sorte speedbar)
};

#endif /* SIMTADYN_MAP_HPP_ */

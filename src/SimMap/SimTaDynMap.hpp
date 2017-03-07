#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "Vertex.hpp"
#  include "File.hpp"
#  include "Resources.hpp"
#  include "RTree.hpp"
#  include "SimTaDynGraph.hpp"
#  include "Renderable.hpp"
#  include "Renderer.hpp"
//#  include "MapEditor.hpp"

// FIXME: faut creer une class helper pour charger une seule fois les
// shader commun a tous les cartes (ou alors 1 carte == 1 shader meme
// s'il doit etre mis N fois dans le GPU

class SimTaDynMap
  : private ClassCounter<SimTaDynMap>,
    public IResource,
    public IRenderable
{
  friend class MapEditor;

public:

  SimTaDynMap()
    : IResource(howMany()),
      m_graph()
  {
    m_name = "Map_" + std::to_string(id());
  }

  SimTaDynMap(std::string const& name)
    : IResource(howMany()),
      m_graph(),
      m_name(File::shortNameWithExtension(name))
  {
  }

  ~SimTaDynMap()
  {
    m_graph.BasicGraph::reset();
    //FIXME MapEditor::instance().remove(id());
  }

  //! \brief
  SimTaDynNode *addNode(Vertex const& p);

  //! \brief
  bool removeNode(const Key nodeID);

  //! \brief
  SimTaDynNode *getNode(const Key nodeID);

  //! \brief Instances counter.
  static Key howMany()
  {
    return ClassCounter<SimTaDynMap>::howMany();
  }

  inline AABB const& bbox() const
  {
    // FIXME: a faire par la suite m_spatial_index.root.bbox()
    return m_bbox;
  }

  void drawnBy(IRenderer const& renderer) const override
  {
    (void) renderer;
    //renderer.draw(m_vertices);
  }

  //! \brief For debug purpose.
  virtual void debug()
  {
    std::cout << "I am SimTaDynMap #" << id() << " named '" << m_name << "':"
              << bbox()
      // << m_graph
              << std::endl;
  }

public:

  //! \brief the map structured as a graph.
  SimTaDynGraph_t m_graph;

  // TODO liste de scripts Forth
  // Et qui dit script dit qu'il faudra les gerer dans l'IHM (sorte speedbar)

  //! \brief Spatial index FIXME: drawble rtree
  RTreeNode* m_rtree;

  //! \brief Axis Align Bounding box of the map
  //! FIXME: TEMPORAIRE car sera donner par Rtree.bbox()
  AABB m_bbox;

  //! \brief Give a name to the element which will be displayed in the
  //! GUI.  Contrary to id_ several cells can have the same name. By
  //! default the name is unique.
  std::string m_name;

  //! \brief database FIXME TBD:
  // enum DataField { Position, Color };
  // std::vector<containers*> m_datum
  // or:
  // std::map(string, containers)
  // or:
  // BTree sur des fichiers + convertion(char, type_de_la_colonne)
  // or: clef MySQL
  container<Vertex> m_vertices;
};

#endif /* SIMTADYN_MAP_HPP_ */

#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "Vertex.hpp"
#  include "Resources.hpp"
#  include "RTree.hpp"
#  include "SimTaDynGraph.hpp"
#  include "Set.tpp"
#  include "Renderer.hpp"

// FIXME: faut creer une class helper pour charger une seule fois les
// shader commun a tous les cartes (ou alors 1 carte == 1 shader meme
// s'il doit etre mis N fois dans le GPU

class SimTaDynMap
  : public IResource<Key>,
    private ClassCounter<SimTaDynMap>
    //public IRenderable
{
  friend class MapEditor;

public:

  SimTaDynMap()
    : IResource(),
      m_id(ClassCounter<SimTaDynMap>::count()),
      m_name("Map_" + std::to_string(m_id)),
      m_graph("graph_01")
  {
    LOGI("New SimTaDynMap with generic name '%s' and Id %u\n", m_name.c_str(), m_id);
  }

  SimTaDynMap(std::string const& name)
    : IResource(),
      m_id(ClassCounter<SimTaDynMap>::count()),
      m_name(name),
      m_graph("graph_01")
  {
    LOGI("New SimTaDynMap named '%s' and ID %u\n", m_name.c_str(), m_id);
  }

  ~SimTaDynMap()
  {
    LOGI("Deleting SimTaDynMap %u named '%s'\n", m_id, m_name.c_str());
    //FIXME MapEditor::instance().remove(m_id);
  }

  //! \brief Return the unique identifier.
  inline Key id() const
  {
    return m_id;
  }

  //! \brief Return the unique identifier.
  operator int()
  {
    return m_id;
  }

  inline AABB const& bbox() const
  {
    // FIXME: a faire par la suite m_spatial_index.root.bbox()
    return m_bbox;
  }

  //! \brief For debug purpose.
  virtual void debug() //FIXME const
  {
    std::cout << "I am SimTaDynMap #" << m_id << " named '" << m_name << "':"
              << std::endl << "{"
              << std::endl << "  " << bbox() << std::endl
              << "  List of nodes (" << m_graph.nodes().blocks() << " blocks):"
              << std::endl;

    // FIXME: ajouter un forEach
    auto end = m_graph.nodes().end();
    auto it = m_graph.nodes().begin();
    for (; it != end; ++it)
      {
        std::cout << "    " << (*it) << " " << pos[it->m_dataKey]
                  << std::endl;
      }
    std::cout << "}" << std::endl;
  }

  inline void clear()
  {
    m_graph.reset();
  }

  /*
  void drawnBy(GLRenderer const& renderer)
  {
    // Draw nodes
    uint32_t i = pos.blocks();
    while (i--)
      {
        col.block(i)->begin();
        renderer.m_colAttrib.begin();

        pos.block(i)->begin();
        renderer.m_posAttrib.begin();
        pos.block(i)->draw(GL_POINTS, renderer.m_posAttrib);
      }
  }
  */

protected:

  // TODO: liste observer + notify() avec notify() === hasPendingData() ?

  //! \brief Unique identifier. We did not use \m m_name as id because
  //! we want the posibility to change its name.
  Key m_id;

public:

  //! \brief Give a name to the element which will be displayed in the
  //! GUI.  Contrary to id_ several cells can have the same name. By
  //! default the name is unique.
  std::string m_name;

  //! \brief the map structured as a graph.
  SimTaDynGraph_t m_graph;

  // TODO liste de scripts Forth
  // Et qui dit script dit qu'il faudra les gerer dans l'IHM (sorte speedbar)

  //! \brief Spatial index FIXME: drawble rtree
  RTreeNode* m_rtree;

  //! \brief Axis Align Bounding box of the map
  //! FIXME: TEMPORAIRE car sera donner par Rtree.bbox()
  AABB m_bbox;

  //! \brief database FIXME TBD:
  // enum DataField { Position, Color };
  // std::vector<containers*> m_datum
  // or:
  // std::map(string, containers)
  // or:
  // BTree sur des fichiers + convertion(char, type_de_la_colonne)
  // or: clef MySQL
  //Set<Vertex, 8U, Block> m_vertices;
  GLVertexBuffer<Vector3D, simtadyn::graph_container_nb_elements> pos;
  GLVertexBuffer<Color, simtadyn::graph_container_nb_elements> col;
};

#endif /* SIMTADYN_MAP_HPP_ */

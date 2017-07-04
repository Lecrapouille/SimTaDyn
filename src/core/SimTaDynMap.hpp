#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "Vertex.hpp"
#  include "Resources.hpp"
#  include "RTree.hpp"
#  include "SimTaDynGraph.hpp"
#  include "Set.tpp"
#  include "Renderable.hpp"

// FIXME: faut creer une class helper pour charger une seule fois les
// shader commun a tous les cartes (ou alors 1 carte == 1 shader meme
// s'il doit etre mis N fois dans le GPU

//! \brief Abstract class for inheritance: drawable by an OpenGL renderer.
class ISimTaDynMap: public Renderable
{
};

// *************************************************************************************************
//! \brief This class defines a SimTaDyn geographic map. It contains, nodes, arcs, Forth scripts,
//! OpenGL datum for the display. It immplements the observable (aka listenable) design pattern:
//! When the map changes, it notifies to all observers which have subscribed to this map (for example
//! the MVC design pattern, where SimTaDynMap is the model).
// *************************************************************************************************
class SimTaDynMap
  : public ISimTaDynMap,
    public IResource<Key>,
    private ClassCounter<SimTaDynMap>
{
  friend class MapEditor;

public:

  // ***********************************************************************************************
  //! \brief This interface is used to define methods
  // ***********************************************************************************************
  class ISimTaDynMapListener
  {
  public:

    //! \brief
    ISimTaDynMapListener() { };

    //! \brief Callback when the map changed (loaded, graph modified, etc)
    virtual void onChanged(SimTaDynMap*) = 0;
  };

public:

  //! \brief Empty constructor.
  SimTaDynMap()
    : IResource(),
      m_id(ClassCounter<SimTaDynMap>::count()),
      m_name("Map_" + std::to_string(m_id)),
      m_graph("graph_01")
  {
    LOGI("New SimTaDynMap with generic name '%s' and ID #%u\n", m_name.c_str(), m_id);
  }

  //! \brief Constructor with the desired name for the map.
  SimTaDynMap(std::string const& name)
    : IResource(),
      m_id(ClassCounter<SimTaDynMap>::count()),
      m_name(name),
      m_graph("graph_01")
  {
    LOGI("New SimTaDynMap named '%s' and ID #%u\n", m_name.c_str(), m_id);
  }

  //! \brief Destructor.
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

  //! \brief Return the Axis Aligned Bounding Box containing all elements of the map.
  inline AABB3f const& bbox() const
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

  inline void add(Vector3f const& p)
  {
    LOGI("Add a new node to SimTaDynMap #%u %s\n", m_id, m_name.c_str());
    m_graph.addNode();
    pos.append(p / 10.0f + 0.1f * id());
    col.append(Color(1.0f, 0.0f, 0.0f));
    notify();
  }

  //! \brief Reset all states concerning the map.
  inline void clear()
  {
    m_graph.reset();
    notify();
  }

  //! \brief Attach a new listener to map events.
  void addListener(ISimTaDynMapListener& listener)
  {
    m_listeners.push_back(&listener);
    LOGI("Attaching a listener to the SimTaDynMap #%u %s\n", m_id, m_name.c_str());
  }

  //! \brief Remove the given listener of map events.
  void removeListener(ISimTaDynMapListener& listener)
  {
    LOGI("Detaching a listener from the SimTaDynMap #%u %s\n", m_id, m_name.c_str());
    auto it = std::find(m_listeners.begin(), m_listeners.end(), &listener);
    if (it != m_listeners.end())
      {
        m_listeners.erase(it);
      }
  }

  //! \brief Notify all listeners that the map changed.
  void notify()
  {
    uint32_t i = m_listeners.size();
    LOGI("SimTaDynMap #%u %s has changed and notifies %u listener%s\n",
         m_id, m_name.c_str(), i, (i > 1 ? "s" : ""));
    while (i--)
      {
        m_listeners[i]->onChanged(this);
      }
  }

  virtual void drawnBy(GLRenderer& renderer) const override
  {
    LOGI("SimTaDynMap.drawnBy 0x%x", this);
    LOGI("SimTaDynMap #%u %s drawnBy renderer",  m_id, m_name.c_str());

    if (pos.blocks() != col.blocks())
      {
        LOGI("Incompatible number of elements in VBO");
        return ;
      }

    // Draw nodes
    /*uint32_t i = pos.blocks();
    while (i--)
      {
        col.block(i)->begin();
        renderer.m_colAttrib.begin();

        pos.block(i)->begin();
        renderer.m_posAttrib.begin();
        pos.block(i)->draw(GL_POINTS, renderer.m_posAttrib);
      }*/
  }

protected:

  //! \brief Unique identifier. We did not use \m m_name as id because
  //! we want the posibility to change its name.
  Key m_id;

  //! \brief List of observers attached to this map events.
  std::vector<ISimTaDynMapListener*> m_listeners;

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
  //RTreeNode* m_rtree;

  //! \brief Axis Align Bounding box of the map
  //! FIXME: TEMPORAIRE car sera donner par Rtree.bbox()
  AABB3f m_bbox;

  //! \brief database FIXME TBD:
  // enum DataField { Position, Color };
  // std::vector<containers*> m_datum
  // or:
  // std::map(string, containers)
  // or:
  // BTree sur des fichiers + convertion(char, type_de_la_colonne)
  // or: clef MySQL
  //Set<Vertex, 8U, Block> m_vertices;
  GLVertexBuffer<Vector3f, config::graph_container_nb_elements> pos;
  GLVertexBuffer<Color, config::graph_container_nb_elements> col;
};

class CurrentSimTaDynMap
{
public:

  CurrentSimTaDynMap()
  {
    m_map = nullptr;
  }

  void set(SimTaDynMap* p)
  {
    if (m_map != p)
      {
        m_map = p;
        if (nullptr != p)
          {
            p->notify();
          }
      }
  }

  SimTaDynMap* get()
  {
    return m_map;
  }

protected:

  SimTaDynMap* m_map;
};

#endif /* SIMTADYN_MAP_HPP_ */

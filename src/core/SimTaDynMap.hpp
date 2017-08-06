//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "Vertex.hpp"
//#  include "IResource.tpp"
#  include "ResourceManager.tpp"
#  include "RTree.hpp"
#  include "SimTaDynGraph.hpp"
//#  include "Set.tpp"
#  include "GLCollection.tpp"

class SimTaDynMap;

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

// *************************************************************************************************
//! \brief This class defines a SimTaDyn geographic map. It contains, nodes, arcs, Forth scripts,
//! OpenGL datum for the display. It immplements the observable (aka listenable) design pattern:
//! When the map changes, it notifies to all observers which have subscribed to this map (for example
//! the MVC design pattern, where SimTaDynMap is the model).
// *************************************************************************************************
class SimTaDynMap
  : public IResource<Key>,
    private UniqueID<SimTaDynMap>
{
  friend class MapEditor;

public:

  //! \brief Empty constructor.
  SimTaDynMap()
    : IResource(UniqueID<SimTaDynMap>::getID()),
      m_name("Map_" + std::to_string(m_id)),
      m_graph("graph_01")
  {
    LOGI("New SimTaDynMap with generic name '%s' and ID #%u\n", m_name.c_str(), m_id);
  }

  //! \brief Constructor with the desired name for the map.
  SimTaDynMap(std::string const& name)
    : IResource(UniqueID<SimTaDynMap>::getID()),
      m_name(name),
      m_graph("graph_01")
  {
    LOGI("Creating SimTaDynMap named '%s' with ID #%u\n", m_name.c_str(), m_id);
  }

  //! \brief Destructor.
  ~SimTaDynMap()
  {
    LOGI("Deleting SimTaDynMap #%u named '%s'\n", m_id, m_name.c_str());
  }

  //! \brief Return the unique identifier.
  operator int()
  {
    return m_id;
  }

  // Same interface than TextDocument
  /*inline void title(std::string const& text)
  {
    m_title = text;
  }

  inline std::string title() const
  {
    return m_title;
  }

  inline void filename(std::string const& filename)
  {
    m_filename = filename;
  }

  inline const std::string& filename() const
  {
    return m_filename;
  }*/

  inline bool modified() const
  {
    return m_modified;
  }

  inline void modified(bool const value)
  {
    m_modified = value;
  }

  //! \brief Reset all states concerning the map.
  inline void clear()
  {
    m_graph.reset();
    m_modified = true;
    notify();
  }

  /*FIXME bool save() { }
  bool saveAs(std::string const& filename);
  bool load(std::string const& filename, bool clear = true);*/

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

  inline void addNode(Vector3f const& p)
  {
    LOGI("Add a new node to SimTaDynMap #%u %s\n", m_id, m_name.c_str());
    m_graph.addNode();
    pos.append(p / 10.0f + 0.1f * id());
    col.append(Color(1.0f, 0.0f, 0.0f));
    m_modified = true;
    notify(); // FIXME: mettre a jour AABB mais ici cette fonction est utilisee pour loader shapefile et on connait deja la AABB
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

  #if 0
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
  #endif

protected:

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

  //FIXME std::atomic<bool> m_modified = false;
  bool m_modified = false;

  //! \brief database FIXME TBD:
  // enum DataField { Position, Color };
  // std::vector<containers*> m_datum
  // or:
  // std::map(string, containers)
  // or:
  // BTree sur des fichiers + convertion(char, type_de_la_colonne)
  // or: clef MySQL
  //Set<Vertex, 8U, Block> m_vertices;
  GLVertexCollection<Vector3f, config::graph_container_nb_elements> pos;
  GLVertexCollection<Color, config::graph_container_nb_elements> col;
};

// ***********************************************************************************************
//! \brief A class holding the currently edited SimTaDynMap. When
//! the user changes of map, this class will notifies to observers that
//! map changed.
// ***********************************************************************************************
class SimTaDynMapHolder
{
public:

  SimTaDynMapHolder()
  {
    m_map = nullptr;
  }

  void set(SimTaDynMap* p)
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_map != p)
      {
        ResourceManager<Key> &rm =
          ResourceManager<Key>::instance();
        if (nullptr != m_map)
          {
            //FIXME MapEditor::save();
            rm.dispose(m_map->id());
          }
        m_map = p;
        if (nullptr != p)
          {
            rm.acquire(m_map->id());
            p->notify(); // TODO ---> DrawingArea::onNotify(){>attachModel(*map);}
          }
      }
  }

  SimTaDynMap* get()
  {
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_map;
  }

protected:

  SimTaDynMap* m_map;
  std::mutex m_mutex;
};

#endif /* SIMTADYN_MAP_HPP_ */

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

#  include "ResourceManager.tpp"
#  include "SimTaDynGraph.hpp"
#  include "SceneGraph.tpp"

class SimTaDynMap;
typedef SceneNode<SimTaDynGraph, float, 3U> SceneNode_t;

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
      m_name("Map_" + std::to_string(m_id))
  {
    LOGI("New SimTaDynMap with generic name '%s' and ID #%u\n", m_name.c_str(), m_id);
  }

  //! \brief Constructor with the desired name for the map.
  SimTaDynMap(std::string const& name)
    : IResource(UniqueID<SimTaDynMap>::getID()),
      m_name(name)
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

  void clear()
  {
    // TODO
  }

  SimTaDynGraph* graph()
  {
    if (nullptr == m_graphs.root())
      return nullptr;

    return m_graphs.root()->mesh();
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

  //! \brief For debug purpose.
  /*virtual void debug() //FIXME const
  {
    std::cout << "I am SimTaDynMap #" << m_id << " named '" << m_name << "':"
              << std::endl << "{"
              << std::endl << "  " << m_graphs.bbox() << std::endl
              << "  List of nodes (" << m_graphs.nodes().blocks() << " blocks):"
              << std::endl;

    // FIXME: ajouter un forEach
    auto end = m_graphs.nodes().end();
    auto it = m_graphs.nodes().begin();
    for (; it != end; ++it)
      {
        std::cout << "    " << (*it) << " " << pos[it->m_dataKey]
                  << std::endl;
      }
    std::cout << "}" << std::endl;
    }*/

  inline bool modified() const
  {
    return (m_nb_graphs_modified > 0U) || (m_nb_scripts_modified > 0U);
  }

public:

  void draw()
  {
    LOGI("SimTaDynMap.drawnBy 0x%x", this);
    LOGI("SimTaDynMap #%u %s drawnBy renderer",  m_id, m_name.c_str());

    if (nullptr != m_graphs.root())
      draw(*(m_graphs.root()));
  }

private:

  /* FIXME
  void setUniform(const char *name, Matrix44f const &mat)
  {
    GLint id  = glCheck(glGetUniformLocation(m_shader, name));
    glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
    }*/

  void draw(SceneNode_t &node)
  {
    LOGI("Renderer:drawNode '%s'", node.m_name.c_str());

    SimTaDynGraph *m = node.mesh();
    if (nullptr != m)
      {
        //Matrix44f transform = matrix::scale(node.worldTransform(), node.localScale());
        //setUniform("model", transform);
        m->draw(GL_POINTS);
      }

    std::vector<SceneNode_t*> const &children = node.children();
    for (auto i: children)
      {
        draw(*i);
      }
  }

protected:

  //! \brief List of observers attached to this map events.
  std::vector<ISimTaDynMapListener*> m_listeners;

public:

  //! \brief Give a name to the element which will be displayed in the
  //! GUI.  Contrary to id_ several cells can have the same name. By
  //! default the name is unique.
  std::string m_name;

  //! \brief the map structured as a graph.
  SceneGraph<SimTaDynGraph, float, 3U> m_graphs; // FIXME *m_graphs ???

  //! \brief List of Forth scripts.
  std::vector<std::string> m_scripts_forth;

  std::string               m_zip_path;
  std::string               m_base_dir;
  std::string               m_full_path;

  uint32_t m_nb_graphs_modified = 0U;
  uint32_t m_nb_scripts_modified = 0U;
};

// *************************************************************************************************
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

    if (m_map == p)
      return ;

    ResourceManager<Key> &rm =
      ResourceManager<Key>::instance();

    //FIXME MapEditor::save();
    rm.dispose(m_map->id());

    if (nullptr != p)
      {
        m_map = p;
        rm.acquire(m_map->id());
        //m_map->notify(); // TODO ---> DrawingArea::onNotify(){>attachModel(*map);} mais PendingData le fait deja
        // TODO: notify to SimForth to get the address of the scenegraph<SimTaDynGraph>
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

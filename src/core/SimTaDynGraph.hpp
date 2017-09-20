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

#ifndef SIMTADYNGRAPH_HPP_
#  define SIMTADYNGRAPH_HPP_

#  include "CellForth.hpp"
#  include "Graph.hpp"
#  include "BoundingBox.tpp"
#  include "IResource.tpp"

// **************************************************************
//
// **************************************************************
class SimTaDynNode: public BasicNode, public CellForth
{
public:

  SimTaDynNode()
    : BasicNode(), CellForth()
  {
    init();
  }

  SimTaDynNode(const Key nodeID)
    : BasicNode(nodeID), CellForth()
  {
    init();
  }

  SimTaDynNode(BasicNode const& node)
    : BasicNode(node), CellForth()
  {
    init();
  }

protected:

  inline void setName() override
  {
    m_name = "Node" + std::to_string(id());
  }

private:

  inline void init()
  {
    setName();
    m_dataKey = id();
  }

public:

  //! \brief Key to database
  Key m_dataKey;
};

// **************************************************************
//
// **************************************************************
class SimTaDynArc: public BasicArc, public CellForth
{
public:

  SimTaDynArc()
    : BasicArc(), CellForth()
  {
    init();
  }

  SimTaDynArc(const Key id, BasicNode& fromNode, BasicNode& toNode)
    : BasicArc(id, fromNode, toNode), CellForth()
  {
    init();
  }

  SimTaDynArc(BasicArc const& arc)
    : BasicArc(arc), CellForth()
  {
    init();
  }

protected:

  inline void setName()
  {
    m_name = "Arc" + std::to_string(id());
  }

private:

  inline void init()
  {
    setName();
    m_dataKey = id();
  }

public:

  //! \brief Key to database
  Key m_dataKey;
};

#  include "Vertex.hpp"
#  include "GLCollection.tpp"
#  include "RTree.hpp"

// **************************************************************
//
// **************************************************************
class SimTaDynGraph
  : public BasicGraph<SimTaDynNode, SimTaDynArc>,
    public IResource<Key>,
    private UniqueID<SimTaDynGraph>
{
public:

  SimTaDynGraph(const bool directed = true)
    : BasicGraph<SimTaDynNode, SimTaDynArc>(directed),
      IResource(UniqueID<SimTaDynGraph>::getID())
  {
    LOGI("New SimTaDynGraph with generic name '%s' and ID #%u\n", m_name.c_str(), m_id);
  }

  SimTaDynGraph(std::string const& name,
                const bool directed = true)
    : BasicGraph<SimTaDynNode, SimTaDynArc>(name, directed),
      IResource(UniqueID<SimTaDynGraph>::getID())
  {
    LOGI("Creating SimTaDynGraph named '%s' with ID #%u\n", m_name.c_str(), m_id);
  }

  SimTaDynGraph(const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : BasicGraph<SimTaDynNode, SimTaDynArc>(noNodes, noArcs, directed),
      IResource(UniqueID<SimTaDynGraph>::getID())
  {
    LOGI("New SimTaDynGraph with generic name '%s' and ID #%u\n", m_name.c_str(), m_id);
  }

  SimTaDynGraph(std::string const& name,
                const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : BasicGraph<SimTaDynNode, SimTaDynArc>(name, noNodes, noArcs, directed),
      IResource(UniqueID<SimTaDynGraph>::getID())
  {
    LOGI("Creating SimTaDynGraph named '%s' with ID #%u\n", m_name.c_str(), m_id);
  }

  //! \brief Destructor.
  ~SimTaDynGraph()
  {
    LOGI("Deleting SimTaDynGraph #%u named '%s'\n", m_id, m_name.c_str());
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

  SimTaDynNode& addNode(Vector3f const& p)
  {
    LOGI("Add a new node to SimTaDynGraph #%u %s\n", m_id, m_name.c_str());
    SimTaDynNode& n = BasicGraph<SimTaDynNode, SimTaDynArc>::addNode();
    pos.append(p / 10.0f + 0.1f * id());
    col.append(Color(1.0f, 0.0f, 0.0f));

    return n;
  }

  void draw(GLuint const /*type*/)
  {
    /*LOGI("SimTaDynMap.drawnBy 0x%x", this);
    LOGI("SimTaDynMap #%u %s drawnBy renderer",  m_id, m_name.c_str());

    if (pos.blocks() != col.blocks())
      {
        LOGI("Incompatible number of elements in VBO");
        return ;
      }

    // Draw nodes
    uint32_t i = pos.blocks();
    while (i--)
      {
        col.block(i)->begin();
        renderer.m_colAttrib.begin();

        pos.block(i)->begin();
        renderer.m_posAttrib.begin();
        pos.block(i)->draw(GL_POINTS, renderer.m_posAttrib);
      }*/
  }

public: // FIXME

  //! \brief Spatial index: drawble rtree
  //RTreeNode* m_rtree;
  //! \brief Axis Align Bounding box of the map
  //! FIXME: TEMPORAIRE car sera donner par Rtree.bbox()
  AABB3f m_bbox;
  std::string m_name;
  GLVertexCollection<Vector3f, config::graph_container_nb_elements> pos;
  GLVertexCollection<Color, config::graph_container_nb_elements> col;
};

#endif /* SIMTADYNGRAPH_HPP_ */

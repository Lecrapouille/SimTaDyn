//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef CLASSIC_SPREADSHEET_HPP_
#  define CLASSIC_SPREADSHEET_HPP_

#  include "Resource.hpp"
#  include "SimTaDynCell.hpp"
//#  include "GraphAlgorithm.hpp"
#  include "BoundingBox.tpp"
#  include "Vertex.hpp"
#  include "GLCollection.tpp"

//FIXME
#include "algorithm/SimTaDynBFS.hpp"

using namespace graphtheory;

// **************************************************************
//
// **************************************************************
class SimTaDynSheet
  : public ASpreadSheet,
    public SimTaDynGraph<CellNode, CellArc, CellZone>,
    public Resource,
    private UniqueID<SimTaDynSheet>
{
public:

  enum MapProjection { NoMapProjection, WGS84Projection };

  SimTaDynSheet(const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(directed),
      m_name("No name")
  {
    LOGI("New SimTaDynSheet with generic name '%s' and ID #%u\n",
         this->name().c_str(), getID());
  }

  SimTaDynSheet(std::string const& name, const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(directed),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), getID());
  }

  SimTaDynSheet(const char* name, const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(directed),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), getID());
  }

  SimTaDynSheet(const size_t noNodes,
                const size_t noArcs,
                const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(noNodes, noArcs, directed),
      m_name("No name")
  {
    LOGI("New SimTaDynSheet with generic name '%s' and ID #%u\n",
         this->name().c_str(), getID());
  }

  SimTaDynSheet(std::string const& name,
                const size_t noNodes,
                const size_t noArcs,
                const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(noNodes, noArcs, directed),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), getID());
  }

  SimTaDynSheet(const char *name,
                const size_t noNodes,
                const size_t noArcs,
                const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(noNodes, noArcs, directed),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), getID());
  }

  //! \brief Destructor.
  ~SimTaDynSheet()
  {
    LOGI("Deleting SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), getID());
  }

  virtual const std::string& name() const override
  {
    return m_name;
  }

  virtual const std::string& path() const
  {
    return m_file_path;
  }

  //! \brief Return the unique identifier.
  operator size_t()
  {
    return getID();
  }

  inline MapProjection mapProjectionType() const
  {
    return m_projection_type;
  }

  void draw(GLuint const /*type*/)
  {
    /*LOGI("SimTaDynMap.drawnBy 0x%x", this);
    LOGI("SimTaDynMap #%u %s drawnBy renderer",  getID(), m_name.c_str());

    if (pos.blocks() != col.blocks())
      {
        LOGI("Incompatible number of elements in VBO");
        return ;
      }

    // Draw nodes
    size_t i = pos.blocks();
    while (i--)
      {
        col.block(i)->begin();
        renderer.m_colAttrib.begin();

        pos.block(i)->begin();
        renderer.m_posAttrib.begin();
        pos.block(i)->draw(GL_POINTS, renderer.m_posAttrib);
      }*/
  }

  std::string m_name;
  std::string m_file_path = "FIXME";
  MapProjection m_projection_type = MapProjection::NoMapProjection;

protected:

  virtual void resetCellIterator() override
  {
    /* FIXME if (nullptr == m_graphAlgorithm)
      {
        LOGW("GraphAlgorithm was not specified. Select default one: BFS");
        m_graphAlgorithm = GraphAlgorithm::factory("SimBFS");
      }
    */
    LOGI("resetCellIterator()");
    if (hasNode(0u))
      {
        m_graphAlgorithm.init(*this, getNode(0u), false);
      }
    else
      {
        LOGE("PAS de NOEUD PAS DE CHOCOLAT");
      }
  }

  virtual bool hasCell() const override
  {
    //assert(nullptr != m_graphAlgorithm);
    LOGI("hasCell(): %u", !m_graphAlgorithm.finished());
    return !m_graphAlgorithm.finished();
  }

  virtual ASpreadSheetCell* nextCell() /*const*/ override
  {
    GraphElement *ge = const_cast<GraphElement *>(m_graphAlgorithm.update());
    assert(nullptr != ge);

    CellNode* cell = reinterpret_cast<CellNode*>(ge);
    LOGI("nextCell():Cell: %p %s %u", cell, cell->name().c_str(), cell->id());

    return cell;
  }

  virtual size_t howManyCells() const override
  {
    LOGI("howManyCell(): %u", howManyNodes() + howManyArcs() + howManyZones());
    return howManyNodes() + howManyArcs() + howManyZones();
  }

  virtual ASpreadSheetCell *isACell(std::string const& word) override;

  /*ASpreadSheetCell& addCell(std::string const& formulae)
  {
    ASpreadSheetCell& node = SimTaDynGraph<CellNode, CellArc, CellZone>::addNode();
    node.setFormulae(formulae);
    std::cout << "New Node " << node.id() << std::endl;
    return node;
    }*/

  //! \brief Return the Axis Aligned Bounding Box containing all elements of the map.
  inline AABB3f const& bbox() const
  {
    // FIXME: a faire par la suite m_spatial_index.root.bbox()
    return m_bbox;
  }

public:

  //! \brief Spatial index: drawble rtree
  //RTreeNode* m_rtree;
  //! \brief Axis Align Bounding box of the map
  //! FIXME: TEMPORAIRE car sera donner par Rtree.bbox()
  AABB3f m_bbox;

private:

  //FIXME std::shared_ptr<GraphAlgorithm> m_graphAlgorithm = nullptr;
  GraphAlgorithmSimTaDynBFS m_graphAlgorithm;
  std::vector<ASpreadSheetCell*> m_cells;
};

using SimTaDynSheetPtr = std::shared_ptr<SimTaDynSheet>;

#endif

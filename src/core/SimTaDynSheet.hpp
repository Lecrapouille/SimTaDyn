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

#ifndef CLASSIC_SPREADSHEET_HPP_
#  define CLASSIC_SPREADSHEET_HPP_

#  include "Resource.hpp"
#  include "SimTaDynCell.hpp"
#  include "SimTaDynGraph.hpp"
//#  include "GraphAlgorithm.hpp"
#  include "BoundingBox.tpp"
#  include "Vertex.hpp"
#  include "GLCollection.tpp"
#  include "Logger.hpp"

//FIXME
#include "algorithm/SimTaDynBFS.hpp"

using namespace graphtheory;

// **************************************************************
//
// **************************************************************
class SimTaDynSheet
  : public ASpreadSheet,
    public SimTaDynGraph<CellNode, CellArc, CellZone>,
    public IResource<Key>,
    private UniqueID<SimTaDynSheet>
{
public:

  SimTaDynSheet(const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(directed),
      IResource(UniqueID<SimTaDynSheet>::getID()),
      m_name("No name")
  {
    LOGI("New SimTaDynSheet with generic name '%s' and ID #%u\n",
         this->name().c_str(), m_id);
  }

  SimTaDynSheet(std::string const& name, const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(directed),
      IResource(UniqueID<SimTaDynSheet>::getID()),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), m_id);
  }

  SimTaDynSheet(const char* name, const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(directed),
      IResource(UniqueID<SimTaDynSheet>::getID()),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), m_id);
  }

  SimTaDynSheet(const uint32_t noNodes,
                const uint32_t noArcs,
                const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(noNodes, noArcs, directed),
      IResource(UniqueID<SimTaDynSheet>::getID()),
      m_name("No name")
  {
    LOGI("New SimTaDynSheet with generic name '%s' and ID #%u\n",
         this->name().c_str(), m_id);
  }

  SimTaDynSheet(std::string const& name,
                 const uint32_t noNodes,
                 const uint32_t noArcs,
                 const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(noNodes, noArcs, directed),
      IResource(UniqueID<SimTaDynSheet>::getID()),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), m_id);
  }

  SimTaDynSheet(const char *name,
                 const uint32_t noNodes,
                 const uint32_t noArcs,
                 const bool directed = true)
    : ASpreadSheet(),
      SimTaDynGraph<CellNode, CellArc, CellZone>(noNodes, noArcs, directed),
      IResource(UniqueID<SimTaDynSheet>::getID()),
      m_name(name)
  {
    LOGI("Creating SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), m_id);
  }

  //! \brief Destructor.
  ~SimTaDynSheet()
  {
    LOGI("Deleting SimTaDynSheet named '%s' with ID #%u\n",
         this->name().c_str(), m_id);
  }

  virtual const std::string& name() const override
  {
    return m_name;
  }

  //! \brief Return the unique identifier.
  operator int()
  {
    return m_id;
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

  std::string m_name;

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

  virtual uint32_t howManyCells() const override
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

#endif

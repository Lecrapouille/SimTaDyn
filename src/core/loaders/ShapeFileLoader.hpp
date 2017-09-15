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

#ifndef SHAPEFILELOADER_HPP_
#  define SHAPEFILELOADER_HPP_

#  include "ILoader.hpp"
#  include "SimTaDynMap.hpp"

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class ShapefileLoader : public ILoader<SimTaDynMap>
{
public:

  ShapefileLoader()
    : ILoader<SimTaDynMap>("Shapefile")
  {
    LOGI("Creating a Shapefile loader %p", this);
  }
  virtual void loadFromFile(std::string const& filename, SimTaDynMap* &map) override;

protected:

  int32_t      readBigEndianInt();
  int32_t      readLittleEndianInt();
  double       readDouble();
  float        readDoubleCastedFloat();
  void         goToByte(const uint32_t offset);
  void         skypeNBytes(const uint32_t offset);
  const std::string shapeTypes(const int id);
  void         checkFileSize();
  void         openShapeFile(const std::string& filename);
  uint32_t     getShapeVersion();
  uint32_t     getShapeType();
  void         getBoundingBox(Vector3f& bbox_min, Vector3f& bbox_max);
  void         getBoundingBox(AABB3f& bbox);
  uint32_t     getRecordAt(SimTaDynMap& map, const uint32_t offset);
  void         getAllRecords(SimTaDynMap& map);

  std::ifstream m_infile;
  std::string   m_filename;
  uint32_t      m_file_length = 0U;

private:

  Vector3f      m_point;
};

#endif /* SHAPEFILELOADER_HPP_ */

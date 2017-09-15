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

#include "ShapeFileLoader.hpp"

// ESRI Shapefile Technical Description:
// https://www.esri.com/library/whitepapers/pdfs/shapefile.pdf

int32_t ShapefileLoader::readBigEndianInt()
{
  char buf[4];

  m_infile.read(buf, 4);
  return (((buf[0] & 0xFF) << 24) | ((buf[1] & 0xFF) << 16) | ((buf[2] & 0xFF) << 8) | (buf[3] & 0xFF));
}

int32_t ShapefileLoader::readLittleEndianInt()
{
  char buf[4];

  m_infile.read(buf, 4);
  return (((buf[3] & 0xFF) << 24) | ((buf[2] & 0xFF) << 16) | ((buf[1] & 0xFF) << 8) | (buf[0] & 0xFF));
}

double ShapefileLoader::readDouble()
{
  union {
    char buf[8];
    double value;
  } u;
  double convert;

  m_infile.read(u.buf, 8);
  convert = u.value;
  return convert;
}

float ShapefileLoader::readDoubleCastedFloat()
{
  double val = readDouble();
  return static_cast<float>(val);
}

void ShapefileLoader::goToByte(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::beg);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderException");
      LoaderException e("Failed parsing the shapefile '" + m_filename
                        + "' at offset " + std::to_string(offset));
      throw e;
    }
}

void ShapefileLoader::skypeNBytes(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::cur);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderException");
      LoaderException e("Failed parsing the shapefile '" + m_filename
                        + "' at offset " + std::to_string(offset));
      throw e;
    }
}

const std::string ShapefileLoader::shapeTypes(const int id)
{
  std::string shape;

  switch (id)
    {
    case 0:
      shape = "Null Shape";
      break;
    case 1:
      shape = "Point";
      break;
    case 3:
      shape = "Poly Line";
      break;
    case 5:
      shape = "Polygon";
      break;
    case 8:
      shape = "MultiPoint";
      break;
    case 11:
      shape = "PointZ";
      break;
    case 13:
      shape = "PolyLineZ";
      break;
    case 15:
      shape = "PolygonZ";
      break;
    case 18:
      shape = "MultiPointZ";
      break;
    case 21:
      shape = "PointM";
      break;
    case 23:
      shape = "PolyLineM";
      break;
    case 25:
      shape = "PolygonM";
      break;
    case 28:
      shape = "MultiPointM";
      break;
    case 31:
      shape = "MultiPatch";
      break;
    default:
      shape = "Wrong match found";
      break;
    }
  return shape;
}

void ShapefileLoader::checkFileSize()
{
  uint32_t value32b;

  // Get the file size from the file system
  m_infile.seekg(0, std::ios::end);
  m_file_length = (uint32_t)  m_infile.tellg();
  m_infile.seekg(0, std::ios::beg);

  // Get the file size stored inside the file
  goToByte(24);
  value32b = readBigEndianInt() * sizeof (uint16_t);

  // Expect to have same values
  if (value32b == m_file_length)
    {
      //std::cout << "File Length: " << m_file_length << std::endl;
    }
  else
    {
      LOGF("ShapefileLoaderBadLength");
      std::string msg("Incorrect Shapefile size. Expected ");
      msg += std::to_string(m_file_length);
      msg += " bytes but detected ";
      msg += value32b;
      msg += " bytes";
      LoaderException e(msg);
      throw e;
    }
}

void ShapefileLoader::openShapeFile(const std::string& filename)
{
  uint32_t value32b;

  m_file_length = 0;
  m_filename = filename;

  m_infile.open(filename, std::ios::binary | std::ios::in);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderOpenFailed");
      LoaderException e("Failed opening the Shapefile '" + filename + "': " + strerror(errno));
      throw e;
    }

  checkFileSize();

  goToByte(0U);
  value32b = readBigEndianInt();
  if (9994 != value32b)
    {
      LOGF("ShapefileLoaderBadId");
      LoaderException e("Bad Shapefile ID: read " + std::to_string(value32b) + " instead of 9994");
      throw e;
    }
}

uint32_t ShapefileLoader::getShapeVersion()
{
  goToByte(28U);
  return readLittleEndianInt();
}

uint32_t ShapefileLoader::getShapeType()
{
  goToByte(32U);
  return readLittleEndianInt();
}

void ShapefileLoader::getBoundingBox(Vector3f& bbox_min, Vector3f& bbox_max)
{
  goToByte(36U);
  bbox_min.x = readDoubleCastedFloat();
  bbox_min.y = readDoubleCastedFloat();
  bbox_max.x = readDoubleCastedFloat();
  bbox_max.y = readDoubleCastedFloat();
  bbox_min.z = readDoubleCastedFloat();
  bbox_max.z = readDoubleCastedFloat();
}

void ShapefileLoader::getBoundingBox(AABB3f& bbox)
{
  goToByte(36U);
  bbox.m_bbmin.x = readDoubleCastedFloat();
  bbox.m_bbmin.y = readDoubleCastedFloat();
  bbox.m_bbmax.x = readDoubleCastedFloat();
  bbox.m_bbmax.y = readDoubleCastedFloat();
  bbox.m_bbmin.z = readDoubleCastedFloat();
  bbox.m_bbmax.z = readDoubleCastedFloat();
}

uint32_t ShapefileLoader::getRecordAt(SimTaDynMap& map, const uint32_t offset)
{
  uint32_t record_number, content_length, shape_type;

  goToByte(offset);
  record_number = readBigEndianInt();
  content_length = readBigEndianInt() * sizeof (uint16_t);
  shape_type = readLittleEndianInt();

  //std::cout << "Record Number: " << record_number << ", Content Length: " << content_length << ":" << std::endl;
  //std::cout << "  Shape " << record_number - 1U << " (" << shapeTypes(shape_type) << "): ";
  (void) record_number;

  switch (shape_type)
    {
    case 1: // Point
      m_point.x = readDoubleCastedFloat();
      m_point.y = readDoubleCastedFloat();
      m_point.z = 0.0f;
      map.addNode(m_point);
      break;
    case 11: // PointZ
      m_point.x = readDoubleCastedFloat();
      m_point.y = readDoubleCastedFloat();
      m_point.z = readDoubleCastedFloat();
      map.addNode(m_point);
      break;
    default:
      std::cout << "  Shape " << shapeTypes(shape_type) << " not yet managed. Ignored !" << std::endl;
      skypeNBytes(content_length);
      break;
    }
  return content_length + 8U; // 8U: record header
}

void ShapefileLoader::getAllRecords(SimTaDynMap& map)
{
  uint32_t content_length;
  uint32_t offset = 100U;

  while (offset < m_file_length)
    {
      if (m_infile.eof())
        break;

      content_length = getRecordAt(map, offset);
      offset += content_length;
    }
}

void ShapefileLoader::loadFromFile(std::string const& filename, SimTaDynMap* &current_map)
{
  bool dummy_map = (nullptr == current_map);

  LOGI("Loading the shapefile '%s' in an %s",
       filename.c_str(), (dummy_map ? "dummy map" : "already opened map"));

  try
    {
      openShapeFile(filename);

      uint32_t value32b = getShapeVersion();
      if (1000 != value32b)
        {
          LOGWS("Expected shapefile version 1000 but found %u: the file '%s' may be not fully interpreted",
                value32b, filename.c_str());
        }

      value32b = getShapeType();
      LOGI("Shapefile Type: %u: %s", value32b, shapeTypes(value32b).c_str());

      std::string shortname = File::fileName(filename);
      SimTaDynMap *map = new SimTaDynMap(shortname);

      getBoundingBox(map->m_bbox);
      // FIXME CPP_LOG(logger::Info) << "Map Bounding Box: " << map->m_bbox << std::endl;

      getAllRecords(*map);
      m_infile.close();

      if (dummy_map)
        {
          current_map = map;
        }
      else
        {
          // Concat the old map with the new one: elements, name and bounding box
          current_map += *map;
          current_map->m_bbox = merge(current_map->m_bbox, map->m_bbox); // TODO a mettre dans le code de +=

          if (map->m_name != "") // TODO a mettre dans le code de += avec option
            map->m_name += "_";
          map->m_name += shortname;
        }
    }
  catch (std::exception const &e)
    {
      m_infile.close();
      throw e;
    }
}

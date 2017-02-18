#include "ShapeFile.hpp"

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

float64_t ShapefileLoader::readDouble()
{
  union {
    char buf[8];
    float64_t value;
  } u;
  float64_t convert;

  m_infile.read(u.buf, 8);
  convert = u.value;
  return convert;
}

float32_t ShapefileLoader::readDoubleCastedFloat()
{
  float64_t val = readDouble();
  return static_cast<float32_t>(val);
}

void ShapefileLoader::goToByte(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::beg);
  if (!m_infile)
    {
      ShapefileLoaderException e(offset, "ShapefileLoader::goToByte");
      throw e;
    }
}
void ShapefileLoader::skypeNBytes(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::cur);
  if (!m_infile)
    {
      ShapefileLoaderException e(offset, "ShapefileLoader::skypeNBytes");
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
      ShapefileLoaderBadLength e(m_file_length, value32b);
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
      ShapefileLoaderOpenFailed e(errno);
      throw e;
    }

  checkFileSize();

  goToByte(0U);
  value32b = readBigEndianInt();
  if (9994 != value32b)
    {
      ShapefileLoaderBadId e(value32b, 9994);
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

void ShapefileLoader::getBoundingBox(Position3D& bbox_min, Position3D& bbox_max)
{
  goToByte(36U);
  bbox_min.x = readDoubleCastedFloat();
  bbox_min.y = readDoubleCastedFloat();
  bbox_max.x = readDoubleCastedFloat();
  bbox_max.y = readDoubleCastedFloat();
  bbox_min.z = readDoubleCastedFloat();
  bbox_max.z = readDoubleCastedFloat();
}

void ShapefileLoader::getBoundingBox(AABB& bbox)
{
  goToByte(36U);
  bbox.bbmin.x = readDoubleCastedFloat();
  bbox.bbmin.y = readDoubleCastedFloat();
  bbox.bbmax.x = readDoubleCastedFloat();
  bbox.bbmax.y = readDoubleCastedFloat();
  bbox.bbmin.z = readDoubleCastedFloat();
  bbox.bbmax.z = readDoubleCastedFloat();
}

uint32_t ShapefileLoader::getRecordAt(SimTaDynMap& map, const uint32_t offset)
{
  uint32_t record_number, content_length, shape_type;
  Position3D p;

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
      p.x = readDoubleCastedFloat();
      p.y = readDoubleCastedFloat();
      p.z = 0.0f;
      map.addNode(p);
      break;
    case 11: // PointZ
      p.x = readDoubleCastedFloat();
      p.y = readDoubleCastedFloat();
      p.z = readDoubleCastedFloat();
      map.addNode(p);
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
      //std::cout << "Total bytes read: " << offset << std::endl;
    }
}

bool ShapefileLoader::load(std::string const& filename, SimTaDynMap* &map)
{
  bool ret = false;

  try // FIXME: comment faire undo en cas d'echec ?
    {
      uint32_t value32b;

      openShapeFile(filename);
      value32b = getShapeVersion();
      if (1000 != value32b)
        {
          std::cerr << "Warning. Expected shapefile version 1000 not found. The file '" << filename << "' may be not fully interpreted" << std::endl;
          goto l_err;
        }

      if (nullptr == map)
        {
          map = new SimTaDynMap(filename);
        }
      else
        {
          // Concat the old map name with the new one
          std::string shortname = SimTaDynMap::shortName(filename);
          map->m_name += '_' + shortname;
        }

      value32b = getShapeType();
      //std::cout << "Shape Type: " << value32b << ": " << shapeTypes(value32b) << std::endl;

      getBoundingBox(map->m_bbox);
      std::cout << "Map Bounding Box: " << map->m_bbox << std::endl;

      getAllRecords(*map);
      m_error = "no error";
      ret = true;
    }
  catch (const ShapefileLoaderBadLength& e)
    {
      m_error = "The real file size is " + std::to_string(e.m_real_size)
        + " ko but the extracted size information is " + std::to_string(e.m_expected_size)
        + " bytes.";
    }
  catch (const ShapefileLoaderBadId& e)
    {
      m_error = "The expected file identifier is " + std::to_string(e.m_expected_id)
        + " but the identifier read is " + std::to_string(e.m_bad_id)
        + ". The file '" + m_filename + "' seems not to be a proper .shp file.";
    }
  catch (const ShapefileLoaderOpenFailed& e)
    {
      m_error = strerror(e.m_error) + '.';
      return ret;
    }

l_err:
  m_infile.close();
  return ret;
}

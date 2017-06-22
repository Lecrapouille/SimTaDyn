#include "ShapeFile.hpp"
#include "File.hpp"

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
      ShapefileLoaderException e(offset, "ShapefileLoader::goToByte");
      throw e;
    }
}
void ShapefileLoader::skypeNBytes(const uint32_t offset)
{
  m_infile.seekg(offset, std::ios::cur);
  if (!m_infile)
    {
      LOGF("ShapefileLoaderException");
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
      LOGF("ShapefileLoaderException");
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
      LOGF("ShapefileLoaderException");
      ShapefileLoaderOpenFailed e(errno);
      throw e;
    }

  checkFileSize();

  goToByte(0U);
  value32b = readBigEndianInt();
  if (9994 != value32b)
    {
      LOGF("ShapefileLoaderException");
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
      map.add(m_point);
      break;
    case 11: // PointZ
      m_point.x = readDoubleCastedFloat();
      m_point.y = readDoubleCastedFloat();
      m_point.z = readDoubleCastedFloat();
      map.add(m_point);
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

  LOGI("Loading the shapefile '%s' in an %s", filename.c_str(), ((nullptr == map) ? "empty map" : "already opened map"));
  try // FIXME: comment faire undo en cas d'echec ?
    {
      uint32_t value32b;

      openShapeFile(filename);
      value32b = getShapeVersion();
      if (1000 != value32b)
        {
          LOGIS("Expected shapefile version 1000 not found. The file '%s' may be not fully interpreted",
                filename.c_str());
          goto l_err;
        }

      std::string shortname = File::shortNameWithExtension(filename);
      if (nullptr == map)
        {
          map = new SimTaDynMap(shortname);
        }
      else
        {
          // Concat the old map name with the new one
          if (map->m_name != "")
            map->m_name += "_";
          map->m_name += shortname;
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

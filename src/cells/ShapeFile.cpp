#include "ShapeFile.hpp"

// ESRI Shapefile Technical Description:
// https://www.esri.com/library/whitepapers/pdfs/shapefile.pdf

int32_t ShapefileLoader::readBigEndianInt()
{
  char buf[4];

  infile_.read(buf, 4);
  return (((buf[0] & 0xFF) << 24) | ((buf[1] & 0xFF) << 16) | ((buf[2] & 0xFF) << 8) | (buf[3] & 0xFF));
}

int32_t ShapefileLoader::readLittleEndianInt()
{
  char buf[4];

  infile_.read(buf, 4);
  return (((buf[3] & 0xFF) << 24) | ((buf[2] & 0xFF) << 16) | ((buf[1] & 0xFF) << 8) | (buf[0] & 0xFF));
}

float64_t ShapefileLoader::readDouble()
{
  union {
    char buf[8];
    float64_t value;
  } u;
  float64_t convert;

  infile_.read(u.buf, 8);
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
  infile_.seekg(offset, ios::beg);
  if (!infile_)
    {
      ShapefileLoaderException e;
      throw e;
    }
}
void ShapefileLoader::skypeNBytes(const uint32_t offset)
{
  infile_.seekg(offset, ios::cur);
  if (!infile_)
    {
      ShapefileLoaderException e;
      throw e;
    }
}

const string ShapefileLoader::shapeTypes(const int id)
{
  string shape;

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
  infile_.seekg(0, ios::end);
  file_length_ = (uint32_t)  infile_.tellg();
  infile_.seekg(0, ios::beg);

  // Get the file size stored inside the file
  goToByte(24);
  value32b = readBigEndianInt() * sizeof (uint16_t);

  // Expect to have same values
  if (value32b == file_length_)
    {
      //cout << "File Length: " << file_length_ << endl;
    }
  else
    {
      ShapefileLoaderBadLength e(file_length_, value32b);
      throw e;
    }
}

void ShapefileLoader::openShapeFile(const string& filename)
{
  uint32_t value32b;

  file_length_ = 0;
  filename_ = filename;

  infile_.open(filename, ios::binary | ios::in);
  if (!infile_)
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

uint32_t ShapefileLoader::getRecordAt(SimTaDynGraph& graph, const uint32_t offset)
{
  uint32_t record_number, content_length, shape_type;
  Position3D p;

  goToByte(offset);
  record_number = readBigEndianInt();
  content_length = readBigEndianInt() * sizeof (uint16_t);
  shape_type = readLittleEndianInt();

  //cout << "Record Number: " << record_number << ", Content Length: " << content_length << ":" << endl;
  //cout << "  Shape " << record_number - 1U << " (" << shapeTypes(shape_type) << "): ";
  (void) record_number;

  switch (shape_type)
    {
    case 1: // Point
      p.x = readDoubleCastedFloat();
      p.y = readDoubleCastedFloat();
      p.z = 0.0f;
      //cout << p.x << " " << p.y << " " << p.z << " " << endl;
      graph.addNode(p);
      break;
    case 11: // PointZ
      p.x = readDoubleCastedFloat();
      p.y = readDoubleCastedFloat();
      p.z = readDoubleCastedFloat();
      //cout << p.x << " " << p.y << " " << p.z << " " << endl;
      graph.addNode(p);
      break;
    default:
      cout << "  Shape " << shapeTypes(shape_type) << " not yet managed. Ignored !" << endl;
      skypeNBytes(content_length);
      break;
    }
  return content_length + 8U; // 8U: record header
}

void ShapefileLoader::getAllRecords(SimTaDynGraph& graph)
{
  uint32_t content_length;
  uint32_t offset = 100U;

  while (offset < file_length_)
    {
      if (infile_.eof())
        break;

      content_length = getRecordAt(graph, offset);
      offset += content_length;
      //cout << "Total bytes read: " << offset << endl;
    }
}

bool ShapefileLoader::loadShapefile(const string& filename, SimTaDynGraph& graph) // new graph ?
{
  uint32_t value32b;

  try // FIXME: comment faire undo en cas d'echec ?
    {
      openShapeFile(filename);

      value32b = getShapeVersion();
      //cout << "Shapefile Version: " << value32b << endl;
      if (1000 != value32b)
        {
          cerr << "Warning. Expected shapefile version 1000 not found. The file '" << filename << "' may be not fully interpreted" << endl;
          return false;
        }

      value32b = getShapeType();
      //cout << "Shape Type: " << value32b << ": " << shapeTypes(value32b) << endl;

      getBoundingBox(graph.bbox);
      cout << "Map Bounding Box: " << graph.bbox << endl;

      getAllRecords(graph);
      graph.name += '_' + filename;
      infile_.close();
      return true;
    }
  catch (const ShapefileLoaderBadLength& e)
    {
      cerr << "Failed parsing the shapefile " << filename << ":" << endl;
      cerr << "  The real file size is " << e.real_size << " ko but the extracted size information is "
           << e.expected_size << " ko" << endl;
      infile_.close();
      return false;
    }
  catch (const ShapefileLoaderBadId& e)
    {
      cerr << "Failed parsing the shapefile " << filename << ":" << endl;
      cerr << "  The expected file identifier is " << e.expected_id << " but the identifier read is " << e.bad_id
           << ". The file '" << filename_ << "' seems not to be a proper .shp file." << endl;
      infile_.close();
      return false;
    }
  catch (const ShapefileLoaderOpenFailed& e)
    {
      cerr << "Failed opening the shapefile " << filename << ":" << endl;
      cerr << "  Reason is '" << strerror(e.error) << "'." << endl;
      return false;
    }
}

ShapefileLoader::ShapefileLoader(const string& filename, SimTaDynGraph& graph)
{
  loadShapefile(filename, graph);
}

#include "shapefile.hpp"

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
      cout << "File Length: " << file_length_ << endl;
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
  bbox_min.x = readDouble();
  bbox_min.y = readDouble();
  bbox_max.x = readDouble();
  bbox_max.y = readDouble();
  bbox_min.z = readDouble();
  bbox_max.z = readDouble();
}

uint32_t ShapefileLoader::getRecordAt(SimTaDynGraph& graph, const uint32_t offset)
{
  uint32_t record_number, content_length, shape_type;
  Position3D p;

  goToByte(offset);
  record_number = readBigEndianInt();
  content_length = readBigEndianInt() * sizeof (uint16_t);
  shape_type = readLittleEndianInt();

  cout << "Record Number: " << record_number << ", Content Length: " << content_length << ":" << endl;
  cout << "  Shape " << record_number - 1U << " (" << shapeTypes(shape_type) << "): ";

  switch (shape_type)
    {
    case 11: // PointZ
      p.x = readDouble();
      p.y = readDouble();
      p.z = readDouble();
      cout << p.x << " " << p.y << " " << p.z << " " << endl;
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
      cout << "Total bytes read: " << offset << endl;
    }
}

void ShapefileLoader::loadShapefile(const string& filename, SimTaDynGraph& graph) // new graph ?
{
  uint32_t value32b;

  try // FIXME: comment faire undo en cas d'echec ?
    {
      openShapeFile(filename);

      value32b = getShapeVersion();
      cout << "Shapefile Version: " << value32b << endl;
      if (1000 != value32b)
        {
          cerr << "Warning. Expected shapefile version 1000 not found. The file '" << filename << "' may be not fully interpreted" << endl;
          return ;
        }

      value32b = getShapeType();
      cout << "Shape Type: " << value32b << ": " << shapeTypes(value32b) << endl;

      getBoundingBox(graph.bbox_min, graph.bbox_max);
      cout << "Map Bounding Box:" << endl;
      cout << " Xmin: " << graph.bbox_min.x;
      cout << " Ymin: " << graph.bbox_min.y;
      cout << " Zmin: " << graph.bbox_min.z << endl;
      cout << " Xmax: " << graph.bbox_max.x;
      cout << " Ymax: " << graph.bbox_max.y;
      cout << " Zmax: " << graph.bbox_max.z << endl;

      getAllRecords(graph);
      graph.setName(graph.getName() + '_' + filename);
      infile_.close();
    }
  catch (const ShapefileLoaderBadLength& e)
    {
      cerr << "Failed parsing the shapefile " << filename << ":" << endl;
      cerr << "  The real file size is " << e.real_size << " ko but the extracted size information is "
           << e.expected_size << " ko" << endl;
      infile_.close();
    }
  catch (const ShapefileLoaderBadId& e)
    {
      cerr << "Failed parsing the shapefile " << filename << ":" << endl;
      cerr << "  The expected file identifier is " << e.expected_id << " but the identifier read is " << e.bad_id
           << ". The file '" << filename_ << "' seems not to be a proper .shp file." << endl;
      infile_.close();
    }
  catch (const ShapefileLoaderOpenFailed& e)
    {
      cerr << "Failed opening the shapefile " << filename << ":" << endl;
      cerr << "  Reason is '" << strerror(e.error) << "'." << endl;
    }
}

ShapefileLoader::ShapefileLoader(const string& filename, SimTaDynGraph& graph)
{
  loadShapefile(filename, graph);
}

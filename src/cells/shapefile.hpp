#ifndef SHAPEFILE_HPP_
#  define SHAPEFILE_HPP_

#  include "graph.hpp"
#  include "boundingbox.hpp"

class ShapefileLoaderException { };
class ShapefileLoaderOpenFailed : public ShapefileLoaderException
{
public:
  int error;

  ShapefileLoaderOpenFailed(int e)
  {
    error = e;
  }
};

class ShapefileLoaderBadId : public ShapefileLoaderException
{
public:
  int bad_id;
  int expected_id;

  ShapefileLoaderBadId(int b, int e)
  {
    bad_id = b;
    expected_id = e;
  }
};

class ShapefileLoaderBadLength : public ShapefileLoaderException
{
public:
  int real_size;
  int expected_size;

  ShapefileLoaderBadLength(int r, int e)
  {
     real_size = r;
     expected_size = e;
  }
};

class ShapefileLoader
{
public:
  ShapefileLoader(const string& filename, SimTaDynGraph& graph);
  ShapefileLoader() { }
  ~ShapefileLoader() { }

  int32_t      readBigEndianInt();
  int32_t      readLittleEndianInt();
  float64_t    readDouble();
  void         goToByte(const uint32_t offset);
  void         skypeNBytes(const uint32_t offset);
  const string shapeTypes(const int id);
  void         checkFileSize();
  void         openShapeFile(const string& filename);
  uint32_t     getShapeVersion();
  uint32_t     getShapeType();
  void         getBoundingBox(Position3D& bbox_min, Position3D& bbox_max);
  void         getBoundingBox(AABB& bbox);
  uint32_t     getRecordAt(SimTaDynGraph& graph, const uint32_t offset);
  void         getAllRecords(SimTaDynGraph& graph);
  void         loadShapefile(const string& filename, SimTaDynGraph& graph);

private:
  ifstream     infile_;
  string       filename_;
  uint32_t     file_length_;
};

#endif /* SHAPEFILE_HPP_ */

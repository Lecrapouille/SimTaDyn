#ifndef SHAPEFILE_HPP_
#  define SHAPEFILE_HPP_

#  include "IMapLoader.hpp"

class ShapefileLoaderException : public SimTaDynMapLoaderException
{
 public:
  uint32_t m_offset;
  std::string m_funcName;

  ShapefileLoaderException(const uint32_t offset, const std::string funcName)
  {
    m_offset = offset;
    m_funcName = funcName;
  }
};

class ShapefileLoaderOpenFailed : public SimTaDynMapLoaderException
{
public:
  int error;

  ShapefileLoaderOpenFailed(int e)
  {
    error = e;
  }
};

class ShapefileLoaderBadId : public SimTaDynMapLoaderException
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

class ShapefileLoaderBadLength : public SimTaDynMapLoaderException
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

class ShapefileLoader : public SimTaDynMapLoader
{
public:
  ShapefileLoader(const string& filename, SimTaDynGraph& graph);
  ShapefileLoader()
  {
  }
  ~ShapefileLoader()
  {
  }
  bool         load(const string& filename, SimTaDynGraph& graph) override;

  // FIXME: protected:

  int32_t      readBigEndianInt();
  int32_t      readLittleEndianInt();
  float64_t    readDouble();
  float32_t    readDoubleCastedFloat();
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

private:
  ifstream     infile_;
  string       filename_;
  uint32_t     file_length_;
};

#endif /* SHAPEFILE_HPP_ */

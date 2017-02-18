#ifndef SHAPEFILE_HPP_
#  define SHAPEFILE_HPP_

#  include "MapLoader.hpp"

class ShapefileLoaderException : public MapLoaderException
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

class ShapefileLoaderOpenFailed : public MapLoaderException
{
public:
  int m_error;

  ShapefileLoaderOpenFailed(int e)
  {
    m_error = e;
  }
};

class ShapefileLoaderBadId : public MapLoaderException
{
public:
  int m_bad_id;
  int m_expected_id;

  ShapefileLoaderBadId(int b, int e)
  {
    m_bad_id = b;
    m_expected_id = e;
  }
};

class ShapefileLoaderBadLength : public MapLoaderException
{
public:
  int m_real_size;
  int m_expected_size;

  ShapefileLoaderBadLength(int r, int e)
  {
     m_real_size = r;
     m_expected_size = e;
  }
};

class ShapefileLoader : public MapLoader
{
public:
  ShapefileLoader()
  {
  }
  ~ShapefileLoader()
  {
  }

  virtual bool load(std::string const& filename, SimTaDynMap* &map) override;

  // FIXME: protected:

  int32_t      readBigEndianInt();
  int32_t      readLittleEndianInt();
  float64_t    readDouble();
  float32_t    readDoubleCastedFloat();
  void         goToByte(const uint32_t offset);
  void         skypeNBytes(const uint32_t offset);
  const std::string shapeTypes(const int id);
  void         checkFileSize();
  void         openShapeFile(const std::string& filename);
  uint32_t     getShapeVersion();
  uint32_t     getShapeType();
  void         getBoundingBox(Position3D& bbox_min, Position3D& bbox_max);
  void         getBoundingBox(AABB& bbox);
  uint32_t     getRecordAt(SimTaDynMap& map, const uint32_t offset);
  void         getAllRecords(SimTaDynMap& map);

private:
  std::ifstream m_infile;
  std::string   m_filename;
  uint32_t      m_file_length;
};

#endif /* SHAPEFILE_HPP_ */

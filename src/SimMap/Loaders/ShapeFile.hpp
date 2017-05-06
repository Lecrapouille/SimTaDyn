#ifndef SHAPEFILE_HPP_
#  define SHAPEFILE_HPP_

#  include "MapLoader.hpp"

// **************************************************************
//
// **************************************************************
class ShapefileLoaderException: public MapLoaderException
{
public:

  uint32_t m_offset;
  std::string m_funcName;

  ShapefileLoaderException(const uint32_t offset, std::string const& funcName)
    : MapLoaderException(41), m_offset(offset), m_funcName(funcName)
  {
    m_msg = "Shapefile Loader Exception at offset " + std::to_string(offset)
      + " in file '" + funcName + "'";
  }
};

class ShapefileLoaderOpenFailed : public MapLoaderException
{
public:

  int m_error;

  ShapefileLoaderOpenFailed(const int error)
    : MapLoaderException(42), m_error(error)
  {
    m_msg = "Shapefile Loader Exception: failed opening with error code " + std::to_string(error) + "'";
  }
};

class ShapefileLoaderBadId : public MapLoaderException
{
public:

  int m_bad_id;
  int m_expected_id;

  ShapefileLoaderBadId(int bad_id, int expected_id)
    : MapLoaderException(43), m_bad_id(bad_id), m_expected_id(expected_id)
  {
    m_msg = "Shapefile Loader Exception: Read bad id " + std::to_string(bad_id)
      + " while expected id " + std::to_string(expected_id);
  }
};

class ShapefileLoaderBadLength : public MapLoaderException
{
public:

  int m_real_size;
  int m_expected_size;

  ShapefileLoaderBadLength(int real_size, int expected_size)
    : MapLoaderException(44), m_real_size(real_size), m_expected_size(expected_size)
  {
     m_msg = "Shapefile Loader Exception: File size is " + std::to_string(real_size)
       + " while indicating " + std::to_string(expected_size);
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

protected:

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

  std::ifstream m_infile;
  std::string   m_filename;
  uint32_t      m_file_length;

private:

  Vector3D      m_point;
};

#endif /* SHAPEFILE_HPP_ */

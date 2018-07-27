#ifndef SHAPEFILE_HPP_
#  define SHAPEFILE_HPP_

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

#endif /* SHAPEFILE_HPP_ */

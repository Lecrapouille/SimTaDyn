//#ifndef SHAPEFILE_HPP_
//#  define SHAPEFILE_HPP_

#  include <iostream>
#  include <fstream>
#  include <cerrno>
#  include <cstring>
#  include <cstdint>

using namespace std;

typedef double float64_t;

static int32_t readBigEndianInt(ifstream& infile)
{
  char buf[4];

  infile.read(buf, 4);
  return (((buf[0] & 0xFF) << 24) | ((buf[1] & 0xFF) << 16) | ((buf[2] & 0xFF) << 8) | (buf[3] & 0xFF));
}

static int32_t readLittleEndianInt(ifstream& infile)
{
  char buf[4];

  infile.read(buf, 4);
  return (((buf[3] & 0xFF) << 24) | ((buf[2] & 0xFF) << 16) | ((buf[1] & 0xFF) << 8) | (buf[0] & 0xFF));
}

static float64_t readDouble(ifstream& infile)
{
  char buf[8];
  float64_t convert;

  infile.read(buf, 8);
  convert = *reinterpret_cast<double * const>(buf);
  return convert;
}

static inline void goToByte(ifstream& infile, const uint32_t offset)
{
  infile.seekg(offset, ios::beg);
}

static inline void skypeNBytes(ifstream& infile, const uint32_t offset)
{
  infile.seekg(offset, ios::cur);
}

const string shapeTypes(const int id)
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

class PointZ
{
public:
  float64_t x, y, z, m;
};

int main(void)
{
  ifstream infile;
  string filename = "3dpoints.shp";
  uint32_t value32b, file_length;
  float64_t bounding_box[4];

  infile.open(filename, ios::binary | ios::in);
  if (!infile)
    {
      cerr << "Cannot open the file " << filename << ". Reason is '" << strerror(errno) << "'" << endl;
      return EXIT_FAILURE;
    }

  value32b = readBigEndianInt(infile);
  cout << "File Code: " << value32b << endl;
  if (9994 != value32b)
    {
      cerr << "Error. Expected file code 9994 not found. The file '" << filename << "' seems not to be a shp file" << endl;
      return EXIT_FAILURE;
    }

  goToByte(infile, 24);
  file_length = readBigEndianInt(infile) * sizeof (uint16_t);
  cout << "File Lenght: " << file_length << endl;

  // goToByte(infile, 28);
  value32b = readLittleEndianInt(infile);
  cout << "Shapefile Version: " << value32b << endl;
  if (1000 != value32b)
    {
      cerr << "Warning. Expected shapefile version 1000 not found. The file '" << filename << "' may be not fully interpreted" << endl;
      return EXIT_FAILURE;
    }

  // goToByte(infile, 32);
  value32b = readLittleEndianInt(infile);
  cout << "Shape Type: " << value32b << ": " << shapeTypes(value32b) << endl;

  // goToByte(infile, 36);
  bounding_box[0] = readDouble(infile);
  bounding_box[1] = readDouble(infile);
  bounding_box[2] = readDouble(infile);
  bounding_box[3] = readDouble(infile);
  cout << "Bounding Box: Xmin: " << bounding_box[0] << endl;
  cout << "Bounding Box: Ymin: " << bounding_box[1] << endl;
  cout << "Bounding Box: Ymax: " << bounding_box[2] << endl;
  cout << "Bounding Box: Ymax: " << bounding_box[3] << endl;

  goToByte(infile, 100);
  uint32_t total_bytes_read = 100U;
  while (total_bytes_read < file_length)
    {
      uint32_t record_number, content_length;

      if (infile.eof())
        break;

      record_number = readBigEndianInt(infile);
      content_length = readBigEndianInt(infile) * sizeof (uint16_t);
      total_bytes_read += 8U;
      value32b = readLittleEndianInt(infile);

      cout << "Record Number: " << record_number << ", Content Length: " << content_length << ":" << endl;
      cout << "  Shape " << record_number - 1U << " (" << shapeTypes(value32b) << "): ";
      switch (value32b)
        {
        case 11: // PointZ
          PointZ p;
          p.x = readDouble(infile);
          p.y = readDouble(infile);
          p.z = readDouble(infile);
          //p.m = readDouble(infile);
          //cout << "(PointZ) ";
          cout << p.x << " " << p.y << " " << p.z << " " << endl;
          break;
        default:
          cout << "Shape Type: " << value32b << ": " << shapeTypes(value32b) << ": Content Length: " << content_length << endl;
          skypeNBytes(infile, content_length);
          break;
        }
      total_bytes_read += content_length;
      cout << "Total bytes read: " << total_bytes_read << endl;
    }

  infile.close();
  return EXIT_SUCCESS;
}

//#endif /* SHAPEFILE_HPP_ */

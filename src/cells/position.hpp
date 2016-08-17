#ifndef POSITION_HPP_
#  define POSITION_HPP_

#  include <simtypes.hpp>
#  include <ostream>

// *************************************************************************************************
// Position in space (needed by nodes for example)
// herits from Event class for handler management
// *************************************************************************************************
class Position3D
{
public:
  float32_t x, y, z;

public:
  inline Position3D(float32_t x_, float32_t y_, float32_t z_ = 0.0f) { x = x_; y = y_; z = z_; }
  inline Position3D(const Position3D &p) { x = p.x; y = p.y; z = p.z; }
  inline Position3D() { x = 0.0f; y = 0.0f; z = 0.0f; }

  inline Position3D& operator=(const Position3D& p)
  {
    if (this != &p)
      {
        x = p.x;
        y = p.y;
        z = p.z;
      }
    return *this;
  }

  inline Position3D operator+(const Position3D& p)
  {
    Position3D res;

    res.x = p.x + x;
    res.y = p.y + y;
    res.z = p.z + z;
    return res;
  }

  inline bool operator==(const Position3D& p)
  {
    return (p.x == x) && (p.y == y) && (p.z == z);
  }

  inline bool operator!=(const Position3D& p)
  {
    return !((p.x == x) && (p.y == y) && (p.z == z));
  }

  inline float32_t distance(const Position3D &p)
  {
    return sqrt(p.x * x + p.y * y + p.z * z);
  }

  inline float32_t distance2(const Position3D &p)
  {
    return p.x * x + p.y * y + p.z * z;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Position3D& p)
{
  os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
  return os;
}

#endif /* POSITION_HPP_ */

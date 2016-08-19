#ifndef VECTOR3D_HPP_
#  define VECTOR3D_HPP_

#  include "simtypes.hpp"
#  include <ostream>

// *************************************************************************************************
// Position in space (needed by nodes for example)
// herits from Event class for handler management
// *************************************************************************************************
class Vector3D
{
public:
  float32_t x, y, z;

  inline Vector3D()
  {
  }

  inline Vector3D(const float32_t fx, const float32_t fy, const float32_t fz = 0.0f)
    : x(fx), y(fy), z(fz)
  {
  }

  inline Vector3D(const float32_t coordinate[3])
    : x(coordinate[0]), y(coordinate[1]), z(coordinate[2])
  {
  }

  inline Vector3D(float32_t* const coordinate)
    : x(coordinate[0]), y(coordinate[1]), z(coordinate[2])
  {
  }

  inline Vector3D(const int32_t coordinate[3])
  {
    x = (float32_t) coordinate[0];
    y = (float32_t) coordinate[1];
    z = (float32_t) coordinate[2];
  }

  inline Vector3D(int32_t* const coordinate)
  {
    x = (float32_t) coordinate[0];
    y = (float32_t) coordinate[1];
    z = (float32_t) coordinate[2];
  }

  inline Vector3D(const int32_t ix, const int32_t iy, const int32_t iz = 0)
  {
    x = (float32_t) ix;
    y = (float32_t) iy;
    z = (float32_t) iz;
  }

  inline void swap(Vector3D& other)
  {
    std::swap(x, other.x);
    std::swap(y, other.y);
    std::swap(z, other.z);
  }

  inline bool operator==(const Vector3D& p) const
  {
    return (p.x == x) && (p.y == y) && (p.z == z);
  }

  inline bool operator!=(const Vector3D& p) const
  {
    return (p.x != x) || (p.y != y) || (p.z != z);
  }

  inline Vector3D operator+(const Vector3D& p) const
  {
    return Vector3D(x + p.x, y + p.y, z + p.z);
  }

  inline const Vector3D& operator+() const
  {
    return *this;
  }

  inline const Vector3D operator+(const float32_t scalar) const
  {
    return Vector3D(x + scalar, y + scalar, z + scalar);
  }

  inline const Vector3D operator*(const float32_t scalar) const
  {
    return Vector3D(x * scalar, y * scalar, z * scalar);
  }

  inline const Vector3D operator/(const float32_t scalar) const
  {
    if (0.0f == scalar) std::overflow_error("Divide by zero exception in " + std::string(__PRETTY_FUNCTION__));
    return Vector3D(x / scalar, y / scalar, z / scalar);
  }

  inline Vector3D operator-(const Vector3D& p) const
  {
    return Vector3D(x - p.x, y - p.y, z - p.z);
  }

  inline const Vector3D operator-() const
  {
    return Vector3D(-x, -y, -z);
  }

  inline const Vector3D operator-(const float32_t scalar) const
  {
    return Vector3D(x - scalar, y - scalar, z - scalar);
  }

  inline Vector3D& operator=(const Vector3D& p)
  {
    if (this != &p)
      {
        x = p.x;
        y = p.y;
        z = p.z;
      }
    return *this;
  }

  inline Vector3D& operator=(const float32_t scalar)
  {
    x = scalar;
    y = scalar;
    z = scalar;
    return *this;
  }

  inline Vector3D min(const Vector3D& p) const
  {
    return Vector3D(std::min(x, p.x), std::min(y, p.y), std::min(z, p.z));
  }

  inline Vector3D max(const Vector3D& p) const
  {
    return Vector3D(std::max(x, p.x),std:: max(y, p.y), std::max(z, p.z));
  }

  inline Vector3D midPoint(const Vector3D& p) const
  {
    return Vector3D((x + p.x) * 0.5f, (y + p.y) * 0.5f, (z + p.z) * 0.5f);
  }

  inline float32_t length() const
  {
    return sqrt(x * x + y * y + z * z);
  }

  inline float32_t squaredLength() const
  {
    return x * x + y * y + z * z;
  }

  inline float32_t distance(const Vector3D &p) const
  {
    return (*this - p).length();
  }

  inline float32_t squaredDistance(const Vector3D &p) const
  {
    return (*this - p).squaredLength();
  }

  inline float32_t dotProduct(const Vector3D &p) const
  {
    return p.x * x + p.y * y + p.z * z;
  }

  inline Vector3D crossProduct(const Vector3D& p) const
  {
    return Vector3D(y * p.z - z * p.y,
                    z * p.x - x * p.z,
                    x * p.y - y * p.x);
  }

  inline bool normalise()
  {
    float32_t length = sqrt(x * x + y * y + z * z);
    if (length > 0.0f)
      {
        float32_t invlength = 1.0f / length;
        x *= invlength;
        y *= invlength;
        z *= invlength;
        return true;
      }
    return false;
  }

  static const Vector3D ZERO;
  static const Vector3D UNIT_X;
  static const Vector3D UNIT_Y;
  static const Vector3D UNIT_Z;
  static const Vector3D NEGATIVE_UNIT_X;
  static const Vector3D NEGATIVE_UNIT_Y;
  static const Vector3D NEGATIVE_UNIT_Z;
  static const Vector3D UNIT_SCALE;
};

inline std::ostream& operator<<(std::ostream& os, const Vector3D& p)
{
  os << "Vector3D(" << p.x << ", " << p.y << ", " << p.z << ")";
  return os;
}

const Vector3D Vector3D::ZERO(0, 0, 0);
const Vector3D Vector3D::UNIT_X(1, 0, 0);
const Vector3D Vector3D::UNIT_Y(0, 1, 0);
const Vector3D Vector3D::UNIT_Z(0, 0, 1);
const Vector3D Vector3D::NEGATIVE_UNIT_X(-1, 0, 0);
const Vector3D Vector3D::NEGATIVE_UNIT_Y(0, -1, 0);
const Vector3D Vector3D::NEGATIVE_UNIT_Z(0, 0, -1);
const Vector3D Vector3D::UNIT_SCALE(1, 1, 1);

#define Position3D Vector3D

#endif /* POSITION_HPP_ */

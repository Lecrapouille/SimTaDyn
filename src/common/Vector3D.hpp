#ifndef VECTOR3D_HPP_
#  define VECTOR3D_HPP_

#  include "simtypes.hpp"
#  include <ostream>
#  include <math.h>

// *************************************************************************************************
// Position in space (needed by nodes for example)
// herits from Event class for handler management
// *************************************************************************************************
class Vector3D
{
public:
  float32_t x, y, z;

  inline Vector3D()
    : x(NAN), y(NAN), z(NAN)
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
    return Vector3D(min_(x, p.x), min_(y, p.y), min_(z, p.z));
  }

  inline Vector3D max(const Vector3D& p) const
  {
    return Vector3D(max_(x, p.x), max_(y, p.y), max_(z, p.z));
  }

  inline Vector3D midPoint(const Vector3D& p) const
  {
    return Vector3D((x + p.x) * 0.5f, (y + p.y) * 0.5f, (z + p.z) * 0.5f);
  }

  inline float32_t length() const
  {
    return std::sqrt(x * x + y * y + z * z);
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
    float32_t length = std::sqrt(x * x + y * y + z * z);
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

  static const Vector3D DUMMY;
  static const Vector3D ZERO;
  static const Vector3D UNIT_X;
  static const Vector3D UNIT_Y;
  static const Vector3D UNIT_Z;
  static const Vector3D NEGATIVE_UNIT_X;
  static const Vector3D NEGATIVE_UNIT_Y;
  static const Vector3D NEGATIVE_UNIT_Z;
  static const Vector3D UNIT_SCALE;
  static const Vector3D NEGATIVE_UNIT_SCALE;

protected:
  float32_t min_(const float32_t a, const float32_t b) const
  {
    if (std::isnan(a))
      return b;
    if (std::isnan(b))
      return a;
    else
      return std::min(a, b);
  }
  float32_t max_(const float32_t a, const float32_t b) const
  {
    if (std::isnan(a))
      return b;
    if (std::isnan(b))
      return a;
    else
      return std::max(a, b);
  }
};

inline std::ostream& operator<<(std::ostream& os, const Vector3D& p)
{
  os << "Vector3D(" << p.x << ", " << p.y << ", " << p.z << ")";
  return os;
}

#define Position3D Vector3D

#endif /* VECTOR3D_HPP_ */

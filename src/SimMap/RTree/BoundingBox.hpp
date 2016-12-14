#ifndef BOUNDINGBOX_HPP_
#  define BOUNDINGBOX_HPP_

#  include "Vector3D.hpp"

// Axis Aligned Bounding Box
class AABB
{
public:
  inline AABB()
    : bbmin(Vector3D::DUMMY), bbmax(Vector3D::DUMMY)
  {
  }

  inline AABB(const Position3D& pmin, const Position3D& pmax)
    : bbmin(pmin), bbmax(pmax)
  {
    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  inline AABB(const float32_t xmin, const float32_t ymin, const float32_t zmin,
              const float32_t xmax, const float32_t ymax, const float32_t zmax)
    : bbmin(xmin, ymin, zmin), bbmax(xmax, ymax, zmax)
  {
    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  inline void setBox(const Position3D& pmin, const Position3D& pmax)
  {
    bbmin = pmin;
    bbmax = pmax;

    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  inline void setBox(const float32_t xmin, const float32_t ymin, const float32_t zmin,
                     const float32_t xmax, const float32_t ymax, const float32_t zmax)
  {
    bbmin = Vector3D(xmin, ymin, zmin);
    bbmax = Vector3D(xmax, ymax, zmax);

    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  inline bool isWellFormed(std::string& msg)
  {
    bool res = true;

    if (bbmin.x > bbmax.x)
      {
        msg = msg + "The minimum X-axis corner of the box must be less than or equal to maximum corner\n";
        res = false;
      }
    if (bbmin.y > bbmax.y)
      {
        msg = msg + "The minimum Y-axis corner of the box must be less than or equal to maximum corner\n";
        res = false;
      }
    if (bbmin.z > bbmax.z)
      {
        msg = msg + "The minimum Y-axis corner of the box must be less than or equal to maximum corner\n";
        res = false;
      }
    return res;
  }

  inline void swap(AABB& other)
  {
    bbmin.swap(other.bbmin);
    bbmax.swap(other.bbmax);
  }

  inline bool operator==(const AABB& other) const
  {
    return (bbmin == other.bbmin) && (bbmax == other.bbmax);
  }

  inline bool operator!=(const AABB& other) const
  {
    return (bbmin != other.bbmin) || (bbmax != other.bbmax);
  }

  inline AABB operator+(const AABB& other) const
  {
    return merge(other);
  }

  inline AABB operator+(const float32_t scalar) const
  {
    return AABB(bbmin + scalar, bbmax + scalar);
  }

  inline AABB operator+(const Vector3D vector) const
  {
    return AABB(bbmin + vector, bbmax + vector);
  }

  inline const AABB operator*(const float32_t scalar) const
  {
    return AABB(bbmin * scalar, bbmax * scalar);
  }

  inline const AABB& operator+() const
  {
    return *this;
  }

  inline AABB operator-(const AABB& other) const
  {
    return intersection(other);
  }

  inline AABB operator-(const float32_t scalar) const
  {
    return AABB(bbmin - scalar, bbmax - scalar);
  }

  inline AABB operator-(const Vector3D vector) const
  {
    return AABB(bbmin - vector, bbmax - vector);
  }

  inline const AABB operator/(const float32_t scalar) const
  {
    return AABB(bbmin / scalar, bbmax / scalar);
  }

  inline const AABB operator-() const
  {
    return AABB(-bbmin, -bbmax);
  }

  inline AABB& operator=(const AABB& other)
  {
    if (this != &other)
      {
        bbmin = other.bbmin;
        bbmax = other.bbmax;
      }
    return *this;
  }

  inline Vector3D centerPoint() const
  {
    return Vector3D((bbmax + bbmin) * 0.5f);
  }

  /*inline AABB scaleFromCenter(const float32_t scalar) const
  {
    const Vector3D c = centerPoint();
    return AABB((bbmin - c) * scalar, (bbmax - c) * scalar) + c;
    }*/
  inline void scaleFromCenter(const float32_t scalar)
  {
    if (bbmin != bbmax)
      {
        const Vector3D c = centerPoint();
        bbmin = ((bbmin - c) * scalar) + c;
        bbmax = ((bbmax - c) * scalar) + c;
      }
    else
      {
        bbmin = bbmin * scalar;
        bbmax = bbmax * scalar;
      }
  }

  inline Vector3D dimension() const
  {
    return bbmax - bbmin;
  }

  inline float32_t volume() const
  {
    Vector3D dim = dimension();
    return dim.x * dim.y * dim.z;
  }

  inline bool operator<=(const AABB& other) const
  {
    return volume() <= other.volume();
  }

  inline bool operator<(const AABB& other) const
  {
    return volume() < other.volume();
  }

  inline bool operator>=(const AABB& other) const
  {
    return volume() >= other.volume();
  }

  inline bool operator>(const AABB& other) const
  {
    return volume() > other.volume();
  }

  inline bool contains(const Vector3D& other) const
  {
    return
      (bbmin.x <= other.x) &&
      (other.x <= bbmax.x) &&
      (bbmin.y <= other.y) &&
      (other.y <= bbmax.y) &&
      (bbmin.z <= other.z) &&
      (other.z <= bbmax.z);
  }

  inline bool contains(const AABB& other) const
  {
    return (bbmin.x <= other.bbmin.x) &&
      (bbmin.y <= other.bbmin.y) &&
      (bbmin.z <= other.bbmin.z) &&
      (other.bbmax.x <= bbmax.x) &&
      (other.bbmax.y <= bbmax.y) &&
      (other.bbmax.z <= bbmax.z);
  }

  inline bool collides(const Vector3D& other) const
  {
    return
      (other.x >= bbmin.x) &&
      (other.x <= bbmax.x) &&
      (other.y >= bbmin.y) &&
      (other.y <= bbmax.y) &&
      (other.z >= bbmin.z) &&
      (other.z <= bbmax.z);
  }

  // Alias for overlapes
  inline bool collides(const AABB& other) const
  {
    return
      ((bbmax.x >= other.bbmin.x) &&
       (bbmax.y >= other.bbmin.y) &&
       (bbmax.z >= other.bbmin.z) &&
       (bbmin.x <= other.bbmax.x) &&
       (bbmin.y <= other.bbmax.y) &&
       (bbmin.z <= other.bbmax.z));
  }

  // Alias for combineRect
  // Alias for union but union is a C/C++ keyword so cannot be used
  inline AABB merge(const AABB& other) const
  {
    return AABB(bbmin.min(other.bbmin), bbmax.max(other.bbmax));
  }
  inline AABB intersection(const AABB& other) const
  {
    if (collides(other))
      return AABB(bbmin.max(other.bbmin), bbmax.min(other.bbmax));
    return AABB::ZERO;
  }

  inline friend std::ostream& operator<<(std::ostream& os, const AABB& other)
  {
    return os << "AABB(min: " << other.bbmin << ", max: " << other.bbmax << ", center: " << other.centerPoint() << ") ";
  }

  // special values
  static const AABB DUMMY;
  static const AABB ZERO;
  static const AABB UNIT_SCALE;
  static const AABB INFINITE;

  //protected:
  Vector3D bbmin;
  Vector3D bbmax;
};

#endif /* BOUNDINGBOX_HPP_ */
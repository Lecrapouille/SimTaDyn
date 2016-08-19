#ifndef BOUNDINGBOX_HPP_
#  define BOUNDINGBOX_HPP_

#  include "vector3D.hpp"

// Axis Aligned Bounding Box
class AABB
{
public:
  inline AABB()
    : bbmin(Vector3D::ZERO), bbmax(Vector3D::UNIT_SCALE)
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

  // alias for union but union is a C/C++ keyword so cannot be used
  inline AABB merge(const AABB& other) const
  {
    return AABB(bbmin.min(other.bbmin), bbmax.max(other.bbmax));
  }
  inline AABB intersection(const AABB& other) const
  {
    return AABB(bbmin.max(other.bbmin), bbmax.min(other.bbmax));
  }

  inline friend std::ostream& operator<<(std::ostream& os, const AABB& other)
  {
    os << "AABB(" << other.bbmin << ", " << other.bbmax << ") ";
    return os;
  }

  // special values
  static const AABB AABB_ZERO;
  static const AABB AABB_UNIT_SCALE;

protected:
  Position3D bbmin;
  Position3D bbmax;
};

const AABB AABB::AABB_ZERO(Vector3D::ZERO, Vector3D::ZERO);
const AABB AABB::AABB_UNIT_SCALE(Vector3D::ZERO, Vector3D::UNIT_SCALE);


#endif /* BOUNDINGBOX_HPP_ */

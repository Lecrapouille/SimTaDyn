#ifndef BOUNDINGBOX_TPP_
#  define BOUNDINGBOX_TPP_

#  include "Vector3D.tpp"
#  include <limits>

// *************************************************************************************************
//! \brief Axis Aligned Bounding Box
// *************************************************************************************************
template <typename T, uint32_t n>
class AABB
{
public:
  inline AABB()
    : bbmin(NAN), bbmax(NAN)
  {
  }

  inline AABB(const Vector<T, n>& pmin, const Vector<T, n>& pmax)
    : bbmin(pmin), bbmax(pmax)
  {
    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  inline AABB(std::initializer_list<T> initMinList, std::initializer_list<T> initMaxList)
    : bbmin(initMinList), bbmax(initMaxList)
  {
    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  inline void setBox(const Vector<T, n>& pmin, const Vector<T, n>& pmax)
  {
    bbmin = pmin;
    bbmax = pmax;

    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
  }

  /*inline void setBox(const T xmin, const T ymin, const T zmin,
                     const T xmax, const T ymax, const T zmax)
  {
    bbmin = Vector<T, n>(xmin, ymin, zmin);
    bbmax = Vector<T, n>(xmax, ymax, zmax);

    std::string msg = "";
    if (false == isWellFormed(msg))
      {
        throw std::out_of_range(std::string(__PRETTY_FUNCTION__) + ": " + msg);
      }
      }*/

  inline bool isWellFormed(std::string& msg)
  {
    char c = 'X';
    const uint32_t d = bbmin.dimension();

    for (uint32_t i = 0U; i < d; ++i)
      {
        if (bbmin[i] > bbmax[i])
          {
            ++c;
            msg = msg + "The minimum " + c + "-axis corner of the box must be less than or equal to maximum corner";
            return false;
          }
      }
    return true;
  }

  inline void swap(AABB& other)
  {
    swap(bbmin, other.bbmin);
    swap(bbmax, other.bbmax);
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

  inline AABB operator+(const T scalar) const
  {
    return AABB(bbmin + scalar, bbmax + scalar);
  }

  inline AABB operator+(const Vector<T, n> vector) const
  {
    return AABB(bbmin + vector, bbmax + vector);
  }

  inline const AABB operator*(const T scalar) const
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

  inline AABB operator-(const T scalar) const
  {
    return AABB(bbmin - scalar, bbmax - scalar);
  }

  inline AABB operator-(const Vector<T, n> vector) const
  {
    return AABB(bbmin - vector, bbmax - vector);
  }

  inline const AABB operator/(const T scalar) const
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

  inline Vector<T, n> centerPoint() const
  {
    return Vector<T, n>((bbmax + bbmin) * 0.5f);
  }

  /*inline AABB scaleFromCenter(const T scalar) const
  {
    const Vector<T, n> c = centerPoint();
    return AABB((bbmin - c) * scalar, (bbmax - c) * scalar) + c;
    }*/
  inline void scaleFromCenter(const T scalar)
  {
    if (bbmin != bbmax)
      {
        const Vector<T, n> c = centerPoint();
        bbmin = ((bbmin - c) * scalar) + c;
        bbmax = ((bbmax - c) * scalar) + c;
      }
    else
      {
        bbmin = bbmin * scalar;
        bbmax = bbmax * scalar;
      }
  }

  inline Vector<T, n> dimension() const
  {
    return bbmax - bbmin;
  }

  inline T volume() const
  {
    Vector<T, n> dim = dimension();
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

  inline bool contains(const Vector<T, n>& other) const
  {
    uint32_t i = n;
    while (i--)
      {
        if (!((bbmin[i] <= other[i]) && (other[i] <= bbmax[i])))
          return false;
      }
    return true;
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

  inline bool collides(const Vector<T, n>& other) const
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
  static const AABB<T, n> DUMMY;
  static const AABB<T, n> ZERO;
  static const AABB<T, n> UNIT_SCALE;
  static const AABB<T, n> INFINITE;

  //protected:
  Vector<T, n> bbmin;
  Vector<T, n> bbmax;
};

const AABB<T, n> AABB<T, n>::DUMMY(Vector<T, n>(NAN), Vector<T, n>(NAN));
const AABB<T, n> AABB<T, n>::ZERO(Vector<T, n>(0), Vector<T, n>(0));
const AABB<T, n> AABB<T, n>::UNIT_SCALE(Vector<T, n>(-0.5f), Vector3<T, n>(0.5f));
const AABB<T, n> AABB<T, n>::INFINITE(Vector<T, n>(std::numeric_limits<T>::lowest()),
                                      Vector<T, n>(std::numeric_limits<T>::max()));

#endif /* BOUNDINGBOX_TPP_ */

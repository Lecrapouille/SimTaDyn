#ifndef TYPES_HPP_
#  define TYPES_HPP_

#  include "handler.hpp"

using namespace std;

// *************************************************************************************************
// Redifine types
// *************************************************************************************************


class   SimTaDynCell;
class   SimTaDynNode;

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
};

inline ostream& operator<<(ostream& os, const Position3D& p)
{
  os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
  return os;
}

// *************************************************************************************************
// Inspired from: http://www.drdobbs.com/cpp/counting-objects-in-c/
//
// This class allows to count the number of created instances. It's safer to use this class than
// using directly a memeber 'static size_t count;' when a failure occured in creator member and also
// easier for heritage.
// *************************************************************************************************
template<typename T> class ClassCounter
{
public:
  ClassCounter() { ++count_; }
  ClassCounter(const ClassCounter&) { ++count_; }
  ~ClassCounter() { --count_; }

  static size_t howMany() { return count_; }
private:
  static size_t count_;
};

template<typename T> size_t ClassCounter<T>::count_ = 0;

#endif /* TYPES_HPP_ */

// -*- c++ -*- Coloration Syntaxique pour Emacs
//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef BOUNDINGBOX_TPP_
#  define BOUNDINGBOX_TPP_

#  include "Vector.tpp"
#  include <limits>

namespace aabb
{

  //! \brief Enum for getting the box corner.
  enum Corner { LowerCorner, UpperCorner };

}

#  define DEFINE_INPLACE_OPERATORS(op)   \
  AABB<T, n> &operator op(AABB<T, n> const &a)  \
  {                                      \
    m_bbmin op a.m_bbmin;                \
    m_bbmax op a.m_bbmax;                \
    return *this;                        \
  }                                      \
  AABB<T, n> &operator op(T const a)     \
  {                                      \
    m_bbmin op a;                        \
    m_bbmax op a;                        \
    return *this;                        \
  }

#  define DEFINE_BINARY_OPERATORS(op)                                   \
  inline AABB<T, n> operator op (T const &scalar) const                 \
  {                                                                     \
    return AABB<T, n>(m_bbmin op scalar, m_bbmax op scalar);            \
  }

// *************************************************************************************************
//! \brief Axis Aligned Bounding Box (AABB for short). An AABB is either a rectangle (in 2D world)
//! or a cube (in a 3D word) which an entire object fits in. AABBs are permanently aligned to the
//! world axes. It's also possible to set an axis to infinite.
//!
//! AABB are used for classifying spatial objects in the aim to coarse collision detection or object
//! picking).
// *************************************************************************************************
template <typename T, uint32_t n>
class AABB
{
public:

  //! \brief Empty constructor. Positions are not init.
  AABB()
    : m_bbmin(), m_bbmax()
  {
  }

  //! \brief Constructor. Init all values to the same given value.
  explicit AABB(T const a)
    : m_bbmin(a), m_bbmax(a)
  {
  }

  //! \brief Constructor. Init all values to the same given value.
  explicit AABB(T const a, T const b)
    : m_bbmin(a), m_bbmax(b)
  {
  }

  //! \brief Constructor by copy. The dimension of vectors can be not
  //! to be the same size: it's ok just the good part will be taken
  //! into account.
  template <typename U, uint32_t m>
  explicit AABB(AABB<U, m> const& a)
    : m_bbmin(a.m_bbmin), m_bbmax(a.m_bbmax)
  {
  }

  //! \brief Constructor. Init bounding box dimensions through two
  //! vectors: min postion and max position.
  //! \param bbmin: vector for the lower corner for the box.
  //! \param bbmax: vector for the upper corner for the box.
  //! \throw std::out_of_range if bbmin > bbmax.
  AABB(const Vector<T, n>& bbmin, const Vector<T, n>& bbmax)
    : m_bbmin(bbmin), m_bbmax(bbmax)
  {
    checkWellFormed();
  }

  //! \brief Constructor. Init bounding box dimensions through two
  //! initializer list: min postion and max position.
  //! \param bbmin: vector for the lower corner for the box.
  //! \param bbmax: vector for the upper corner for the box.
  //! \throw std::out_of_range if bbmin > bbmax.
  AABB(std::initializer_list<T> bbmin, std::initializer_list<T> bbmax)
    : m_bbmin(bbmin), m_bbmax(bbmax)
  {
    checkWellFormed();
  }

  //! \brief Change the bounding box dimensions through two vectors:
  //! min postion and max position.
  //! \param bbmin: vector for the lower corner for the box.
  //! \param bbmax: vector for the upper corner for the box.
  //! \throw std::out_of_range if bbmin > bbmax.
  inline void set(const Vector<T, n>& bbmin, const Vector<T, n>& bbmax)
  {
    m_bbmin = bbmin;
    m_bbmax = bbmax;
    checkWellFormed();
  }

  //! \brief Accessor. Return the selected box corner in read only
  //! mode.
  const Vector<T, n> &get(aabb::Corner const i) const
  {
    return bboxes[i];
  }

  //! \brief Return the dimension of the box (2D, 3D ...).
  inline uint32_t dim() const
  {
    return n;
  }

  DEFINE_INPLACE_OPERATORS(+=)
  DEFINE_INPLACE_OPERATORS(-=)
  DEFINE_INPLACE_OPERATORS(*=)
  DEFINE_INPLACE_OPERATORS(/=)
  //! \brief Move the box of the given scalar.
  DEFINE_BINARY_OPERATORS(+)
  //! \brief Move the box of the given scalar.
  DEFINE_BINARY_OPERATORS(-)
  //! \brief Scale the box of the given scalar. It's a different
  //! behavior that the scale().
  DEFINE_BINARY_OPERATORS(*)
  //! \brief Scale the box of the given scalar. It's a different
  //! behavior that the scale().
  DEFINE_BINARY_OPERATORS(/)

  //! \brief Sign operator.
  inline AABB<T, n> const & operator+() const
  {
    return *this;
  }

  //! \brief Change the sign of the box dimension.
  inline const AABB<T, n> operator-() const
  {
    return AABB(-m_bbmax, -m_bbmin);
  }

  //! \brief Replace dimensions of the bounding box by the given
  //! bounding box.
  inline AABB<T, n>& operator=(AABB<T, n> const &other)
  {
    if (this != &other)
      {
        m_bbmin = other.m_bbmin;
        m_bbmax = other.m_bbmax;
      }
    return *this;
  }

  //! \brief Return the center point of the box.
  inline Vector<T, n> center() const
  {
    return Vector<T, n>((m_bbmax + m_bbmin) * 0.5f);
  }

  //! \brief Scale the box from its center to the factor given the
  //! scalar value. Note: It's a different behavior that the
  //! operator*().
  inline void scale(const T scalar)
  {
    if (m_bbmin != m_bbmax)
      {
        const Vector<T, n> c = center();
        m_bbmin = ((m_bbmin - c) * scalar) + c;
        m_bbmax = ((m_bbmax - c) * scalar) + c;
      }
    else
      {
        m_bbmin = m_bbmin * scalar;
        m_bbmax = m_bbmax * scalar;
      }
  }

  //! \brief Return the size of the box.
  inline Vector<T, n> size() const
  {
    return m_bbmax - m_bbmin;
  }

  //! \brief Return the volume of the box (if in >= 3D) or the volume
  //! (for 2D box).
  inline T volume() const
  {
    T result = T(1);
    Vector<T, n> dim = size();
    uint32_t i = n;

    while (i--)
      result *= dim[i];

    return result;
  }

  //! \brief Compare volume of the box. Be careful it does not compare
  //! corner positions.
  inline bool operator==(AABB<T, n> const &other) const
  {
    return volume() == other.volume();
  }

  //! \brief Compare volume of the box. Be careful it does not compare
  //! corner positions.
  inline bool operator!=(AABB<T, n> const &other) const
  {
    return volume() != other.volume();
  }

  //! \brief Compare volume of the box. Be careful it does not compare
  //! corner positions.
  inline bool operator<=(AABB<T, n> const &other) const
  {
    return volume() <= other.volume();
  }

  //! \brief Compare volume of the box. Be careful it does not compare
  //! corner positions.
  inline bool operator<(AABB<T, n> const &other) const
  {
    return volume() < other.volume();
  }

  //! \brief Compare volume of the box. Be careful it does not compare
  //! corner positions.
  inline bool operator>=(AABB<T, n> const &other) const
  {
    return volume() >= other.volume();
  }

  //! \brief Compare volume of the box. Be careful it does not compare
  //! corner positions.
  inline bool operator>(AABB<T, n> const &other) const
  {
    return volume() > other.volume();
  }

  //! \brief Clamp the box dimension to the limit given by the AABB.
  inline void constrain(Vector<T, n> const& vmin, Vector<T, n> const& vmax)
  {
    uint32_t i = n;

    while (i--)
      {
        m_bbmin[i] = maths::clamp(m_bbmin[i], vmin[i], vmax[i]);
        m_bbmax[i] = maths::clamp(m_bbmax[i], vmin[i], vmax[i]);
      }
  }

  //! \brief Check if the box corners contain the other box corners.
  inline bool contains(const Vector<T, n>& point) const
  {
    uint32_t i = n;
    while (i--)
      {
        if (!((m_bbmin[i] <= point[i]) && (point[i] <= m_bbmax[i])))
          return false;
      }
    return true;
  }

  //! \brief Check if the box corners contain the other box corners.
  inline bool contains(AABB<T, n> const &other) const
  {
    uint32_t i = n;
    while (i--)
      {
        if (!((m_bbmin[i] <= other.m_bbmin[i]) && (other.m_bbmax[i] <= m_bbmax[i])))
          return false;
      }
    return true;
  }

  //! \brief Check if the AABB overlapes another AABB (collision test).
  inline bool collides(AABB<T, n> const &other) const
  {
    uint32_t i = n;
    while (i--)
      {
        if (!((m_bbmax[i] >= other.m_bbmin[i]) && (m_bbmin[i] <= other.m_bbmax[i])))
          return false;
      }
    return true;
  }

  /*template <typename T, int n>
  void boxProjectOnAxis(box<T, n> a, vector<T, n> b, T & outMin, T & outMax)
  {
    T dotCenter = dot((a.mins + a.maxs) / T(2), b);
    T dotDiagonal = dot(a.maxs - a.mins, abs(b));
    outMin = dotCenter - dotDiagonal;
    outMax = dotCenter + dotDiagonal;
    }*/

  //! \brief
  inline friend std::ostream& operator<<(std::ostream &os, AABB<T, n> const &aabb)
  {
    return os << "AABB((min: " << aabb.m_bbmin << "), (max: " << aabb.m_bbmax << "), (center: " << aabb.center() << ")) ";
  }

  //! \brief Define an AABB with Not-A-Number values (T needs to be of type double or float).
  static const AABB<T, n> DUMMY;
  //! \brief Define an infinite AABB
  static const AABB<T, n> INFINITE;
  //! \brief Define a zero-sized AABB positioned in 0.
  static const AABB<T, n> ZERO;
  //! \brief Define a square/cubic AABB centered in 0 with dimension set to 1.
  static const AABB<T, n> UNIT_SCALE;

private:

  operator bool();

protected:

  //! \brief
  void checkWellFormed() const
  {
    std::stringstream msg;
    char c = 'X';
    bool res = true;

    for (uint32_t i = 0U; i < n; ++i)
      {
        msg << c << "-axis: (" << m_bbmin[i] << ", " << m_bbmax[i] << ")";
        if (m_bbmin[i] > m_bbmax[i])
          {
            msg << ": The minimum corner of the box must be less than or equal to maximum corner";
            res = false;
          }
        msg << '\n';
        ++c;
      }
    if (false == res)
      {
        throw std::out_of_range(msg.str().c_str());
      }
  }

public:

  union
  {
    Vector<T, n> bboxes[2U];
    struct
    {
      Vector<T, n> m_bbmin;
      Vector<T, n> m_bbmax;
    };
  };
};

template <typename T, uint32_t n>
AABB<T, n> operator+(T const &scalar, AABB<T, n> const &aabb)
{
  return AABB<T, n>(scalar + aabb.m_bbmin, scalar + aabb.m_bbmax);
}

template <typename T, uint32_t n>
AABB<T, n> operator-(T const &scalar, AABB<T, n> const &aabb)
{
  return AABB<T, n>(scalar - aabb.m_bbmax, scalar - aabb.m_bbmin);
}

template <typename T, uint32_t n>
AABB<T, n> operator*(T const &scalar, AABB<T, n> const &aabb)
{
  return AABB<T, n>(scalar * aabb.m_bbmin, scalar * aabb.m_bbmax);
}

template <typename T, uint32_t n>
AABB<T, n> operator/(T const &scalar, AABB<T, n> const &aabb)
{
  return AABB<T, n>(scalar / aabb.m_bbmax, scalar / aabb.m_bbmin);
}

// Suppressed because too dangerous: can create malformed AABB
#  if 0
#    define DEFINE_BINARY_OPERATORS(op)                                  \
  /* Scalar-AABB op */                                                   \
   template <typename T, uint32_t n>                                     \
   AABB<T, n> operator op(T const &scalar, AABB<T, n> const &aabb)       \
   {                                                                     \
     return AABB<T, n>(scalar op aabb.m_bbmin, scalar op aabb.m_bbmax);  \
   }                                                                     \
   /* AABB-scalar op */                                                  \
   template <typename T, uint32_t n>                                     \
   AABB<T, n> operator op(AABB<T, n> const &aabb, T const &scalar)       \
   {                                                                     \
     return AABB<T, n>(aabb.m_bbmin op scalar, aabb.m_bbmax op scalar);  \
   }                                                                     \
   /* Vector-AABB op */                                                  \
   template <typename T, uint32_t n>                                     \
   AABB<T, n> operator op(Vector<T, n> const &vect, AABB<T, n> const &aabb) \
   {                                                                     \
     return AABB<T, n>(vect op aabb.m_bbmin, vect op aabb.m_bbmax);      \
   }                                                                     \
   /* AABB-Vector op */                                                  \
   template <typename T, uint32_t n>                                     \
   AABB<T, n> operator op(AABB<T, n> const &aabb, Vector<T, n> const &vect) \
   {                                                                     \
     return AABB<T, n>(aabb.m_bbmin op vect, aabb.m_bbmax op vect);      \
   }

//! \brief Move the box of the given scalar.
DEFINE_BINARY_OPERATORS(+);
//! \brief Move the box of the given scalar.
DEFINE_BINARY_OPERATORS(-);
//! \brief Scale the box of the given scalar. It's a different
//! behavior that the scale().
DEFINE_BINARY_OPERATORS(*);
//! \brief Scale the box of the given scalar. It's a different
//! behavior that the scale().
DEFINE_BINARY_OPERATORS(/);
#  endif

#  define DEFINE_BOOL_OPERATOR(name, op)                        \
  template <typename T, uint32_t n>                             \
  bool name(AABB<T, n> const &a, AABB<T, n> const &b)           \
  {                                                             \
    uint32_t i = n;                                             \
                                                                \
    while (i--)                                                 \
      {                                                         \
        if (!(a.m_bbmin[i] op b.m_bbmin[i]))                    \
          return false;                                         \
        if (!(a.m_bbmax[i] op b.m_bbmax[i]))                    \
          return false;                                         \
      }                                                         \
    return true;                                                \
  }

//! \brief Check if two boxes are at the same position.
DEFINE_BOOL_OPERATOR(eq, ==)
//! \brief Check if two boxes are not at the same position.
DEFINE_BOOL_OPERATOR(ne, !=)
DEFINE_BOOL_OPERATOR(ge, >=)
DEFINE_BOOL_OPERATOR(gt, >)
DEFINE_BOOL_OPERATOR(le, <=)
DEFINE_BOOL_OPERATOR(lt, <)

#  undef DEFINE_BINARY_OPERATORS
#  undef DEFINE_BOOL_OPERATOR
#  undef DEFINE_INPLACE_OPERATORS

//! \brief Permutate two bounding boxes.
template <typename T, uint32_t n>
inline void swap(AABB<T, n> &a, AABB<T, n> &b)
{
  if (&a != &b)
    {
      vector::swap(a.m_bbmin, b.m_bbmin);
      vector::swap(a.m_bbmax, b.m_bbmax);
    }
}

//! \brief Return the smallest box fitting the union of two boxes
//! (boxes combinaison).
template <typename T, uint32_t n>
inline AABB<T, n> merge(AABB<T, n> const &a, AABB<T, n> const &b)
{
  return AABB<T, n>(vector::min(a.m_bbmin, b.m_bbmin), vector::max(a.m_bbmax, b.m_bbmax));
}

//! \brief Return the box of the intersection of two boxes. In the
//! case of boxes do not collide. It return a dummy box.
template <typename T, uint32_t n>
inline AABB<T, n> intersection(AABB<T, n> const &a, AABB<T, n> const &b)
{
  if (a.collides(b))
    return AABB<T, n>(vector::max(a.m_bbmin, b.m_bbmin), vector::min(a.m_bbmax, b.m_bbmax));
  return AABB<T, n>::DUMMY;
}

//! \brief Constrain the box dimension to the limit given by the AABB.
template <typename T, uint32_t n>
inline AABB<T, n> clamp(AABB<T, n> const &a, AABB<T, n> const &b)
{
  return AABB<T, n>(::clamp(a.m_bbmin, b.m_bbmin), ::clamp(a.m_bbmax, b.m_bbmax));
}

template <typename T, uint32_t n> const AABB<T, n> AABB<T, n>::DUMMY(Vector<T, n>(NAN), Vector<T, n>(NAN));
template <typename T, uint32_t n> const AABB<T, n> AABB<T, n>::ZERO(Vector<T, n>(0), Vector<T, n>(0));
template <typename T, uint32_t n> const AABB<T, n> AABB<T, n>::UNIT_SCALE(Vector<T, n>(-0.5f), Vector<T, n>(0.5f));
template <typename T, uint32_t n> const AABB<T, n> AABB<T, n>::INFINITE(Vector<T, n>((T) std::numeric_limits<T>::lowest()),
                                                                        Vector<T, n>((T)std::numeric_limits<T>::max()));

typedef AABB<int32_t, 2U> AABB2i;
typedef AABB<int32_t, 3U> AABB3i;
typedef AABB<float, 2U> AABB2f;
typedef AABB<float, 3U> AABB3f;
typedef AABB<double, 2U> AABB2g;
typedef AABB<double, 3U> AABB3g;

#endif /* BOUNDINGBOX_TPP_ */

// -*- c++ -*- Coloration Syntaxique pour Emacs
#ifndef VECTOR_TPP_
#  define VECTOR_TPP_

// *************************************************************************************************
// Inspired from https://github.com/Reedbeta/reed-util
// and http://www.reedbeta.com/blog/on-vector-math-libraries/
// and http://www.ogre3d.org
// *************************************************************************************************

#  include "Maths.hpp"
#  include <cstdint>
#  include <initializer_list>
#  include <algorithm>
#  include <iostream>

// *************************************************************************************************
//! \brief Macro for building constructors
// *************************************************************************************************
#define VECTOR_DIM(N)                                                   \
  /*! \brief Empty constructor */                                       \
  Vector()                                                              \
  {                                                                     \
    static_assert(N >= 2U, "Minimun dimension for a vector is 2");      \
  }                                                                     \
                                                                        \
  /*! \brief Constructor with initialization list */                    \
  Vector(std::initializer_list<T> initList)                             \
  {                                                                     \
    const uint32_t m = std::min(N, uint32_t(initList.size()));          \
    auto iter = initList.begin();                                       \
    for (uint32_t i = 0; i < m; ++i)                                    \
      {                                                                 \
        m_data[i] = *iter;                                              \
        ++iter;                                                         \
      }                                                                 \
                                                                        \
    /* Zero-fill any remaining elements */                              \
    for (uint32_t i = m; i < N; ++i)                                    \
      {                                                                 \
        m_data[i] = T(0);                                               \
      }                                                                 \
  }                                                                     \
                                                                        \
  /*! \brief Constructor with uniform value */                          \
  explicit Vector(T const a)                                            \
  {                                                                     \
    uint32_t i = N;                                                     \
    while (i--)                                                         \
      {                                                                 \
        m_data[i] = a;                                                  \
      }                                                                 \
  }                                                                     \
                                                                        \
  /*! \brief Constructor by copy */                                     \
  template <typename U, uint32_t nOther>                                \
  explicit Vector(Vector<U, nOther> const &v)                           \
  {                                                                     \
    const uint32_t m = std::min(N, nOther);                             \
    uint32_t i = m;                                                     \
    while (i--)                                                         \
      {                                                                 \
        m_data[i] = T(v[i]);                                            \
      }                                                                 \
                                                                        \
    /* Zero-fill any remaining elements */                              \
    for (i = m; i < N; ++i)                                             \
      {                                                                 \
        m_data[i] = T(0);                                               \
      }                                                                 \
  }                                                                     \
                                                                        \
  /*! \brief Return the dimension */                                    \
  inline uint32_t size() const { return N; }                            \
                                                                        \
  /* Accessors */                                                       \
  T& operator[](uint32_t i)             { return m_data[i]; }           \
  const T& operator[](uint32_t i) const { return m_data[i]; }           \
  T& operator[](int i)                  { return m_data[i]; }           \
  const T& operator[](int i) const      { return m_data[i]; }           \
                                                                        \
  /* C array conversions */                                             \
  typedef T(&array_t)[N];                                               \
  typedef const T(&const_array_t)[N];                                   \
  operator array_t ()                   { return m_data; }              \
  operator const_array_t () const       { return m_data; }              \
                                                                        \
  private:                                                              \
                                                                        \
  /* Disallow bool conversions (without this, they'd happen implicitly*/\
  /* via the array conversions) */                                      \
  operator bool();

// *************************************************************************************************
//! \brief Generic mathematic vector: T for the type (float, int) and n the vector dimension
// *************************************************************************************************
template <typename T, uint32_t n>
class Vector
{
public:

  VECTOR_DIM(n);

protected:

  T m_data[n];
};

// *************************************************************************************************
//! \brief Specialization for n = 2
// *************************************************************************************************
template <typename T>
class Vector<T, 2U>
{
public:

  Vector(const T x, const T y)
  {
    this->x = x;
    this->y = y;
  }

  VECTOR_DIM(2U);

protected:

  union
  {
    T m_data[2U];
    struct { T x, y; };
    struct { T u, v; };
  };

public:

  const static Vector<T, 2U> DUMMY;
  const static Vector<T, 2U> ZERO;
  const static Vector<T, 2U> UNIT_SCALE;
  const static Vector<T, 2U> NEGATIVE_UNIT_SCALE;
  const static Vector<T, 2U> UNIT_X;
  const static Vector<T, 2U> UNIT_Y;
  const static Vector<T, 2U> NEGATIVE_UNIT_X;
  const static Vector<T, 2U> NEGATIVE_UNIT_Y;
};

// Predifined vectors
template <typename T> const Vector<T, 2U> Vector<T, 2U>::DUMMY(T(NAN));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::ZERO(T(0));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::UNIT_SCALE(T(1));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::NEGATIVE_UNIT_SCALE(T(-1));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::UNIT_X(T(1), T(0));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::UNIT_Y(T(0), T(1));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::NEGATIVE_UNIT_X(T(-1), T(0));
template <typename T> const Vector<T, 2U> Vector<T, 2U>::NEGATIVE_UNIT_Y(T(0), T(-1));

// *************************************************************************************************
//! \brief Specialization for n = 3
// *************************************************************************************************
template <typename T>
class Vector<T, 3U>
{
public:

  Vector(Vector<T, 2U> const &v, const T z = T(0))
  {
    this->x = v.x;
    this->y = v.y;
    this->z = z;
  }

  Vector(const T x, const T y, const T z = T(0))
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  VECTOR_DIM(3U);

protected:

  union
  {
    T m_data[3U];
    struct { T x, y, z; };
    struct { T r, g, b; };
  };

public:

  const static Vector<T, 3U> DUMMY;
  const static Vector<T, 3U> ZERO;
  const static Vector<T, 3U> UNIT_SCALE;
  const static Vector<T, 3U> NEGATIVE_UNIT_SCALE;
  const static Vector<T, 3U> UNIT_X;
  const static Vector<T, 3U> UNIT_Y;
  const static Vector<T, 3U> UNIT_Z;
  const static Vector<T, 3U> NEGATIVE_UNIT_X;
  const static Vector<T, 3U> NEGATIVE_UNIT_Y;
  const static Vector<T, 3U> NEGATIVE_UNIT_Z;
};

template <typename T> const Vector<T, 3U> Vector<T, 3U>::DUMMY(T(NAN));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::ZERO(T(0));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::UNIT_SCALE(T(1));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::NEGATIVE_UNIT_SCALE(T(-1));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::UNIT_X(T(1), T(0), T(0));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::UNIT_Y(T(0), T(1), T(0));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::UNIT_Z(T(0), T(0), T(1));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::NEGATIVE_UNIT_X(T(-1), T(0), T(0));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::NEGATIVE_UNIT_Y(T(0), T(-1), T(0));
template <typename T> const Vector<T, 3U> Vector<T, 3U>::NEGATIVE_UNIT_Z(T(0), T(0), T(-1));

// *************************************************************************************************
// Specializations for n = 4
// *************************************************************************************************
template <typename T>
class Vector<T, 4U>
{
public:

  Vector(Vector<T, 3U> const &v, const T w = T(0))
  {
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
  }

  Vector(const T x, const T y, const T z, const T w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  VECTOR_DIM(4U);

protected:

  union
  {
    T m_data[4U];
    struct { T x, y, z, w; };
    struct { T r, g, b, a; };
  };

public:

  const static Vector<T, 4U> DUMMY;
  const static Vector<T, 4U> ZERO;
  const static Vector<T, 4U> UNIT_SCALE;
  const static Vector<T, 4U> NEGATIVE_UNIT_SCALE;
  const static Vector<T, 4U> UNIT_X;
  const static Vector<T, 4U> UNIT_Y;
  const static Vector<T, 4U> UNIT_Z;
  const static Vector<T, 4U> UNIT_W;
  const static Vector<T, 4U> NEGATIVE_UNIT_X;
  const static Vector<T, 4U> NEGATIVE_UNIT_Y;
  const static Vector<T, 4U> NEGATIVE_UNIT_Z;
  const static Vector<T, 4U> NEGATIVE_UNIT_W;
};

template <typename T> const Vector<T, 4U> Vector<T, 4U>::DUMMY(T(NAN));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::ZERO(T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::UNIT_SCALE(T(1));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::NEGATIVE_UNIT_SCALE(T(-1));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::UNIT_X(T(1), T(0), T(0), T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::UNIT_Y(T(0), T(1), T(0), T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::UNIT_Z(T(0), T(0), T(1), T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::UNIT_W(T(0), T(0), T(0), T(1));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::NEGATIVE_UNIT_X(T(-1), T(0), T(0), T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::NEGATIVE_UNIT_Y(T(0), T(-1), T(0), T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::NEGATIVE_UNIT_Z(T(0), T(0), T(-1), T(0));
template <typename T> const Vector<T, 4U> Vector<T, 4U>::NEGATIVE_UNIT_W(T(0), T(0), T(0), T(-1));

// *************************************************************************************************
// Overloaded math operators
// *************************************************************************************************

#define DEFINE_UNARY_OPERATOR(op)                       \
  template <typename T, uint32_t n>                     \
  Vector<T, n> operator op (Vector<T, n> const &a)      \
  {                                                     \
    Vector<T, n> result;                                \
    uint32_t i = n;                                     \
    while (i--)                                         \
      result[i] = op a[i];                              \
    return result;                                      \
  }

#define DEFINE_BINARY_OPERATORS(op)                                     \
  /* Vector-Vector op */                                                \
  template <typename T, uint32_t n>                                     \
  Vector<T, n> operator op (Vector<T, n> const &a, Vector<T, n> const &b) \
  {                                                                     \
    Vector<T, n> result;                                                \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      result[i] = a[i] op b[i];                                         \
    return result;                                                      \
  }                                                                     \
  /* Scalar-Vector op */                                                \
  template <typename T, uint32_t n>                                     \
  Vector<T, n> operator op (T const a, Vector<T, n> const &b)           \
  {                                                                     \
    Vector<T, n> result;                                                \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      result[i] = a op b[i];                                            \
    return result;                                                      \
  }                                                                     \
  /* Vector-scalar op */                                                \
  template <typename T, uint32_t n>                                     \
  Vector<T, n> operator op (Vector<T, n> const &a, T const b)           \
  {                                                                     \
    Vector<T, n> result;                                                \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      result[i] = a[i] op b;                                            \
    return result;                                                      \
  }

#define DEFINE_INPLACE_OPERATORS(op)                                    \
  /* Vector-Vector op */                                                \
  template <typename T, uint32_t n>                                     \
  Vector<T, n>& operator op (Vector<T, n> &a, Vector<T, n> const &b)    \
  {                                                                     \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      a[i] op b[i];                                                     \
    return a;                                                           \
  }                                                                     \
  /* Vector-scalar op */                                                \
  template <typename T, uint32_t n>                                     \
  Vector<T, n>& operator op (Vector<T, n> &a, T const b)                \
  {                                                                     \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      a[i] op b;                                                        \
    return a;                                                           \
  }

#define DEFINE_RELATIONAL_OPERATORS(op)                                 \
  /* Vector-Vector op */                                                \
  template <typename T, typename U, uint32_t n>                         \
  Vector<bool, n> operator op (Vector<T, n> const &a, Vector<U, n> const &b) \
  {                                                                     \
    Vector<bool, n> result;                                             \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      result[i] = a[i] op b[i];                                         \
    return result;                                                      \
  }                                                                     \
  /* Scalar-Vector op */                                                \
  template <typename T, typename U, uint32_t n>                         \
  Vector<bool, n> operator op (T const &a, Vector<U, n> const &b)       \
  {                                                                     \
    Vector<bool, n> result;                                             \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      result[i] = a op b[i];                                            \
    return result;                                                      \
  }                                                                     \
  /* Vector-scalar op */                                                \
  template <typename T, typename U, uint32_t n>                         \
  Vector<bool, n> operator op (Vector<T, n> const &a, U const &b)       \
  {                                                                     \
    Vector<bool, n> result;                                             \
    uint32_t i = n;                                                     \
    while (i--)                                                         \
      result[i] = a[i] op b;                                            \
    return result;                                                      \
  }

#define DEFINE_FUN1_OPERATOR(name, op)                          \
  template <typename T, uint32_t n>                             \
  Vector<T, n> name(Vector<T, n> const &a)                      \
  {                                                             \
    Vector<T, n> result;                                        \
    uint32_t i = n;                                             \
                                                                \
    while (i--)                                                 \
      result[i] = op(a[i]);                                     \
    return result;                                              \
  }

#define DEFINE_FUN2_OPERATOR(name, op)                          \
  template <typename T, uint32_t n>                             \
  Vector<T, n> name(Vector<T, n> const &a, Vector<T, n> const &b) \
  {                                                             \
    Vector<T, n> result;                                        \
    uint32_t i = n;                                             \
                                                                \
    while (i--)                                                 \
      result[i] = op(a[i], b[i]);                               \
    return result;                                              \
  }

#define DEFINE_BOOL_OPERATOR(name, op)                          \
  template <typename T, uint32_t n>                             \
  bool name(Vector<T, n> const &a, Vector<T, n> const &b)       \
  {                                                             \
    uint32_t i = n;                                             \
                                                                \
    while (i--)                                                 \
      {                                                         \
        if (!(a[i] op b[i]))                                    \
          return false;                                         \
      }                                                         \
    return true;                                                \
  }

DEFINE_BINARY_OPERATORS(+)
DEFINE_BINARY_OPERATORS(-)
DEFINE_UNARY_OPERATOR(-)
DEFINE_UNARY_OPERATOR(+)
DEFINE_BINARY_OPERATORS(*)
DEFINE_BINARY_OPERATORS(/)
DEFINE_BINARY_OPERATORS(&)
DEFINE_BINARY_OPERATORS(|)
DEFINE_BINARY_OPERATORS(^)
DEFINE_UNARY_OPERATOR(!)
DEFINE_UNARY_OPERATOR(~)

DEFINE_INPLACE_OPERATORS(+=)
DEFINE_INPLACE_OPERATORS(-=)
DEFINE_INPLACE_OPERATORS(*=)
DEFINE_INPLACE_OPERATORS(/=)
DEFINE_INPLACE_OPERATORS(&=)
DEFINE_INPLACE_OPERATORS(|=)
DEFINE_INPLACE_OPERATORS(^=)

DEFINE_RELATIONAL_OPERATORS(==)
DEFINE_RELATIONAL_OPERATORS(!=)
DEFINE_RELATIONAL_OPERATORS(<)
DEFINE_RELATIONAL_OPERATORS(>)
DEFINE_RELATIONAL_OPERATORS(<=)
DEFINE_RELATIONAL_OPERATORS(>=)

namespace vector
{
  //! \brief Return a matrix where each e
  DEFINE_FUN2_OPERATOR(min, std::min)
  DEFINE_FUN2_OPERATOR(max, std::max)
  DEFINE_FUN1_OPERATOR(abs, maths::abs)
  DEFINE_BOOL_OPERATOR(ge, >=)
  DEFINE_BOOL_OPERATOR(gt, >)
  DEFINE_BOOL_OPERATOR(le, <=)
  DEFINE_BOOL_OPERATOR(lt, <)

  //! \brief Compare the integer values for each elements of vectors.
  //! \note Do not confuse this function with operator==() or the
  //! function equivalent() which do not have the same behavior.
  //! \return true if all elements have the same value.
  template <typename T, uint32_t n>
  inline typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
  eq(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    if (&a != &b)
      {
        uint32_t i = n;
        while (i--)
          {
            if (a[i] != b[i])
              return false;
          }
      }
    return true;
  }

  //! \brief Compare the float values for each elements of vectors.
  //! \note Do not confuse this function with operator==() or the
  //! function equivalent() which do not have the same behavior.
  //! \return true if all elements have the same value.
  template <typename T, uint32_t n>
  inline typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
  eq(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    if (&a != &b)
      {
        uint32_t i = n;
        while (i--)
          {
            if (!maths::almostEqual(a[i], b[i]))
              return false;
          }
      }
    return true;
  }

  template <typename T, uint32_t n>
  inline bool ne(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return !vector::eq(a, b);
  }

  template <typename T, uint32_t n>
  void swap(Vector<T, n> &a, Vector<T, n> &b)
  {
    if (&a != &b)
      {
        uint32_t i = n;
        while (i--)
          {
            std::swap(a[i], b[i]);
          }
      }
  }

  //! \brief Get the coeficient of collinearity (k) of two vectors (u and v).
  //!
  //! Two non-null vectors are collinear if and only it exists a
  //! scalar k != 0, where: u = k v (note: if u is a null vector, any
  //! vector v is collinear to u because k = 0).
  //!
  //! \return Nan if vectors are not collinear. Return 0 if Return k if they are collinear.
  //!
  //! \note Use this function for T a familly of float and not for integers.
  template <typename T, uint32_t n>
  T collinearity(Vector<T, n> const &u, Vector<T, n> const &v)
  {
    // Null vector ?
    if (maths::almostZero(u[0]) || maths::almostZero(v[0]))
      return T(0);

    const T k = u[0] / v[0];
    for (uint32_t i = 1U; i < n; ++i)
      {
        if (!maths::almostEqual(k * v[i], u[i]))
          return T(NAN);
      }
    return k;
  }

  //! \brief Check if two vectors are parallels.
  template <typename T, uint32_t n>
  bool collinear(Vector<T, n> const &u, Vector<T, n> const &v)
  {
    T k = collinearity(u, v);
    return !isnan(k);
  }

  //! \brief Check if two vectors are mathematicaly equivalent: same
  //! norm (length), same direction (parallel) and same sign.
  template <typename T, uint32_t n>
  bool equivalent(Vector<T, n> const &u, Vector<T, n> const &v)
  {
    T k = collinearity(u, v);
    return maths::almostEqual(k, T(1));
  }

  //! \brief Check if three points A, B, C are aligned.
  template <typename T, uint32_t n>
  bool arePointsAligned(Vector<T, n> const &a, Vector<T, n> const &b, Vector<T, n> const &c)
  {
    return collinear(b - a, c - a);
  }

// collinear
// http://www.educastream.com/vecteurs-colineaires-seconde

// FIXME u et v colineaires s'il existe k: v = ku
// FIXME bool colineaire(v1, v2) // [2 -3] et [4 -6]
// ==> x1y2 == x2y1
// x1/x2 == y1/y2 == z1/z2 == k

// colineaire: [3 -2] et [-15 10]
// Non col (6 4) (4 2); (3 -2) (12 -5)

// ==> paralles: ssi colineaire
// aligned(A, B, C): colineare(AB, AC)

  //! \brief Constrain each value of the vectorto lower and upper bounds.
  template <typename T, uint32_t n>
  Vector<T, n> clamp(Vector<T, n> const &a, T const lower, T const upper)
  {
    Vector<T, n> result;
    uint32_t i = n;

    while (i--)
      result[i] = maths::clamp(a[i], lower, upper);
    return result;
  }

  //! \brief Dot product.
  template <typename T, uint32_t n>
  T dot(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    T result(0);
    uint32_t i = n;

    while (i--)
      result += a[i] * b[i];
    return result;
  }

  template <typename T, uint32_t n>
  inline T squaredLength(Vector<T, n> const &a)
  {
    return dot(a, a);
  }

  template <typename T, uint32_t n>
  inline T length(Vector<T, n> const &a)
  {
    return maths::sqrt(squaredLength(a));
  }

  template <typename T, uint32_t n>
  inline T norm(Vector<T, n> const &a)
  {
    return length(a);
  }

  template <typename T, uint32_t n>
  inline T squaredDistance(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return squaredLength(a - b);
  }

  template <typename T, uint32_t n>
  inline T distance(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return maths::sqrt(squaredDistance(a, b));
  }

  template <typename T, uint32_t n>
  inline Vector<T, n> normalize(Vector<T, n> const &a)
  {
    return a / length(a);
  }

  template <typename T, uint32_t n>
  inline Vector<T, n> middle(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    Vector<T, n> result;
    uint32_t i = n;

    while (i--)
      result[i] = (a[i] + b[i]) / T(2);
    return result;
  }

  template <typename T>
  inline Vector<T, 3U> cross(Vector<T, 3U> const &a, Vector<T, 3> const &b)
  {
    return
      {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
      };
  }

  //! \brief Perpendicular
  template <typename T>
  inline Vector<T, 2U> orthogonal(Vector<T, 2U> const &a)
  {
    return { -a.y, a.x };
  }

  template <typename T>
  inline Vector<T, 3U> orthogonal(Vector<T, 3U> const &a)
  {
    // Implementation due to Sam Hocevar - see blog post:
    // http://lolengine.net/blog/2013/09/21/picking-orthogonal-Vector-combing-coconuts
    if (maths::abs(a.x) > maths::abs(a.z))
      return { -a.y, a.x, T(0) };
    else
      return { T(0), -a.z, a.y };
  }

  template <typename T, uint32_t n>
  inline typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type
  orthogonal(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return T(0) == dot(a, b);
  }

  template <typename T, uint32_t n>
  inline typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
  orthogonal(Vector<T, n> const &a, Vector<T, n> const &b)
  {
    return maths::almostZero(dot(a, b));
  }

  template <typename T, uint32_t n>
  inline T angleBetween(Vector<T, n> const &org, Vector<T, n> const &dest)
  {
    T lenProduct = norm(org) * norm(dest);

    // Divide by zero check
    //if (lenProduct < 1e-6f)
    //  lenProduct = 1e-6f;

    T f = dot(org, dest) / lenProduct;
    f = std::min(std::max(f, T(-1)), T(1));
    return std::acos(f) * 180.0 / 3.14159265;
  }

  template <typename T, uint32_t n>
  inline Vector<T, n> reflect(Vector<T, n> const &v, Vector<T, n> const &normal)
  {
    return v - (T(2) * dot(v, normal) * normal);
  }
}

template <typename T, uint32_t n>
inline std::ostream& operator<<(std::ostream& os, Vector<T, n> const& v)
{
  os << "Vector";
  for (uint32_t i = 0; i < n; ++i)
    {
      std::cout << ' ' << v[i];
    }
  return os;
}

// *************************************************************************************************
// Typedefs for the most common types and dimensions
// *************************************************************************************************

typedef Vector<bool, 2U> Vector2b;
typedef Vector<bool, 3U> Vector3b;
typedef Vector<bool, 4U> Vector4b;

typedef Vector<int32_t, 2U> Vector2i;
typedef Vector<int32_t, 3U> Vector3i;
typedef Vector<int32_t, 4U> Vector4i;

typedef Vector<float, 2U> Vector2f;
typedef Vector<float, 3U> Vector3f;
typedef Vector<float, 4U> Vector4f;

typedef Vector<double, 2U> Vector2g;
typedef Vector<double, 3U> Vector3g;
typedef Vector<double, 4U> Vector4g;

#  undef DEFINE_UNARY_OPERATOR
#  undef DEFINE_BINARY_OPERATORS
#  undef DEFINE_INPLACE_OPERATORS
#  undef DEFINE_RELATIONAL_OPERATORS
#  undef DEFINE_FUN1_OPERATOR
#  undef DEFINE_FUN2_OPERATOR
#  undef DEFINE_BOOL_OPERATOR

#endif
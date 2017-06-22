// -*- c++ -*- Coloration Syntaxique pour Emacs
#ifndef MATRIX_TPP_
#  define MATRIX_TPP_

// *************************************************************************************************
// Inspired by https://github.com/Reedbeta/reed-util and its
// blog http://www.reedbeta.com/blog/on-vector-math-libraries/
// *************************************************************************************************

#  include "Vector.tpp"

namespace matrix
{
  //! \brief Enum for constructing different kind of matrices.
  enum MatrixType { Identity };
};


template <typename T, uint32_t rows, uint32_t cols>
class Matrix
{
public:

  //! \brief Empty constructor. It does NOT initialize the matrix for efficiency.
  Matrix()
  {
  }

  //! \brief Constructor with initialization list.
  Matrix(std::initializer_list<T> initList)
  {
    uint32_t m = std::min(rows * cols, uint32_t(initList.size()));
    auto iter = initList.begin();
    for (uint32_t i = 0U; i < m; ++i)
      {
        m_data[i] = *iter;
        ++iter;
      }

    // Zero-fill any remaining elements.
    for (uint32_t i = m; i < rows * cols; ++i)
      {
        m_data[i] = T(0);
      }
  }

  //! \brief Constructor with an uniform value.
  explicit Matrix(T a)
  {
    uint32_t i = rows * cols;
    while (i--)
      {
        m_data[i] = a;
      }
  }

  //! \brief Constructor for identity matrix.
  explicit Matrix(const matrix::MatrixType type)
  {
    switch (type)
      {
      case matrix::Identity:
        static_assert(rows == cols, "Can't construct identity for a non-square matrix");
        uint32_t i = rows * cols;
        while (i--)
          {
            m_data[i] = (i % (rows + 1U) == 0) ? T(1) : T(0);
          }
        break;
      };
  }

  //! \brief Constructor by copy.
  template <typename U, uint32_t rowsOther, uint32_t colsOther>
  explicit Matrix(Matrix<U, rowsOther, colsOther> const &m)
  {
    uint32_t r = std::min(rows, rowsOther);
    uint32_t c = std::min(cols, colsOther);
    for (uint32_t i = 0U; i < r; ++i)
      {
        for (uint32_t j = 0U; j < c; ++j)
          {
            (*this)[i][j] = T(m[i][j]);
          }
        // Zero-fill any remaining cols
        for (uint32_t j = c; j < cols; ++j)
          {
            (*this)[i][j] = T(0);
          }
      }
    // Zero-fill any remaining rows
    for (uint32_t i = r * cols; i < rows * cols; ++i)
      {
        m_data[i] = T(0);
      }
  }

  //! \brief Return the dimension of the matrix.
  //! \param r (OUT) get the number of rows.
  //! \param c (OUT) get the number of columns.
  inline void size(uint32_t &r, uint32_t &c)
  {
    r = rows;
    c = cols;
  }

  //! \brief Access to the nth row in write mode.
  inline Vector<T, cols>& operator[](uint32_t i)
  {
    return reinterpret_cast<Vector<T, cols> &>(m_data[i * cols]);
  }

  //! \brief Acces to the nth row in read mode.
  inline const Vector<T, cols>& operator[](uint32_t i) const
  {
    return reinterpret_cast<const Vector<T, cols> &>(m_data[i * cols]);
  }

  //! \brief Access to the nth row in write mode.
  inline Vector<T, cols>& operator[](int i)
  {
    return reinterpret_cast<Vector<T, cols> &>(m_data[i * cols]);
  }

  //! \brief Acces to the nth row in read mode.
  inline const Vector<T, cols>& operator[](int i) const
  {
    return reinterpret_cast<const Vector<T, cols> &>(m_data[i * cols]);
  }

  // C array conversions
  typedef T (&array_t)[rows * cols];
  inline operator array_t () { return m_data; }
  typedef const T (&const_array_t)[rows * cols];
  inline operator const_array_t () const { return m_data; }

private:

  // Disallow bool conversions (without this, they'd happen implicitly
  // via the array conversions)
  operator bool();

public:

  T m_data[rows * cols];
};

// Typedefs for the most common types and dimensions
typedef Matrix<bool, 2U, 2U> Matrix22b;
typedef Matrix<bool, 3U, 3U> Matrix33b;
typedef Matrix<bool, 4U, 4U> Matrix44b;

typedef Matrix<int, 2U, 2U> Matrix22i;
typedef Matrix<int, 3U, 3U> Matrix33i;
typedef Matrix<int, 4U, 4U> Matrix44i;

typedef Matrix<double, 2U, 2U> Matrix22g;
typedef Matrix<double, 3U, 3U> Matrix33g;
typedef Matrix<double, 4U, 4U> Matrix44g;

typedef Matrix<float, 2U, 2U> Matrix22f;
typedef Matrix<float, 3U, 3U> Matrix33f;
typedef Matrix<float, 4U, 4U> Matrix44f;

// Overloaded math operators
#  define DEFINE_UNARY_OPERATOR(op)                                     \
  template <typename T, uint32_t rows, uint32_t cols>                   \
  inline Matrix<T, rows, cols> operator op (Matrix<T, rows, cols> const &a)    \
  {                                                                     \
    Matrix<T, rows, cols> result;                                       \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = op a.m_data[i];                                \
    return result;                                                      \
  }

#  define DEFINE_BINARY_SCALAR_OPERATORS(op)                            \
  /* Scalar-matrix op */                                                \
  template <typename T, uint32_t rows, uint32_t cols>                   \
  inline Matrix<T, rows, cols> operator op (T const a, Matrix<T, rows, cols> const &b) \
  {                                                                     \
    Matrix<T, rows, cols> result;                                       \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = a op b.m_data[i];                              \
    return result;                                                      \
  }                                                                     \
  /* Matrix-scalar op */                                                \
  template <typename T, uint32_t rows, uint32_t cols>                   \
  inline Matrix<T, rows, cols> operator op (Matrix<T, rows, cols> const &a, T const b) \
  {                                                                     \
    Matrix<T, rows, cols> result;                                       \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = a.m_data[i] op b;                              \
    return result;                                                      \
  }

#  define DEFINE_BINARY_OPERATORS(op)                                   \
  /* Matrix-matrix op */                                                \
  template <typename T, uint32_t rows, uint32_t cols>                   \
  inline Matrix<T, rows, cols> operator op (Matrix<T, rows, cols> const &a, Matrix<T, rows, cols> const &b) \
  {                                                                     \
    Matrix<T, rows, cols> result;                                       \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = a.m_data[i] op b.m_data[i];                    \
    return result;                                                      \
  }                                                                     \
  DEFINE_BINARY_SCALAR_OPERATORS(op)

#  define DEFINE_INPLACE_SCALAR_OPERATOR(op)                            \
  /* Matrix-scalar op */                                                \
  template <typename T, uint32_t rows, uint32_t cols>                   \
  inline Matrix<T, rows, cols> & operator op (Matrix<T, rows, cols> &a, T const b)  \
  {                                                                     \
    uint32_t i = rows * cols; while (i--)                               \
      a.m_data[i] op b;                                                 \
    return a;                                                           \
  }

#  define DEFINE_INPLACE_OPERATORS(op)                                  \
  /* Matrix-matrix op */                                                \
  template <typename T, uint32_t rows, uint32_t cols>                   \
  inline Matrix<T, rows, cols> & operator op (Matrix<T, rows, cols> &a, Matrix<T, rows, cols> const &b) \
  {                                                                     \
    uint32_t i = rows * cols; while (i--)                               \
      a.m_data[i] op b.m_data[i];                                       \
    return a;                                                           \
  }                                                                     \
  DEFINE_INPLACE_SCALAR_OPERATOR(op)

#  define DEFINE_RELATIONAL_OPERATORS(op)                               \
  /* Matrix-matrix op */                                                \
  template <typename T, typename U, uint32_t rows, uint32_t cols>       \
  inline Matrix<bool, rows, cols> operator op (Matrix<T, rows, cols> const &a, Matrix<U, rows, cols> const &b) \
  {                                                                     \
    Matrix<bool, rows, cols> result;                                    \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = a.m_data[i] op b.m_data[i];                    \
    return result;                                                      \
  }                                                                     \
  /* Scalar-matrix op */                                                \
  template <typename T, typename U, uint32_t rows, uint32_t cols>       \
  inline Matrix<bool, rows, cols> operator op (T const a, Matrix<U, rows, cols> const &b) \
  {                                                                     \
    Matrix<bool, rows, cols> result;                                    \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = a op b.m_data[i];                              \
    return result;                                                      \
  }                                                                     \
  /* Matrix-scalar op */                                                \
  template <typename T, typename U, uint32_t rows, uint32_t cols>       \
  inline Matrix<bool, rows, cols> operator op (Matrix<T, rows, cols> const &a, U const b) \
  {                                                                     \
    Matrix<bool, rows, cols> result;                                    \
    uint32_t i = rows * cols; while (i--)                               \
      result.m_data[i] = a.m_data[i] op b;                              \
    return result;                                                      \
  }

DEFINE_BINARY_OPERATORS(+)
DEFINE_BINARY_OPERATORS(-)
DEFINE_UNARY_OPERATOR(-)
DEFINE_BINARY_SCALAR_OPERATORS(*)
DEFINE_BINARY_SCALAR_OPERATORS(/)
DEFINE_BINARY_OPERATORS(&)
DEFINE_BINARY_OPERATORS(|)
DEFINE_BINARY_OPERATORS(^)
DEFINE_UNARY_OPERATOR(!)
DEFINE_UNARY_OPERATOR(~)

DEFINE_INPLACE_OPERATORS(+=)
DEFINE_INPLACE_OPERATORS(-=)
DEFINE_INPLACE_SCALAR_OPERATOR(*=)
DEFINE_INPLACE_SCALAR_OPERATOR(/=)
DEFINE_INPLACE_OPERATORS(&=)
DEFINE_INPLACE_OPERATORS(|=)
DEFINE_INPLACE_OPERATORS(^=)

DEFINE_RELATIONAL_OPERATORS(==)
DEFINE_RELATIONAL_OPERATORS(!=)
DEFINE_RELATIONAL_OPERATORS(<)
DEFINE_RELATIONAL_OPERATORS(>)
DEFINE_RELATIONAL_OPERATORS(<=)
DEFINE_RELATIONAL_OPERATORS(>=)

namespace matrix
{

  //! \brief Compare each elements of two matrices, check if they have
  //! the same value +/- epsilon and store the comparaison result in a
  //! boolean matrix.
  //! \return the boolean matrix containing the result of each element
  //! comparaison.
  template <typename T, uint32_t rows, uint32_t cols>
  inline Matrix<bool, rows, cols> compare(Matrix<T, rows, cols> const &a, Matrix<T, cols, cols> const &b, T const epsilon)
  {
    Matrix<bool, rows, cols> result;
    uint32_t i = rows * cols;

    while (i--)
      result.m_data[i] = (std::abs(a.m_data[i] - b.m_data[i]) < epsilon);
    return result;
  }

  //! \brief Matrix-Matrix multiplication.
  template <typename T, uint32_t rows, uint32_t inner, uint32_t cols>
  inline Matrix<T, rows, cols> operator*(Matrix<T, rows, inner> const &a, Matrix<T, inner, cols> const &b)
  {
    Matrix<T, rows, cols> result(T(0));
    for (uint32_t i = 0U; i < rows; ++i)
      for (uint32_t j = 0U; j < cols; ++j)
        for (uint32_t k = 0; k < inner; ++k)
          result[i][j] += a[i][k] * b[k][j];
    return result;
  }

  //! \brief Matrix-Vector multiplication.
  template <typename T, uint32_t rows, uint32_t cols>
  inline Vector<T, rows> operator*(Matrix<T, rows, cols> const &a, Vector<T, cols> const &b)
  {
    Vector<T, rows> result(T(0));
    uint32_t i = rows;
    uint32_t j = cols;
    while (i--)
      while (j--)
        result[i] += a[i][j] * b[j];
    return result;
  }

  //! \brief Vector-Matrix multiplication.
  template <typename T, uint32_t rows, uint32_t cols>
  inline Vector<T, cols> operator*(Vector<T, rows> const &a, Matrix<T, rows, cols> const &b)
  {
    Vector<T, cols> result (T(0));
    uint32_t i = rows;
    uint32_t j = cols;
    while (i--)
      while (j--)
        result[j] += a[i] * b[i][j];
    return result;
  }

  //! \brief Hadamard product (element by element multiplication).
  //! See https://en.wikipedia.org/wiki/Hadamard_product_(matrices)
  template <typename T, uint32_t rows, uint32_t cols>
  inline Matrix<T, rows, cols> Hprod(Matrix<T, rows, cols> const &a, Matrix<T, rows, cols> const &b)
  {
    Matrix<T, rows, cols> result;
    uint32_t i = rows;
    uint32_t j = cols;
    while (i--)
      while (j--)
        result[i][j] = a[i][j] * b[i][j];
    return result;
  }

  //! \brief vector = vector * Matrix
  template <typename T, uint32_t n>
  inline Vector<T, n>& operator*=(Vector<T, n> &a, Matrix<T, n, n> const &b)
  {
    a = a * b;
    return a;
  }

  //! \brief Hadamard product.
  template <typename T, uint32_t rows, uint32_t cols>
  inline Matrix<T, rows, cols>& operator*=(Matrix<T, rows, cols> &a, Matrix<T, cols, cols> const &b)
  {
    a = a * b;
    return a;
  }

  //! \brief Transpose the matrix.
  template <typename T, uint32_t rows, uint32_t cols>
  inline Matrix<T, cols, rows> transpose(Matrix<T, rows, cols> const &a)
  {
    Matrix<T, cols, rows> result;

    uint32_t i = rows;
    uint32_t j = cols;
    while (i--)
      while (j--)
        result[j][i] = a[i][j];
    return result;
  }

  //! \brief Compute the matrix trace. The matrix shall be a squared matrix.
  template <typename T, uint32_t rows, uint32_t cols>
  inline T trace(Matrix<T, rows, cols> const &a)
  {
    static_assert(rows == cols, "Can't compute the trace of a non-square matrix");

    T result = T(0);
    uint32_t i = rows;

    while (i--)
      result += a[i][i];

    return result;
  }

  //! \brief Check if the matrix is diagonal. The matrix shall be a squared matrix.
  template <typename T, uint32_t rows, uint32_t cols>
  inline bool isDiagonal(Matrix<T, rows, cols> const &a, T const epsilon)
  {
    static_assert(rows == cols, "Can't compute the diagonal of a non-square matrix");

    uint32_t i = rows;
    uint32_t j = cols;

    while (i--)
      {
        while (j--)
          {
            if (i != j)
              {
                if (std::abs(a[i][j]) > epsilon)
                  return false;
              }
          }
      }
    return true;
  }

  //! \brief Check if the matrix is symetric. The matrix shall be a squared matrix.
  template <typename T, uint32_t rows, uint32_t cols>
  inline bool isSymmetric(Matrix<T, rows, cols> const &a, T const epsilon)
  {
    static_assert(rows == cols, "Can't compute the diagonal of a non-square matrix");

    uint32_t i = rows;
    uint32_t j = cols;

    while (i--)
      {
        while (j--)
          {
            if (i != j)
              {
                if (std::abs(a[i][j] - a[j][i]) > epsilon)
                  return false;
              }
          }
      }
    return true;
  }

  //! \brief Check if the boolean matrix has all its elements true.
  template <uint32_t rows, uint32_t cols>
  inline bool isTrue(Matrix<bool, rows, cols> const &a)
  {
    uint32_t i = rows * cols;

    while (i--)
      if (false == a.m_data[i])
        return false;
    return true;
  }

  //! \brief Check if the boolean matrix has all its elements false.
  template <uint32_t rows, uint32_t cols>
  inline bool isFalse(Matrix<bool, rows, cols> const &a)
  {
    uint32_t i = rows * cols;

    while (i--)
      if (true == a.m_data[i])
        return false;
    return true;
  }

  template <typename T, uint32_t rows, uint32_t cols>
  inline bool swapRows(Matrix<bool, rows, cols> const &a, uint32_t const i, uint32_t const j)
  {
    if (i == j)
      return true;

    if ((i >= rows) || (j >= rows))
      return false;

    Vector<T, cols> tmp(a[i]);
    a[i] = a[j];
    a[j] = tmp;
    return true;
  }
}

//! \brief Display the matrix.
template <typename T, uint32_t rows, uint32_t cols>
inline std::ostream& operator<<(std::ostream& os, Matrix<T, rows, cols> const& m)
{
  for (uint32_t i = 0U; i < rows; ++i)
    {
      for (uint32_t j = 0U; j < cols; ++j)
        std::cout << m[i][j] << " ";
      std::cout << std::endl;
    }
  return os;
}

#  undef DEFINE_UNARY_OPERATOR
#  undef DEFINE_BINARY_SCALAR_OPERATORS
#  undef DEFINE_BINARY_OPERATORS
#  undef DEFINE_INPLACE_SCALAR_OPERATOR
#  undef DEFINE_INPLACE_OPERATORS
#  undef DEFINE_RELATIONAL_OPERATORS

#endif

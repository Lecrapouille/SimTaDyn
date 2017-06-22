#ifndef POLYFIT_HPP_
#  define POLYFIT_HPP_

#  include "Matrix.hpp"
#  include "Polynom.hpp"
#  include <cmath>

// *************************************************************************************************
//! \brief Recursive Least Square method
// *************************************************************************************************
template <uint32_t order>
class PolyFit
{
public:

  //! \brief Constructor and define the order of the polynom
  PolyFit()
  {
    reset();
  }

  inline uint32_t order() const
  {
    return order + 1U;
  }

  //! \brief Reset states to initial values.
  void reset()
  {
    m_b * 0.0;
    m_Xsquared * 0.0;
    m_iteration = 0;
  }

  //! \brief Accumulate new raw datum from axis X and Y.
  void process(const double dataX, const double dataY)
  {
    assert(isfinite(dataX));
    assert(isfinite(dataY));

    // Compute [X^0 X^1 X^2 ... X^(2*order)]
    double X[2U * order];
    X[0U] = 1.0; X[1U] = dataX;
    for (uint32_t i = 2U; i < 2U * order; ++i)
      {
        X[i] = X[i - 1U] * dataX;
      }

    // Compute m_Xsquared
    const double it = m_iteration;
    for (uint32_t i = 0U; i < order; ++i)
    {
      for (uint32_t j = 0U; j < order; ++j)
        {
          // The following code suffers of accurancy due to
          // implementation of float and doubles:
          //   m_Xsquared[i][j] = m_Xsquared[i][j] + X[i + j];
          // The follow one, less:
          m_Xsquared[i][j] = (it * m_Xsquared[i][j] + X[i + j]) / (it + 1.0);
        }
    }

    // Compute m_b
    for (uint32_t i = 0U; i < m_order; ++i)
    {
      // The following code suffers of accurancy:
      //   b->element[i] = b->element[i] + X[i] * DataY;
      // The follow one, less:
      m_b[i] = (it * m_b[i] + X[i] * dataY) / (it + 1.0);
    }

    ++m_iteration;
  }

  //! \brief Compute the polynom fitting datum.
  Polynom end()
  {
    Matrix<double, order, order> P(matrix::Identity);
    Matrix<double, order, order> L;
    Matrix<double, order, order> U;
    Matrix<double, order, order> P;

    LUDecomposition(m_Xsquared, L, U, P);
    return LUSolve(L, U, P, m_b);
  }

  //! \brief Apply the polynom on a given data
  double apply(Polynom const& P, Vector<T, order> const& X) const
  {
    assert(X.size() + 1U >= P.degree());

    double res = P[0];
    for (uint32_t i = 1U; i < P.degree(); ++i)
      {
        res += (P[i] * X[i - 1]);
      }
    return res;
  }

protected:

  uint32_t                     m_iteration;
  Matrix<double, order, order> m_Xsquared;
  Vector<double, order>        m_b;
};

#endif

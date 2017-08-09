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

#ifndef POLYFIT_HPP_
#  define POLYFIT_HPP_

#  include "Matrix.tpp"

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

  inline uint32_t size() const
  {
    return order;
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
    //assert(std::isfinite(dataX));
    //assert(std::isfinite(dataY));

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
    for (uint32_t i = 0U; i < order; ++i)
    {
      // The following code suffers of accurancy:
      //   b->element[i] = b->element[i] + X[i] * DataY;
      // The follow one, less:
      m_b[i] = (it * m_b[i] + X[i] * dataY) / (it + 1.0);
    }

    ++m_iteration;
  }

  //! \brief Compute the polynom fitting datum.
  Vector<double, order> end()
  {
    return matrix::LUsolve(m_Xsquared, m_b);
  }

  //! \brief Apply the polynom on a given data
  template <typename T>
  double apply(Polynom const& P, Vector<T, order> const& X) const
  {
    assert(X.size() + 1U >= P.degree());

    double res = P[0];
    for (uint32_t i = 1U; i < P.degree(); ++i)
      {
        res += (P[i] * X[i - 1U]);
      }
    return res;
  }

protected:

  uint32_t                     m_iteration;
  Matrix<double, order, order> m_Xsquared;
  Vector<double, order>        m_b;
};

#endif

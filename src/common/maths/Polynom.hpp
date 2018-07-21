//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef POLYNOM_HPP_
#  define POLYNOM_HPP_

#  include <vector>
#  include <iostream>

// *************************************************************************************************
//
// *************************************************************************************************
class Polynom
{
public:

  //! \brief Constructor with possibility to reserve memory for
  //! storing coefs.
  Polynom(const size_t nth = 0)
  {
    size_t i = nth;

    m_coefs.reserve(nth);
    while (i--)
      {
        m_coefs.push_back(0.0);
      }
  }

  //! \brief Return the container size of the polynom.
  inline uint32_t size() const
  {
    return m_coefs.size();
  }

  //! \brief Return the degree of the polynom.
  inline uint32_t degree() const
  {
    uint32_t deg = m_coefs.size();

    while ((0.0 != m_coefs[deg]) && deg)
      --deg;

    return deg;
  }

  //! \brief Change the value of the nth coef. If nth is greater than
  //! the current degree, intermediate coef are set to 0.  is greater
  //! than the current degree.
  inline double& operator[](size_t nth)
  {
    const uint32_t s = m_coefs.size(); // FIXME capacity
    if (nth >= s)
      {
        // Insert zeros
        uint32_t i = nth - s + 1U;
        m_coefs.reserve(i);
        while (i--)
          {
            m_coefs.push_back(0.0);
          }
      }
    return m_coefs[nth];
  }

  //! \brief Read the nth coef. No security is trying to access to an
  //! coef with higher degree than the degree of the polynom.
  inline const double& operator[](size_t nth) const
  {
    return m_coefs[nth];
  }

protected:

  //! \brief Store coef values.
  std::vector<double> m_coefs;
};


//! \brief Display the matrix.
inline std::ostream& operator<<(std::ostream& os, Polynom const& p)
{
  os << "Degree: " << p.degree() << std::endl;
  for (size_t i = 0; i < p.size(); ++i)
    {
       std::cout << "  P[" << i << "]: " << p[i] << std::endl;
    }
  return os;
}

#endif

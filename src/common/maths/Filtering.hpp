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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FILTERING_HPP_
#  define FILTERING_HPP_

#  include <vector>
#  include <iostream>
#  include <cassert>

// *************************************************************************************************
//! \brief Abstract class for all kind of numerical filters.
// *************************************************************************************************
class IFilter
{
public:

  virtual ~IFilter() { };

  //! \brief Reset filter states to initial values.
  virtual void reset() = 0;

  //! \brief Filter the raw data u and return the filtered value.
  virtual float process(const float u) = 0;
};

// *************************************************************************************************
//! \brief 1st order low pass filter.
//!
//! This is the transfert function of the filter
//!               K
//!   Y(S) =  ---------- U(S)
//!            1 + S/Wc
//!
//! Where:
//! U is the input, K a constant (here foreced to K = 1), Wc the cutoff
//! pulsation, S the Laplace transform (continuous time) and Y the
//! output of the transfert function.
//!
//! We obtain the discret formula (using the LaTeX formalism):
//!   y_{n+1} = (1 - h Wc) y_{n} + K h Wc u_{n}
//!
//! where: Wc = 2 PI Fc,
//! and Fc is the desired cutoff frequency (in Hz).
//! and h is the discretisation step (0 < h < 1)
//! and PI the constant 3.1415.
//!
//! As proof, let write the following transfert function of the
//! derivated of X:
//!   Y = X . S
//! In discrete time, a derivative is written as follow:
//!   Y_{n+1} = (x_{n+1} - x_{n}) / h
// *************************************************************************************************
class LowPassFilter1stOrder: public IFilter
{
public:

  // Empty constructor.
  LowPassFilter1stOrder()
  {
  }

  //! \brief Initialize filter states.
  //! \param fc the cutoff frequency in Hertz.
  //! \param h the time step (discretisation step) is the time between two
  //! filtering cycles given in second and shall be 0 < h < 1.
  //! \param init_value init the 1st filtered value to this value
  //! (default is 0) for faster convergence of the filtered value.
  inline void configure(const float fc, const float h, const float init_value = 0.0f)
  {
    assert((0.0f < h) && (h < 1.0f));
    m_hWc = h * 2.0f * PI * fc;
    m_init_value = init_value;
    reset();
  }

  //! \brief reset filter states.
  inline virtual void reset() override
  {
    m_memory[0] = m_init_value;
  }

  //! \brief Filter the raw data u and return the filtered value.
  inline virtual float process(const float u) override
  {
    float res = m_memory[0];
    std::cout << m_hWc << " " << m_memory[0]  << "  " << u << std::endl;
    m_memory[0] = (1.0f - m_hWc) * m_memory[0] + K * m_hWc * u;
    return res;
  }

protected:

  //! \brief PI number (double to float)
  const float PI = 3.14159265358979323846;
  //! \brief Gain of the filter.
  const float K = 1.0f;
  //! \brief Product of the time step with cutoff pulsation.
  float m_hWc = 1.0f;
  //! \brief Initial filtered value.
  float m_init_value = 0.0f;
  //! \brief Memory of the filter (filter order).
  float m_memory[1] = { 0.0f };
};

// *************************************************************************************************
//! \brief This filter takes a window of raw data and computes the average value on the last X raw
//! value. The window is sliding along raw values.
// *************************************************************************************************
class RollingAverageFilter: public IFilter
{
public:

  // Empty constructor.
  RollingAverageFilter()
  {
  }

  //! \brief Constructor. Take the the number of values for the window
  //! and the number of averaged points. Given values x are then
  //! converted into 2^x.
  inline void configure(const uint8_t window_size, const uint8_t average_points)
  {
    // Final values is a power of two. Usefull for optim the modulo computation.
    m_window_size = (1U << window_size);
    m_average_points = (1 << average_points) - 1U;

    if (m_window_size < m_average_points)
      {
        std::cerr << "ERROR" << std::endl;
        m_window_size = m_average_points;
      }
    reset();
  }

  //! \brief reset filter states.
  inline void reset() override
  {
    m_i = 0;
    m_filtered_value[Current] = 0.0f;
    m_filtered_value[Previous] = 0.0f;
    // Erase elements and reset the reserved memory
    std::vector<float>().swap(m_memory);
    m_memory.reserve(m_window_size);
  }

  //! \brief Filter the raw data u and return the filtered value.
  float process(const float u) override
  {
    if (buffering())
      {
        // y(i) = mean(x(1:i));
        float i = (float) m_i;
        m_filtered_value[Current] = (m_filtered_value[Previous] * i + u) / (i + 1.0f);
      }
    else
      {
        // y(i) = y(i-1) + (x(i) - x(i-Nb_pt_filt)) / Nb_pt_filt;
        m_filtered_value[Current] = m_filtered_value[Previous] + (u - m_previous_u)
          / (((float) m_average_points) + 1.0f);
      }
    m_filtered_value[Previous] = m_filtered_value[Current];
    m_memory[m_i] = u;
    m_i = (m_i + 1) & m_average_points; // optim: modulo with a powered of two value
    m_previous_u = m_memory[m_i];

    return m_filtered_value[Current];
  }

protected:

  //! \brief
  enum Slots { Current, Previous };

  //! \brief Return the boolean indicating if the window is buffering raw values.
  inline bool buffering() const
  {
    return m_i < m_average_points;
  }

  uint32_t           m_i;
  uint32_t           m_window_size;
  uint32_t           m_average_points;
  float              m_filtered_value[2];
  float              m_previous_u;
  std::vector<float> m_memory;
};

#endif

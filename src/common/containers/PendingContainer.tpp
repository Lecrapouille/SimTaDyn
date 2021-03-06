// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef PENDING_CONTAINER_HPP
#  define PENDING_CONTAINER_HPP

#  include "PendingData.hpp"
#  include "GLMemory.hpp"
#  include <vector>
#  include <algorithm>
#  include <cmath>
#  include <fstream>

// **************************************************************
//! \brief PendingContainer is a std::valarray memorizing elements
// which have been changed and has to updated.
// **************************************************************
template<class T>
class PendingContainer: public PendingData
{
public:

  PendingContainer()
    : PendingData()
  {}

  //! \note: we call .reserve(count) and not .resize(count) or pass
  //! count to the std::vector constructor (which called resize)
  //! because we do not want, for VBOs, to transfer invalid dummy data
  //! to the GPU.
  explicit PendingContainer(size_t const count)
  {
    m_container.reserve(count);
    GPUMemory() += memory();
    LOGD("Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
         count, sizeof (T), GPUMemory().load());
  }

  explicit PendingContainer(size_t const count, T const& val)
    : PendingData(count),
      m_container(count, val)
  {
    GPUMemory() += memory();
    LOGD("Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
         count, sizeof (T), GPUMemory().load());
  }

  explicit PendingContainer(PendingContainer<T> const& other)
    : PendingData()
  {
    // Copy container as well as the capacity
    m_container.reserve(other.capacity());
    m_container = other.m_container;

    // Set pending data
    const auto bound = other.getPendingData();
    tagAsPending(bound.first, bound.second);

    GPUMemory() += memory();
    LOGD("Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
         other.size(), sizeof (T), GPUMemory().load());
  }

  explicit PendingContainer(std::vector<T> const& other)
    : PendingData(other.size()),
      m_container(other)
  {
    GPUMemory() += memory();
    LOGD("Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
         other.size(), sizeof (T), GPUMemory().load());
  }

  explicit PendingContainer(std::initializer_list<T> il)
    : PendingData(il.size()),
      m_container(il)
  {
    GPUMemory() += memory();
    LOGD("Reserve %zu elements of %zu bytes. GPU memory: %zu bytes",
         il.size(), sizeof (T), GPUMemory().load());
  }

  ~PendingContainer()
  {
    GPUMemory() -= memory();
    LOGD("Removing %zu bytes. GPU memory: %zu bytes",
         memory(), GPUMemory().load());
  }

  inline size_t capacity() const
  {
    return m_container.capacity();
  }

  inline size_t size() const
  {
    return m_container.size();
  }

  inline size_t memory() const
  {
    LOGD("Internal GPU memory: %zu bytes", sizeof (T) * m_container.size());
    return sizeof (T) * m_container.size();
  }

  inline void reserve(size_t const count)
  {
    throw_if_cannot_expand();
    m_container.reserve(count);
  }

  inline void resize(size_t const count)
  {
    size_t old_count = size();

    // Same size: ignore!
    if (unlikely(count == old_count))
      return ;

    // Resize the container
    throw_if_cannot_expand();
    m_container.resize(count);

    // Case of container is reduced: update pending data
    if (unlikely(count < old_count))
      {
        if (0_z == count)
          {
            clearPending();
          }
        else
          {
            size_t pos_start;
            size_t pos_end;
            getPendingData(pos_start, pos_end);
            clearPending();
            tagAsPending(std::min(pos_start, count), std::min(pos_end, count));
          }
      }

    GPUMemory() -= memory();
    GPUMemory() += count * sizeof (T);
    LOGD("Resizing %zu elements of size %zu bytes. GPU memory: %zu bytes",
         count, sizeof (T), GPUMemory().load());
  }

  inline T& set(size_t const nth)
  {
    if (unlikely(nth > m_container.capacity()))
      {
        resize(nth);
      }
    tagAsPending(nth);
    return m_container.operator[](nth);
  }

  inline T const& get(size_t const nth) const
  {
    return m_container.at(nth);
  }

  void clear()
  {
    throw_if_cannot_expand();
    GPUMemory() -= memory();
    LOGD("Clearing %zu elements of size %zu bytes. GPU memory: %zu bytes",
         size(), sizeof (T), GPUMemory().load());
    m_container.clear();
    clearPending(0_z);
  }

  PendingContainer<T>&
  append(std::initializer_list<T> il)
  {
    throw_if_cannot_expand();
    size_t start = m_container.size();
    m_container.insert(m_container.end(), il);
    tagAsPending(start, m_container.size());
    GPUMemory() += il.size() * sizeof (T);
    LOGD("Appending %zu elements of size %zu bytes. GPU memory: %zu bytes",
         il.size(), sizeof (T), GPUMemory().load());
    return *this;
  }

  PendingContainer<T>&
  append(const T* other, size_t const size)
  {
    throw_if_cannot_expand();
    size_t start = m_container.size();
    m_container.insert(m_container.end(),
                       other,
                       other + size);
    tagAsPending(start, m_container.size());
    GPUMemory() += size * sizeof (T);
    LOGD("Appending %zu elements of size %zu bytes. GPU memory: %zu bytes",
         size, sizeof (T), GPUMemory().load());
    return *this;
  }

  PendingContainer<T>&
  append(std::vector<T> const& other)
  {
    throw_if_cannot_expand();
    size_t start = m_container.size();
    m_container.insert(m_container.end(),
                       other.begin(),
                       other.end());
    tagAsPending(start, m_container.size());
    GPUMemory() += other.size() * sizeof (T);
    LOGD("Appending %zu elements of size %zu bytes. GPU memory: %zu bytes",
         other.size(), sizeof (T), GPUMemory().load());
    return *this;
  }

  PendingContainer<T>&
  append(PendingContainer const& other)
  {
    return PendingContainer<T>::append(other.m_container);
  }

  PendingContainer<T>&
  append(T const& val)
  {
    throw_if_cannot_expand();

    m_container.push_back(val);
    tagAsPending(0_z, m_container.size());
    GPUMemory() += sizeof (T);
    LOGD("Appending 1 element of size %zu bytes. GPU memory: %zu bytes",
         sizeof (T), GPUMemory().load());
    return *this;
  }

  //! \brief Append EBO
  PendingContainer<T>&
  appendIndex(std::vector<T> const& other)
  {
    throw_if_cannot_expand();

    T older_index{0};
    if (likely(0_z != size()))
      {
        older_index = max();
        older_index += T(1);
      }

    LOGD("appendIndex => older_index %zu", static_cast<size_t>(older_index));

    m_container.reserve(other.size());
    for (auto it: other)
      {
        m_container.push_back(it + older_index);
      }
    tagAsPending(m_container.size());
    GPUMemory() += other.size() * sizeof (T);
    LOGD("AppendingIndex %zu elements of size %zu bytes. GPU memory: %zu bytes",
         other.size(), sizeof (T), GPUMemory().load());
    return *this;
  }

  PendingContainer<T>&
  appendIndex(PendingContainer const& other)
  {
    return PendingContainer<T>::appendIndex(other.m_container);
  }

  inline T sum() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        throw std::out_of_range("Cannot compute the summation of an empty container");
      }

    T sum_of_elems = 0;
    for (auto& n : m_container)
      {
        sum_of_elems += n;
      }

    return sum_of_elems;
  }

  inline T prod() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        throw std::out_of_range("Cannot compute the product of an empty container");
      }

    T prod_of_elems = 1;

    for (auto& n : m_container) {
      prod_of_elems *= n;
    }

    return prod_of_elems;
  }

  inline T min() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        throw std::out_of_range("Cannot compute the min of an empty container");
      }
    return *std::min_element(m_container.begin(), m_container.end());
  }

  inline T max() const
  {
    if (unlikely(0_z == m_container.size()))
      {
        throw std::out_of_range("Cannot compute the max of an empty container");
      }
    return *std::max_element(m_container.begin(), m_container.end());
  }

  template<class Function>
  inline PendingContainer<T>& apply(Function f)
  {
    clearPending(m_container.size());
    std::for_each(m_container.begin(), m_container.end(), f);
    return *this;
  }

  inline PendingContainer<T>& abs()
  {
    return apply([](T& x){ x = std::abs(x); });
  }

  inline PendingContainer<T>& sqrt()
  {
    return apply([](T& x){ x = std::sqrt(x); });
  }

  inline PendingContainer<T>& squared()
  {
    return apply([](T& x){ x = x * x; });
  }

  inline PendingContainer<T>& sin()
  {
    return apply([](T& x){ x = std::sin(x); });
  }

  inline PendingContainer<T>& cos()
  {
    return apply([](T& x){ x = std::cos(x); });
  }

  inline PendingContainer<T>& operator=(PendingContainer<T> const& other)
  {
    return this->operator=(other.m_container);
  }

  template<class U>
  PendingContainer<T>& operator=(std::vector<U> const& other)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = other.size();

    if (other_size > my_size) {
      throw_if_cannot_expand();
    }

    GPUMemory() -= memory();
    GPUMemory() += other.size() * sizeof (T);
    LOGD("Affecting %zu elements of size %zu bytes. GPU memory: %zu bytes",
         other.size(), sizeof (T), GPUMemory().load());

    m_container = other;
    tagAsPending(0_z, other_size);
    return *this;
  }

  template<class U>
  PendingContainer<T>& operator=(std::initializer_list<U> il)
  {
    const size_t my_size = m_container.size();
    const size_t other_size = il.size();

    if (other_size > my_size) {
      throw_if_cannot_expand();
    }

    GPUMemory() -= memory();
    GPUMemory() += il.size() * sizeof (T);
    LOGD("Affecting %zu elements of size %zu bytes. GPU memory: %zu bytes",
         il.size(), sizeof (T), GPUMemory().load());

    m_container = il;
    tagAsPending(0_z, other_size);
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator*=(U const& val)
  {
    //FIXME return apply([val](T& x){ x *= val; });
    for (auto& x: m_container) {
      x *= val;
    }
    tagAsPending(0_z, m_container.size());
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator+=(U const& val)
  {
    //FIXME return apply([val](T& x){ x += val; });
    for (auto& x: m_container) {
      x += val;
    }
    tagAsPending(0_z, m_container.size());
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator-=(U const& val)
  {
    //FIXME return apply([val](T& x){ x -= val; });
    for (auto& x: m_container) {
      x -= val;
    }
    tagAsPending(0_z, m_container.size());
    return *this;
  }

  template<class U>
  inline PendingContainer<T>& operator/=(U const& val)
  {
    return PendingContainer<T>::operator*=(U(1) / val);
  }

  friend std::ostream& operator<<(std::ostream& stream, const PendingContainer& cont)
  {
    stream << "PendingContainer: ";
    if (0_z != cont.size())
      {
        stream << cont.m_container[0];
        for (size_t i = 1_z; i < cont.size(); ++i) {
          stream << ", " << cont.m_container[i];
        }
      }
    return stream;
  };

  inline T* to_array()
  {
    if (likely(0_z != m_container.size()))
      {
        return m_container.data();
      }
    else
      {
        return nullptr;
      }
  }

  inline const T* to_array() const
  {
    if (likely(0_z != m_container.size()))
      {
        return m_container.data();
      }
    else
      {
        return nullptr;
      }
  }

  inline std::vector<T>& data()
  {
    return m_container;
  }

protected:

  inline void throw_if_cannot_expand()
  {
    if (likely(!m_can_expand)) {
      throw std::out_of_range("Cannot change buffer size once loaded on GPU");
    }
  }

  inline void set_cannot_expand()
  {
    m_can_expand = false;
  }

  std::vector<T> m_container;
  bool m_can_expand = true;
};

#endif // PENDING_CONTAINER_HPP

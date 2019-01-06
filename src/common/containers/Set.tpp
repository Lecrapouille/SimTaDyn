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

#ifndef SET_HPP_
#  define SET_HPP_

#  include "IContainer.tpp"

#  define INITIAL_INDEX  static_cast<size_t>(-1)

// **************************************************************
//! \brief A set is similar to the std::vector and it's push_back()
//! method but the difference is that this containers does not allow
//! holes and when it's full and user add a new element, instead of
//! creating a new vector with the double size of the previous and
//! copying the older into the newer, the Set only alloc a new memory
//! block and index it. No copy on containers is needed with a
//! temporary 3x memory used (old and newer vector). Finally, acceding
//! on elements has a complexity of O(2). This container is used for
//! storing set of datum where position between them in the container
//! is not important. A case is to transfer datum to the GPU of for
//! SGBD.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
class Set: public IContainer<T, N, Block>
{
protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief Constructor: allocate the given number of elements of
  //! type T.
  Set(const size_t reserve_elements = 1_z)
    : IContainer<T, N, Block>(reserve_elements)
  {
    m_index = m_subindex = m_last = INITIAL_INDEX;
  }

  //! \brief Constructor
  Set(std::initializer_list<T> initList)
    : IContainer<T, N, Block>(initList.size())
  {
    m_index = m_subindex = m_last = INITIAL_INDEX;

    const size_t m = size_t(initList.size());
    auto iter = initList.begin();
    for (size_t i = 0_z; i < m; ++i)
      {
        append(*iter);
        ++iter;
      }
  }

  //! \brief Insert an element at first empty location. Complexity
  //! is O(1) of elements iteration.
  virtual void append(T const& elt);

  //! \brief Append elements from a given array.
  //! \param array an not nullptr address of the array.
  //! \param size the number of elements of the array.
  virtual void append(const T *array, const size_t size)
  {
    for (size_t i = 0_z; i < size; ++i)
      {
        append(array[i]);
      }
  }

  //! \brief Append elements from a given vector.
  //! \param vect
  virtual void append(std::vector<T> &vect)
  {
    const size_t size = vect.size();
    for (size_t i = 0_z; i < size; ++i)
      {
        append(vect[i]);
      }
  }

  //! \brief Check if the given index is incorrect (outside the
  //! definition range of the container). Complexity is O(1) in
  //! number of elements.
  //! \return false if nth is before is inside, else return true.
  virtual inline bool outofbound(const size_t nth) const override
  {
    return (nth > m_last) || (INITIAL_INDEX == m_last);
  }

  //! \brief Remove the n'th element. Complexity is O(1) of elements
  //! iteration. In fact, to preserve both continuities between
  //! elements (no holes) this algorithm performs a swap between the
  //! n'th element by the last inserted before removing the last
  //! element. Note: no deallocation is made (this will be done by the
  //! destructor). Note: Nothing is made if nth is incorrect or if the
  //! element has already been removed.
  virtual void remove(const size_t nth);

  //! \brief Remove the last inserted element.
  virtual inline void remove()
  {
    if (IContainer<T, N, Block>::empty())
      return ;
    removeLast();
  }

  //! \brief Swap two elements. Note: a temporary copy is made. Note:
  //! Nothing is made if nth is incorrect or if the element has
  //! already been removed. Note: Nothing is made if indexes are
  //! incorrect or indexes have the same value.
  //! \return true is case of sucess, else return false if one given
  //! index is incorrect.
  bool swap(const size_t index1, const size_t index2);

  //! \brief Return the current position of the index.
  inline size_t index() const
  {
    if (INITIAL_INDEX == m_last)
      return 0_z;
    return m_last;
  }

  //! \brief Empty all the container. Complexity is O(n)
  //! where n is number of allocated blocks. Note: blocks
  //! are not deleted from memory.
  inline virtual void clear() override
  {
    IContainer<T, N, Block>::clear();
    m_index = m_subindex = m_last = INITIAL_INDEX;
  }

  //! Include iterators
#include "SetIterator.ipp"

  //! \brief Create an iterator on the begining of the container.
  //! Note: begin() refers on the first element of the container.
  inline iterator begin() const
  {
    return iterator(*this, false);
  }

  //! \brief Create an iterator on the end of the container.
  //! Note: end() does not refer on the last element of the container
  //! but the element after it.
  inline iterator end() const
  {
    return iterator(*this, true);
  }

private:

  //! \brief Remove the last inserted element.
  void removeLast();

protected:

  //! \brief Iter on allocated blocks.
  size_t              m_index;
  //! \brief Iter on elements of the block.
  size_t              m_subindex;
  //! \brief Refer the last inserted element.
  size_t              m_last;
};

#  include "Set.ipp"
#  undef INITIAL_INDEX

#endif /* SET_HPP_ */

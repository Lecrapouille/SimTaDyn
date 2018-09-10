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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef CONTAINER_HPP_
#  define CONTAINER_HPP_

#  include "PendingData.hpp"
#  include <iterator>
#  include <iostream>
#  include <vector>
#  include <cassert>

// **************************************************************
//! \brief
// **************************************************************
typedef size_t ContainerBitField;

// **************************************************************
//! \brief this class defines a block of M elements of type T. M
//! shall be a powered of two number and E is the length of the
//! bitfield indicating which elements are stored in the block.
//! By block we mean a contigious memory of elements liek an array.
// **************************************************************
template<typename T, const size_t N>
class Block: public PendingData
{
protected:

#  include "ContainerEnums.ipp"

  //! \brief Typedef
  typedef Block<T, N> block_t;

public:

  //! \brief Default constructor to create an empty block.
  Block(const bool lazy_allocation)
    : PendingData()
  {
    if (lazy_allocation)
      {
        m_block = nullptr;
      }
    else
      {
        m_block = new T[M];
      }
    clear(); // FIXME: this call twice clearPending();
  }

  //! \brief Virtual destructor. Do nothing. Elements are
  //! not deleted from the memory because it's the container
  //! job to do it.
  virtual ~Block()
  {
    if (nullptr != m_block)
      {
        delete[] m_block;
      }
  }

  //! \brief clear the bitfield to have an empty block.
  virtual void clear()
  {
    ContainerBitField i = E;

    while (i--)
      {
        m_occupied[i] = 0_z;
      }
    clearPending();
  }

  //! \brief Return the number of elements a block can store
  inline size_t size() const
  {
    return M;
  }

  //! \brief Return the number of elements currently stored
  size_t occupation() const // FIXME renommer en used()
  {
    ContainerBitField i;
    ContainerBitField total = 0_z;
    const ContainerBitField full = (1_z << B) - 1_z;

    for (i = 0; i < E; ++i)
      {
        if (full == m_occupied[i])
          {
            total += B;
          }
        else
          {
            for (; i < E; ++i)
              {
                total += hammingWeight(m_occupied[i]);
              }
          }
      }
    return total;
  }

  //! \brief Access to the nth row in write mode.
  inline T& nth(size_t i)
  {
    assert(i < M);
    if (nullptr == m_block)
      {
        m_block = new T[M];
      }
    return m_block[i];
  }

  //! \brief Acces to the nth row in read mode.
  inline const T& nth(size_t i) const
  {
    assert(i < M);
    if (nullptr == m_block)
      {
        m_block = new T[M];
      }
    return m_block[i];
  }

private:

  //! \brief Count the number of bit '1' in a number.
  //! \param val the number to count set bits.
  //! \return the number of set bits.
  ContainerBitField hammingWeight(const ContainerBitField val) const
  {
    ContainerBitField bitCount = 0_z;
    ContainerBitField value = val;

    while (value > 0_z)
      {
        if (1_z == (value & 1_z))
          ++bitCount;
        value >>= 1_z;
      }

    return bitCount;
}

public:

  //! Lazy allocation of block of elements
  T                 *m_block;
  //! Indicate which elements are occupied.
  ContainerBitField m_occupied[E];
};

// **************************************************************
//! \brief An abstract class of indexed of Block classes. This class
//! can be seen has a dynamic array but with miminising allocations
//! and deallocations overhead and minimising defragementation of the
//! memory. T is the type of element we want to store. 2^N is the
//! number of elements by block.
//!
//! Principe: inserting elements is similar to inserting in an
//! array. If the container is too small an other array is created and
//! arrays are managed by an index. Accessing to the index is made in
//! constant time. Removing element just clear a flag and do not
//! delete the block, so it's made in constant time. Empty blocks
//! can be all garbage in once by a specific command.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
class IContainer
{
private:
  typedef Block<T, N> block_t;

protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief Constructor: allocate the given number of elements of
  //! type T.
  IContainer(const size_t reserve_elements = 1);

  //! \brief Destructor. Release all created blocks.
  ~IContainer()
  {
    for (auto block : m_blocks)
      {
        delete block;
      }
    m_blocks.clear();
  }

  //! \brief Allocate the given number of elements of type T.
  void reserve(const size_t reserve_elements);

  //! \brief Check if the given index is incorrect (outside the
  //! definition range of the container). Complexity is O(1) in
  //! number of elements.
  virtual bool outofbound(const size_t nth) const = 0;

  //! \brief Check if the given index is occupied by an element.
  //! Complexity is O(1) in number of elements.
  bool occupied(const size_t nth) const;

  //! \brief Force the given index to be not empty.
  //! Complexity is O(1) in number of elements.
  virtual void occupy(const size_t nth);

  //! \brief get the n'th element of the container. Complexity
  //! is O(1) in number of elements.
  virtual T& get(const size_t nth);

  //! \brief get the n'th element of the container. Complexity
  //! is O(1) in number of elements.
  virtual T const& get(const size_t nth) const;

  //! \brief Update the n'th element of the container. Complexity
  //! is O(1) in number of elements.
  virtual void modify(const size_t nth, T const& e) const;

  //! \brief Return the number of elements currently stored.
  inline size_t used() const
  {
    return m_stored_elements;
  }

  //! \brief Return the number of allocated blocks
  inline size_t blocks() const
  {
    return m_allocated_blocks;
  }

  //! \brief Return the nth block
  inline block_t *block(const size_t nth) const
  {
    if (nth < m_allocated_blocks)
      return m_blocks[nth];
    return nullptr;
  }

  //! \brief Return the number of elements currently empty.
  inline size_t remaining() const
  {
    return (m_allocated_blocks << N) - m_stored_elements;
  }

  //! \brief Check if the container is empty.
  inline bool empty() const
  {
    return 0_z == m_stored_elements;
  }

  //! \brief Check if the container is full.
  inline bool full() const
  {
    return (m_allocated_blocks << N) == m_stored_elements;
  }

  //! \brief Empty all the container. Complexity is O(n)
  //! where n is number of allocated blocks. Note: blocks
  //! are not deleted from memory.
  virtual inline void clear()
  {
    size_t i = m_allocated_blocks;
    while (i--)
      {
        m_blocks[i]->clear();
      }
    m_stored_elements = 0_z;
  }

  //! \brief Get the nth element of the container but
  //! without protections (out of bound index, accessing
  //! to an empty element. If safe guards are needed call
  //! the get() method instead.
  inline T& operator[](size_t nth);

  //! \brief Get the nth element of the container but
  //! without protections (out of bound index, accessing
  //! to an empty element. If safe guards are needed call
  //! the get() method instead.
  inline const T& operator[](size_t nth) const;

  //! \brief Empty blocks will have their memory deleted.
  //! Call this routine is memory ressources are limited.
  virtual void garbage();

  //! \brief Call this method for debugging the content of
  //! the container.
  void debug() const;

protected:

  //! \brief Allocate a new block. Use virtual to allow inheritance
  //! of the class Block.
  inline virtual block_t *newBlock(const bool lazy) const
  {
    return new block_t(lazy);
  }

  //! \brief Reserve memory corresponding to the given number of
  //! blocks of typed-T elements.
  void allocateBlocks(const size_t reserve_blocks, const bool lazy);

  //! \brief List of blocks of templated elements.
  std::vector<block_t*> m_blocks;
  //! \brief Number of elements T currently stored.
  size_t              m_stored_elements;
  //! \brief m_blocks size
  size_t              m_allocated_blocks;
};

#  include "IContainer.ipp"

#endif /* CONTAINER_HPP_ */

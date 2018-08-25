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

// Perform a % b but works if and only if M is a power of two number.
#define MODULO(a, b)             ((a) & ((b) - 1ul))
// Accessor to the bitfield indicationg which elements are stored.
#define OCCUPIED(id, sid)        (IContainer<T,N,Block>::m_blocks[id]->m_occupied[sid / S])
// Perform a generic operation on the bitfield.
#define OP_OCCUPIED(op, id, sid) (OCCUPIED(id, sid) op (1ul << (MODULO(sid, S))))
// Set the 'occupied' flag in the bitfield.
#define SET_OCCUPIED(id, sid)    (OP_OCCUPIED(|=, id, sid))
// Set the 'empty' flag in the bitfield.
#define CLEAR_OCCUPIED(id, sid)  (OP_OCCUPIED(&= ~, id, sid))
// Check if the element is stored.
#define IS_OCCUPIED(id, sid)     (OP_OCCUPIED(&, id, sid))

// **************************************************************
//! \param nb_elts is the number of elements of type T we want to
//! pre-allocate. This value is automaticly rounded up to have at
//! least one block of elements created. Note: 0 is a possible value
//! in this case no blocks are allocated. Default value is 1.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
IContainer<T,N,Block>::IContainer(const size_t nb_elts)
{
  m_stored_elements = 0;
  m_allocated_blocks = 0;
  reserve(nb_elts);
}

// **************************************************************
//! \param nb_elts is the number of elements of type T we want to
//! pre-allocate. This value is automaticly rounded up to have at
//! least one block of elements created.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void IContainer<T,N,Block>::reserve(const size_t nb_elts)
{
  std::cout << "Reserving (" << static_cast<unsigned>(nb_elts)
            << " + " << static_cast<unsigned>(M) << " - 1) >> "
            << static_cast<unsigned>(N) << " blocks\n";
  reserveBlocks((nb_elts + M - 1) >> N);
}

// **************************************************************
//! \param nb_blocks is the number of blocks of elements of type T
//! we want to pre-allocate.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void IContainer<T,N,Block>::reserveBlocks(const size_t nb_blocks)
{
  std::cout << "Reserving " << nb_blocks << " blocks\n";
  assert(nb_blocks < 16ul);
  m_blocks.reserve(nb_blocks);

  size_t i = nb_blocks;
  while (i--)
    {
      m_blocks.push_back(newBlock());
    }
  m_allocated_blocks += nb_blocks;
  assert(m_allocated_blocks == m_blocks.size());
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to get.
//! \return the reference of the element.
//! \throw std::out_of_range if n'th is an invalid index (outside
//! the position of the last insereted element) or if the element
//! has been removed.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
T& IContainer<T,N,Block>::get(const size_t nth)
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  return m_blocks[id]->m_block[sid];
}

// **************************************************************
// FIXME code in double with previous code
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
T const& IContainer<T,N,Block>::get(const size_t nth) const
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  return m_blocks[id]->m_block[sid];
}

// **************************************************************
//
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void IContainer<T,N,Block>::modify(const size_t nth, T const& e) const
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  m_blocks[id]->m_block[sid] = e;
  m_blocks[id]->addPendingData(sid);
}

// **************************************************************
//
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
const T& IContainer<T,N,Block>::operator[](size_t nth) const
{
  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  return m_blocks[id]->m_block[sid];
}

// **************************************************************
//
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
T& IContainer<T,N,Block>::operator[](size_t nth)
{
  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  return m_blocks[id]->m_block[sid];
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to check.
//! \return true if an element is present, else return false.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
bool IContainer<T,N,Block>::occupied(const size_t nth) const
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  return !(!(IS_OCCUPIED(id, sid)));
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to be not empty.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void IContainer<T,N,Block>::occupy(const size_t nth)
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      SET_OCCUPIED(id, sid);
      ++m_stored_elements;
    }
}

// **************************************************************
//!
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void IContainer<T,N,Block>::garbage()
{
  // Important: start by the end
  size_t index = m_allocated_blocks;
  while (index--)
    {
      ContainerBitField subindex = M;
      while (subindex--)
        {
          // Stop algorithm at first non empty block
          if (IS_OCCUPIED(index, subindex))
            return ;
        }

      // Empty block
      delete m_blocks[index];
      --m_allocated_blocks;
      m_blocks.pop_back();
      assert(m_allocated_blocks == m_blocks.size());
    }
}

// **************************************************************
//!
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void IContainer<T,N,Block>::debug() const
{
  if (empty())
    {
      std::cout << "The container is empty" << std::endl;
      return ;
    }
  for (size_t index = 0ul; index < m_allocated_blocks; ++index)
    {
      std::cout << "Block " << index << ": " << std::endl;

      // Show elements stored
      for (size_t subindex = 0ul; subindex < M; ++subindex)
        {
          if (IS_OCCUPIED(index, subindex))
            {
              std::cout << " " << m_blocks[index]->m_block[subindex]
                        << std::endl;
            }
          else
            {
              std::cout << '<' << subindex << ": empty> ";
            }
        }
      std::cout << std::endl;
    }
}

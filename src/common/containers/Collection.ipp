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

// **************************************************************
//! \param elt is the element of type T to insert.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void Collection<T,N,Block>::insert(const size_t nth, T const& elt)
{
  constexpr bool lazy_allocation = false;
  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  // Reserve a new block of elements if all current blocks are
  // occupied.
  if (id >= IContainer<T,N,Block>::m_allocated_blocks)
    {
      IContainer<T,N,Block>::allocateBlocks(1_z + id -
         IContainer<T,N,Block>::m_allocated_blocks,
         lazy_allocation);
    }

  // Insert element and add the 'Occupied' flag
  IContainer<T,N,Block>::m_blocks[id]->nth(sid) = elt;
  if (!IS_OCCUPIED(id, sid))
    {
      SET_OCCUPIED(id, sid);
      ++IContainer<T,N,Block>::m_stored_elements;

      // Update min and max bounding box used for iterators
      if (nth >= m_end)
        m_end = nth + 1;
      if (nth < m_begin)
        m_begin = nth;
    }
}

// **************************************************************
//! \param elt is the element of type T to insert.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void Collection<T,N,Block>::insert(T const& elt)
{
  insert(m_end, elt);
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to be not empty.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void Collection<T,N,Block>::occupy(const size_t nth)
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
      ++IContainer<T,N,Block>::m_stored_elements;

      // Update min and max bounding box used for iterators
      if (nth >= m_end)
        m_end = nth + 1_z;
      if (nth < m_begin)
        m_begin = nth;
    }
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to remove.
// **************************************************************
template<typename T, const size_t N,
         template<typename X, const size_t Y> class Block>
void Collection<T,N,Block>::remove(const size_t nth)
{
  if (outofbound(nth))
    return ;

  const size_t id = nth >> N;
  const size_t sid = MODULO(nth, M);

  // Remove the 'Occupied' flag
  if (IS_OCCUPIED(id, sid))
    {
      CLEAR_OCCUPIED(id, sid);
      --IContainer<T,N,Block>::m_stored_elements;

      if (nth == m_begin)
        {
          do {
            ++m_begin;
          } while ((m_begin < m_end) && (!IContainer<T,N,Block>::occupied(m_begin)));
        }
      else if (nth + 1_z == m_end)
        {
          while (m_end && (!IContainer<T,N,Block>::occupied(m_end - 1_z)))
            {
              --m_end;
            }
        }
      if (m_end == m_begin)
        {
          m_begin = INITIAL_INDEX;
          m_end = 0;
        }
      std::cout << "after removing ==> Min: " << m_begin << "  Max: " << m_end << "\n";
    }
}

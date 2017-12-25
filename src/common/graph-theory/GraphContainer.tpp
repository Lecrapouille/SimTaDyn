// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef GRAPHCONTAINER_TPP_
#  define GRAPHCONTAINER_TPP_

#  include "Collection.tpp"

namespace graphtheory
{

#define MODULO(a, b)             ((a) & ((b) - 1U))

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N>
class GraphBlock: public Block<T, N>
{
protected:

#  include "ContainerEnums.ipp"

public:

  GraphBlock()
    : Block<T, N>()
  {
  }

  void clearMarks()
  {
    ContainerBitField i = S;

    while (i--)
      {
        m_marked[i] = 0;
      }
  }

  virtual void clear() override
  {
    Block<T, N>::clear();
    clearMarks();
  }

  ContainerBitField m_marked[S];
};


// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
class GraphContainer: public Collection<T, N, Block>
{
protected:

#  include "ContainerEnums.ipp"

public:

  GraphContainer(const uint32_t reserve_elements = 1)
    : Collection<T, N, Block>(reserve_elements)
  {
  }

  //! \brief Check if the element have been marked
  bool marked(const uint32_t nth) const
  {
    const uint32_t index = nth / M;
    const uint32_t subindex = MODULO(nth, M);

    return 0 != (Collection<T, N, Block>::m_blocks[index]->m_marked[subindex / S] & (1 << (MODULO(subindex, S))));
  }

  //! \brief Mark an element as marked
  void mark(const uint32_t nth)
  {
    if (Collection<T, N, Block>::outofbound(nth))
      return ;

    const uint32_t index = nth / M;
    const uint32_t subindex = MODULO(nth, M);

    Collection<T, N, Block>::m_blocks[index]->m_marked[subindex / S] |= (1 << (MODULO(subindex, S)));
  }

  void unmark(const uint32_t nth)
  {
    if (Collection<T, N, Block>::outofbound(nth))
      return ;

    const uint32_t index = nth / M;
    const uint32_t subindex = MODULO(nth, M);

    if ((index < Collection<T, N, Block>::blocks()) && (subindex < M))
      {
        Collection<T, N, Block>::m_blocks[index]->m_marked[subindex / S] &= ~(1 << (MODULO(subindex, S)));
      }
  }

  //!
  void unmarkAll()
  {
    uint32_t index = Collection<T, N, Block>::blocks();
    while (index--)
      {
        Collection<T, N, Block>::m_blocks[index]->clearMarks();
      }
  }
};

} // namespace graphtheory

#endif /* GRAPHCONTAINER_TPP_ */

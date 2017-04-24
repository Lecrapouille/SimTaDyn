// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef GRAPHCONTAINER_TPP_
#  define GRAPHCONTAINER_TPP_

#  include "Collection.tpp"

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

  void clear() override
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

#endif /* GRAPHCONTAINER_TPP_ */

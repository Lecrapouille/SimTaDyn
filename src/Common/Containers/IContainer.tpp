// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef CONTAINER_HPP_
#  define CONTAINER_HPP_

#  include <iterator>
#  include <iostream>
#  include <vector>

// **************************************************************
//! \brief Idee au lieu de M=8 faire M=2^3
// **************************************************************
typedef uint32_t ContainerBitField;

// **************************************************************
// FIXME heritage pour ajouter des algo: visited, marked, etc ..
// **************************************************************
template<typename T, const uint32_t M, const uint32_t S>
class Block
{
public:

  Block() { clear(); }

  virtual void clear()
  {
    ContainerBitField i = S;

    while (i--)
      {
        m_occupied[i] = 0;
      }
  }

  // FIXME: fonction destroy all elements iif pointers. Attention une case libre ne veut pas dire non vide

  //! Pre-allocated block
  T                  m_block[M];
  //! Indicate which elements are occupied.
  ContainerBitField m_occupied[S];
};

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t M>
class IContainer
{
protected:

  enum { S = sizeof (ContainerBitField) };
  typedef Block<T, M, M / S> block_t;

public:

  //! \brief Constructor and reserve memory corresponding to the
  //! given number of elements of type T.
  IContainer(const uint32_t reserve_elements = 1);

  inline virtual block_t *newBlock()
  {
    return new block_t;
  }

  //! \brief Reserve memory corresponding to the given number of
  //! elements of Type T.
  void reserve(const uint32_t reserve_elements);

  //! \brief Check if the given index is outisde the index of th last
  //! inserted element.
  //! \return false if nth is before is inside, else return true.
  virtual bool outofbound(const uint32_t nth) const = 0;

  //! \brief Check if the n'th slot is occupied or not
  bool occupied(const uint32_t nth);

  //! \brief Set the n'th element as removed. Complexity is O(1) of
  //! elements iteration. Note: no deallocation is made (this will be
  //! done by the destructor). Note: Nothing is made if nth is
  //! incorrect or if the element has already been removed.
  //void empty(const uint32_t nth);

  //! \brief get the n'th element of the container.
  virtual T& get(const uint32_t nth);
  virtual T const& get(const uint32_t nth) const;

  //! \brief Return the number of elements currently stored.
  inline uint32_t used() const
  {
    return m_stored_elements;
  }

  //! \brief Return the number of elements currently empty.
  inline uint32_t remaining() const
  {
    return M * m_allocated_blocks - m_stored_elements;
  }

  //! \brief Check if the container is empty.
  inline bool empty() const
  {
    return 0 == m_stored_elements;
  }

  //! \brief Check if the container is full.
  inline bool full() const
  {
    return (M * m_allocated_blocks) == m_stored_elements;
  }

  //! \brief
  inline uint32_t blocks() const
  {
    return m_allocated_blocks;
  }

  // FIXME pending_data() et clear_pending_data()

  // FIXME: rework m_vectors for removing empty blocks

  void debug() const;

protected:

  //! \brief Reserve memory corresponding to the given number of
  //! blocks of typed-T elements.
  void reserveBlocks(const uint32_t reserve_blocks);

  //! \brief List of blocks of templated elements.
  std::vector<block_t*> m_blocks;
  //! \brief Number of elements T currently stored.
  uint32_t              m_stored_elements;
  //! \brief m_blocks size
  uint32_t              m_allocated_blocks;
};

#  include "IContainer.ipp"

#endif /* CONTAINER_HPP_ */

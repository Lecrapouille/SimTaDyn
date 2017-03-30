// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef CONTAINER_HPP_
#  define CONTAINER_HPP_

#  include <iterator>
#  include <iostream>
#  include <vector>
#  include <cassert>

// **************************************************************
//! \brief
// **************************************************************
typedef uint32_t ContainerBitField;

// **************************************************************
//! \brief this class defines a block of M elements of type T. M
//! shall be a powered of two number and E is the length of the
//! bitfield indicating which elements are stored in the block.
//! By block we mean a contigious memory of elements liek an array.
// **************************************************************
template<typename T, const uint32_t M, const uint32_t E>
class Block
{
public:

  //! \brief Default constructor to create an empty block.
  Block()
  {
    clear();
  }

  //! \brief Virtual destructor. Do nothing. Elements are
  //! not deleted from the memory because it's the container
  //! job to do it.
  virtual ~Block()
  {
  }

  //! \brief clear the bitfield to have an empty block.
  virtual void clear()
  {
    ContainerBitField i = E;

    while (i--)
      {
        m_occupied[i] = 0;
      }
  }

public:

  //! Pre-allocated block
  T                 m_block[M];
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
template<typename T, const uint32_t N>
class IContainer
{
protected:

  //! \brief Number of elements by block: 2^N
  enum { M = (1U << N) };
  //! \brief Number of bytes by integer: 4 for uint32_t, 8 for uint64_t
  enum { B = sizeof (ContainerBitField) };
  //! \brief Number of bits by integer: 32 bits for uint32_t
  enum { S = B * 8U };
  //! \brief Precompute round(M / S)
  enum { E = (M + S - 1U) / S };
  //! \brief Typedef
  typedef Block<T, M, E> block_t;

public:

  //! \brief Constructor: allocate the given number of elements of
  //! type T.
  IContainer(const uint32_t reserve_elements = 1);

  //! \brief Allocate a new block. Use virtual to allow inheritance
  //! of the class Block.
  inline virtual block_t *newBlock() const
  {
    return new block_t;
  }

  //! \brief Allocate the given number of elements of type T.
  void reserve(const uint32_t reserve_elements);

  //! \brief Check if the given index is incorrect (outside the
  //! definition range of the container). Complexity is O(1) in
  //! number of elements.
  virtual bool outofbound(const uint32_t nth) const = 0;

  //! \brief Check if the given index is occupied by an element.
  //! Complexity is O(1) in number of elements.
  bool occupied(const uint32_t nth) const;

  //! \brief get the n'th element of the container. Complexity
  //! is O(1) in number of elements.
  virtual T& get(const uint32_t nth);

  //! \brief get the n'th element of the container. Complexity
  //! is O(1) in number of elements.
  virtual T const& get(const uint32_t nth) const;

  //! \brief Return the number of elements currently stored.
  inline uint32_t used() const
  {
    return m_stored_elements;
  }

  //! \brief Return the number of allocated blocks
  inline uint32_t blocks() const
  {
    return m_allocated_blocks;
  }

  //! \brief Return the number of elements currently empty.
  inline uint32_t remaining() const
  {
    return (m_allocated_blocks << N) - m_stored_elements;
  }

  //! \brief Check if the container is empty.
  inline bool empty() const
  {
    return 0 == m_stored_elements;
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
    uint32_t i = m_allocated_blocks;
    while (i--)
      {
        m_blocks[i]->clear();
      }
    m_stored_elements = 0;
  }

  //! \brief Empty blocks will have their memory deleted.
  //! Call this routine is memory ressources are limited.
  void garbage();

  //! \brief Call this method for debugging the content of
  //! the container.
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

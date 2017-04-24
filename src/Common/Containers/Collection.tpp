// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef COLLECTION_HPP_
#  define COLLECTION_HPP_

#  include "IContainer.tpp"

#  define INITIAL_INDEX ((uint32_t) -1)

// **************************************************************
//! \brief A set is similar to the class Set but holes in elements
//! are allowed. This class can be used for storing huge number of
//! elements like a graph without getting too much overhead due to
//! memory allocation and minimising defragementation of the memory.
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
class Collection: public IContainer<T, N, Block>
{
protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief Constructor and reserve memory corresponding to the
  //! given number of elements of type T.
  //! \param nb_elts is the number of elements of type T we want to
  //! pre-allocate. This value is automaticly rounded up to have at
  //! least one block of elements created. Note: 0 is a possible value
  //! in this case no blocks are allocated. Default value is 1.
  Collection(const uint32_t reserve_elements = 1)
    : IContainer<T, N, Block>(reserve_elements)
  {
    m_begin = INITIAL_INDEX;
    m_end = 0;
  }

  //! \brief Insert an element at the given index. Complexity
  //! is O(1) of elements iteration.
  void insert(const uint32_t index, T const& elt);
  void insert(T const& elt);

  //! \brief Remove the n'th element. Complexity is O(1) of elements
  //! iteration while updating the internal state of the class
  //! iterator can be O(n). In fact, to preserve both continuities
  //! between elements (no holes) this algorithm performs a swap
  //! between the n'th element by the last inserted before removing
  //! the last element. Note: no deallocation is made (this will be
  //! done by the destructor). Note: Nothing is made if nth is
  //! incorrect or if the element has already been removed.
  void remove(const uint32_t nth);

  //! \brief Check if the given index is outisde the container bound.
  //! \return false if nth is before is inside, else return true.
  virtual inline bool outofbound(const uint32_t nth) const override
  {
    return (nth + 1) > (IContainer<T, N, Block>::m_allocated_blocks << N);
  }

  //! \brief Empty all the container. Complexity is O(n)
  //! where n is number of allocated blocks. Note: blocks
  //! are not deleted from memory.
  virtual inline void clear()
  {
    IContainer<T, N, Block>::clear();
    m_begin = INITIAL_INDEX;
    m_end = 0;
  }

  //! \brief Force the given index to be not empty.
  //! Complexity is O(1) in number of elements.
  virtual void occupy(const uint32_t nth) override;

  inline uint32_t last() { return m_end - 1U; }

  //! Include iterators
#include "CollectionIterator.ipp"

  //! \brief Create an iterator on the begining of the container.
  //! Note: begin() refers on the first element of the container.
  inline iterator begin()
  {
    return iterator(*this, false);
  }

  //! \brief Create an iterator on the end of the container.
  //! Note: end() does not refer on the last element of the container
  //! but the element after it.
  inline iterator end()
  {
    return iterator(*this, true);
  }

protected:

  //! \brief iterator.
  uint32_t m_begin;
  //! \brief iterator.
  uint32_t m_end;
};

#  include "Collection.ipp"
#  undef INITIAL_INDEX

#endif /* COLLECTION_HPP_ */

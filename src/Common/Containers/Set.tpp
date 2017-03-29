// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef SET_HPP_
#  define SET_HPP_

#  include "IContainer.tpp"

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
template<typename T, const uint32_t N>
class Set: public IContainer<T, N>
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

public:

  //! \brief Constructor: allocate the given number of elements of
  //! type T.
  Set(const uint32_t reserve_elements = 1)
    : IContainer<T, N>(reserve_elements)
  {
    m_index = m_subindex = m_last = (uint32_t) -1;
  }

  //! \brief Insert an element at first empty location. Complexity
  //! is O(1) of elements iteration.
  void append(T const& elt);

  //! \brief Check if the given index is incorrect (outside the
  //! definition range of the container). Complexity is O(1) in
  //! number of elements.
  //! \return false if nth is before is inside, else return true.
  virtual inline bool outofbound(const uint32_t nth) const override
  {
    return (nth > m_last) || (((uint32_t) -1) == m_last);
  }

  //! \brief Remove the n'th element. Complexity is O(1) of elements
  //! iteration. In fact, to preserve both continuities between
  //! elements (no holes) this algorithm performs a swap between the
  //! n'th element by the last inserted before removing the last
  //! element. Note: no deallocation is made (this will be done by the
  //! destructor). Note: Nothing is made if nth is incorrect or if the
  //! element has already been removed.
  void remove(const uint32_t nth);

  //! \brief Remove the last inserted element.
  inline void remove()
  {
    if (IContainer<T,N>::empty())
      return ;
    removeLast();
  }

  //! \brief Swap two elements. Note: a temporary copy is made. Note:
  //! Nothing is made if nth is incorrect or if the element has
  //! already been removed. Note: Nothing is made if indexes are
  //! incorrect or indexes have the same value.
  void swap(const uint32_t index1, const uint32_t index2);

  //! \brief Return the current position of the index.
  inline uint32_t index() const
  {
    return m_last;
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

  // TODO: bool pending_data(start, end)

protected:

  //! \brief Remove the last inserted element.
  void removeLast();

protected:

  //! \brief Iter on allocated blocks.
  uint32_t              m_index;
  //! \brief Iter on elements of the block.
  uint32_t              m_subindex;
  //! \brief Refer the last inserted element.
  uint32_t              m_last;
};

#  include "Set.ipp"

#endif /* SET_HPP_ */

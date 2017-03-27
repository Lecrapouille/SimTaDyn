// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef SET_HPP_
#  define SET_HPP_

#  include "IContainer.tpp"

// **************************************************************
//
// **************************************************************
template<typename T, const uint32_t M>
class Set: public IContainer<T, M>
{
protected:

  enum { S = sizeof (ContainerBitField) };
  typedef Block<T, M, M / S> block_t;

public:

  //! \brief Constructor and reserve memory corresponding to the
  //! given number of elements of type T.
  Set(const uint32_t reserve_elements = 1)
    : IContainer<T, M>(reserve_elements)
  {
    m_index = m_subindex = m_last = (uint32_t) -1;
  }

  //! \brief Insert an element at first empty location. Complexity
  //! is O(1) of elements iteration.
  void append(T const& elt);

  //! \brief Check if the given index is outisde the index of th last
  //! inserted element.
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
    if (IContainer<T,M>::empty())
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

#include "SetIterator.ipp"

  inline iterator begin() const
  {
    return iterator(*this, false);
  }

  inline iterator end() const
  {
    return iterator(*this, true);
  }

  // FIXME: bool pending_data(start, end)

protected:

  //! \brief Remove the last inserted element
  void removeLast();

protected:

  uint32_t              m_index;
  uint32_t              m_subindex;
  uint32_t              m_last;
};

#  include "Set.ipp"

/*#undef MODULO
#undef OCCUPIED
#undef OP_OCCUPIED
#undef SET_OCCUPIED
#undef CLEAR_OCCUPIED
#undef IS_OCCUPIED*/

#endif /* SET_HPP_ */

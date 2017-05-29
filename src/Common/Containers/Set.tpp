// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef SET_HPP_
#  define SET_HPP_

#  include "IContainer.tpp"

#  define INITIAL_INDEX ((uint32_t) -1)

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
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
class Set: public IContainer<T, N, Block>
{
protected:

#  include "ContainerEnums.ipp"

public:

  //! \brief Constructor: allocate the given number of elements of
  //! type T.
  Set(const uint32_t reserve_elements = 1)
    : IContainer<T, N, Block>(reserve_elements)
  {
    m_index = m_subindex = m_last = INITIAL_INDEX;
  }

  //! \brief Insert an element at first empty location. Complexity
  //! is O(1) of elements iteration.
  virtual void append(T const& elt);

  //! \brief Append elements from a given array.
  //! \param array an not nullptr address of the array.
  //! \param size the number of elements of the array.
  virtual void append(const T *array, const uint32_t size)
  {
    for (uint32_t i = 0; i < size; ++i)
      {
        append(array[i]);
      }
  }

  //! \brief Append elements from a given vector.
  //! \param vect
  virtual void append(std::vector<T> &vect)
  {
    const uint32_t size = vect.size();
    for (uint32_t i = 0; i < size; ++i)
      {
        append(vect[i]);
      }
  }

  //! \brief Check if the given index is incorrect (outside the
  //! definition range of the container). Complexity is O(1) in
  //! number of elements.
  //! \return false if nth is before is inside, else return true.
  virtual inline bool outofbound(const uint32_t nth) const override
  {
    return (nth > m_last) || (INITIAL_INDEX == m_last);
  }

  //! \brief Remove the n'th element. Complexity is O(1) of elements
  //! iteration. In fact, to preserve both continuities between
  //! elements (no holes) this algorithm performs a swap between the
  //! n'th element by the last inserted before removing the last
  //! element. Note: no deallocation is made (this will be done by the
  //! destructor). Note: Nothing is made if nth is incorrect or if the
  //! element has already been removed.
  virtual void remove(const uint32_t nth);

  //! \brief Remove the last inserted element.
  virtual inline void remove()
  {
    if (IContainer<T, N, Block>::empty())
      return ;
    removeLast();
  }

  //! \brief Swap two elements. Note: a temporary copy is made. Note:
  //! Nothing is made if nth is incorrect or if the element has
  //! already been removed. Note: Nothing is made if indexes are
  //! incorrect or indexes have the same value.
  //! \return true is case of sucess, else return false if one given
  //! index is incorrect.
  bool swap(const uint32_t index1, const uint32_t index2);

  //! \brief Return the current position of the index.
  inline uint32_t index() const
  {
    if (INITIAL_INDEX == m_last)
      return 0;
    return m_last;
  }

  //! \brief Empty all the container. Complexity is O(n)
  //! where n is number of allocated blocks. Note: blocks
  //! are not deleted from memory.
  inline void clear()
  {
    IContainer<T, N, Block>::clear();
    m_index = m_subindex = m_last = INITIAL_INDEX;
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

private:

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
#  undef INITIAL_INDEX

#endif /* SET_HPP_ */

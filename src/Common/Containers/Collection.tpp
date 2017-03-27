// -*- c++ -*- Coloration Syntaxique pour Emacs

#ifndef COLLECTION_HPP_
#  define COLLECTION_HPP_

#  include "IContainer.tpp"

template<typename T, const uint32_t M>
class Collection: public IContainer<T, M>
{
protected:

  enum { S = sizeof (ContainerBitField) };
  typedef Block<T, M, M / S> block_t;

public:

  //! \brief Constructor and reserve memory corresponding to the
  //! given number of elements of type T.
  //! \param nb_elts is the number of elements of type T we want to
  //! pre-allocate. This value is automaticly rounded up to have at
  //! least one block of elements created. Note: 0 is a possible value
  //! in this case no blocks are allocated. Default value is 1.
  Collection(const uint32_t reserve_elements = 1)
    : IContainer<T, M>(reserve_elements)
  {
  }

  //! \brief Insert an element at the given index. Complexity
  //! is O(1) of elements iteration.
  void insert(const uint32_t index, T const& elt);

  //! \brief Remove the n'th element. Complexity is O(1) of elements
  //! iteration. In fact, to preserve both continuities between
  //! elements (no holes) this algorithm performs a swap between the
  //! n'th element by the last inserted before removing the last
  //! element. Note: no deallocation is made (this will be done by the
  //! destructor). Note: Nothing is made if nth is incorrect or if the
  //! element has already been removed.
  void remove(const uint32_t nth);

  //! \brief Check if the given index is outisde the container bound.
  //! \return false if nth is before is inside, else return true.
  virtual inline bool outofbound(const uint32_t nth) const override
  {
    return nth + 1 > M * IContainer<T, M>::m_allocated_blocks;
  }
};

#  include "Collection.ipp"

/*#undef MODULO
#undef OCCUPIED
#undef OP_OCCUPIED
#undef SET_OCCUPIED
#undef CLEAR_OCCUPIED
#undef IS_OCCUPIED*/

#endif /* COLLECTION_HPP_ */

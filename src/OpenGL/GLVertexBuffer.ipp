// -*- c++ -*- Coloration Syntaxique pour Emacs

template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void GLBuffer<T,N,Block>::append(T const& elt)
{
  Set<T,N,Block>::append(elt);
  addPendingData(Set<T,N,Block>::m_last, Set<T,N,Block>::m_last);
}

template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void GLBuffer<T,N,Block>::remove()
{
  uint32_t last = Set<T,N,Block>::index();
  Set<T,N,Block>::remove();
  addPendingData(Set<T,N,Block>::m_last, last);
}

template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void GLBuffer<T,N,Block>::remove(const uint32_t nth)
{
  Set<T,N,Block>::remove(nth);
  addPendingData(nth, Set<T,N,Block>::m_last);
}

template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void GLBuffer<T,N,Block>::swap(const uint32_t index1, const uint32_t index2)
{
  Set<T,N,Block>::swap(index1, index2);
  addPendingData(index1, index2);
}

template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void GLBuffer<T,N,Block>::clear()
{
  Set<T,N,Block>::clear;
  addPendingData(index1, index2);
}

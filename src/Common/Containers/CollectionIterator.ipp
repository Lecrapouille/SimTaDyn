// -*- c++ -*- Coloration Syntaxique pour Emacs

// **************************************************************
//! \class iterator allows to iterate on elements contained by \c
//! container. Implements methods for compatibility with
//! std::iterator.
// **************************************************************
class iterator
{
public:

  //! \brief For compatibility with std::iterator.
  typedef T value_type;
  //! \brief For compatibility with std::iterator.
  typedef T& reference;
  //! \brief For compatibility with std::iterator.
  typedef T* pointer;
  //! \brief For compatibility with std::iterator.
  typedef std::forward_iterator_tag iterator_category;

  //! \brief Empty constructor.
  iterator() :
    m_container(nullptr)
  {
  }

  //! \brief Constructor by copy from an iterator.
  //! \param it the iterator to copy.
  iterator(iterator const& it)
    : m_container(it.m_container), m_itr(it.m_itr)
  {
  }

  //! \brief Constructor by copy from a container.
  //! \param container the container to copy.
  //! \param start_at_end set it to true for starting at the end.
  iterator(Collection<T, N, Block>& container, const bool start_at_end)
    : m_container(&container)
  {
    if (container.empty() || start_at_end)
      {
        m_itr = container.m_end;
      }
    else
      {
        m_itr = container.m_begin;
      }
  }

  //! \brief Iterate on the next occupied element (empty slots are ignored).
  inline iterator operator++(int)
  {
    iterator copy(*this);
    operator++();
    return copy;
  }

  //! \brief Iterate on the next occupied element (empty slots are ignored).
  iterator& operator++()
  {
    do {
      ++m_itr;
    } while ((m_itr < m_container->m_end) && (!m_container->occupied(m_itr)));
    return *this;
  }

  //! \brief Iterate on the previous occupied element (empty slots are ignored).
  inline iterator operator--(int)
  {
    iterator copy(*this);
    operator--();
    return copy;
  }

  //! \brief Iterate on the previous occupied element (empty slots are ignored).
  iterator& operator--()
  {
    do {
      --m_itr;
    } while (m_itr && (!IContainer<T,N,Block>::occupied(m_itr)));
    return *this;
  }

  //! \brief Access to the content of the slot
  T const& operator*() const
  {
    return m_container->get(m_itr);
  }

  //! \brief Access to the content of the slot
  inline T const* operator->() const
  {
    return m_container->get(m_itr);
  }

  //! \brief Access to the content of the slot
  T& operator*()
  {
    return m_container->get(m_itr);
  }

  //! \brief Access to the content of the slot
  inline T* operator->()
  {
    return m_container->get(m_itr);
  }

  //! \brief compare iterators.
  inline bool operator==(const iterator& rhs) const
  {
    return (m_container == rhs.m_container) && (m_itr == rhs.m_itr);
  }

  //! \brief compare iterators.
  inline bool operator!=(const iterator& rhs) const
  {
    return !(*this == rhs);
  }

protected:

  //! \brief the address of the container to explore.
  Collection<T, N, Block>* m_container;

  //! \brief the iterator.
  uint32_t m_itr;
};

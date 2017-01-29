#ifndef GRAPHMEMORY_HPP_
#  define GRAPHMEMORY_HPP_

#  include <iostream>
#  include <vector>
#  include <cassert>

// Work if and only if M is a power of two number
#define MODULO(a, b) (a & (b - 1U))

//! \brief This templated file implements a class for allocating pool
//! of memory specialized for storing graph elements (nodes, links)
//! where their following particularities is to be stored in order
//! given by their ID in the aim to have a constant complexity access
//! and avoid search with complexity O(n) or O(log(n)) with uses more
//! time or memory ressources. In addition, we try to avoid reallocing
//! containers: when the container is full, a new one bigger is
//! created in another portion of the memory, old container is then
//! copied to the new one and finally old location released. This is
//! ressource disaster.

//namespace SimTaDyn
//{

  //! \class container is a container for graph. It's a set of
  //! allocated but fixed-size arrays (renamed here as pool of memory)
  //! maintained by an index (an array). So inserting, deleting and
  //! searching are done in constant time. When ressources are full
  //! Only allocating an new pool of memory is necessary (old values
  //! do not need to be copyied). When removing an element, freeing
  //! pool of memory is not necessary a bool flag indicating if the
  //! memory is considered as empty.
  template<class T> class container
  {
  public:
    //! \class iterator allows to iterate on elements contained by \c
    //! container. Implements methods for compatibility with
    //! std::iterator.
    class iterator : public std::iterator<std::input_iterator_tag, T*>
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
        m_container(nullptr), m_itr(0), m_begin(0), m_end(0)
      {
      }
      //! \brief Constructor by copy from an iterator.
      //! \param it the iterator to copy.
      iterator(iterator const& it)
        : m_container(it.m_container), m_itr(it.m_itr),
          m_begin(it.m_begin), m_end(it.m_end)
      {
      }
      //! \brief Constructor by copy from a container.
      //! \param container the container to copy.
      //! \param start_at_end set it to true for starting at the end.
      iterator(container<T> const& container, const bool start_at_end)
        : m_container(&container)
      {
        if (container.empty())
          {
            m_itr = m_begin = m_end = 0;
          }
        else if (start_at_end)
          {
            m_itr = container.emptyPoolE();
            skipDec();
            ++m_itr;
            m_end = m_itr;
          }
        else
          {
            m_itr = container.emptyPoolB();
            skipInc();
            m_begin = m_itr;
          }
      }
      //! \brief
      inline uint32_t debug() const
      {
        return m_itr;
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
        check_assertion();
        if (!m_container->empty())
          {
            skipInc();
          }
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
        check_assertion();
        if (!m_container->empty())
          {
            skipDec();
          }
        return *this;
      }
      //! \brief Access to the content of the slot
      T const& operator*() const
      {
        check_assertion();
        return m_container->get(m_itr / M, MODULO(m_itr, M));
        //T* res = m_container->get(m_itr);
        //assert(nullptr != res && "Invalid iterator dereference");
        //return *res;
      }
      //! \brief Access to the content of the slot
      inline T const* operator->() const
      {
        return m_container->get(m_itr / M, MODULO(m_itr, M));
      }
      //! \brief compare iterators.
      inline bool operator==(const iterator& rhs) const
      {
        check_assertion();
        return (m_container == rhs.m_container) && (m_itr == rhs.m_itr);
      }
      //! \brief compare iterators.
      inline bool operator!=(const iterator& rhs) const
      {
        return !(*this == rhs);
      }

    protected:
      //! \brief
      inline void check_assertion() const
      {
        // TODO: exception instead of assert
        assert(nullptr != m_container && "Uninitialized iterator");
      }
      //! Skip non-occupied elements or halt if reached the end of the container.
      void skipInc()
      {
        do
          {
            ++m_itr;
          }
        while ((m_itr < m_end) && (false == m_container->isOccupied(m_itr)));
      }
      //! Skip non-occupied elements or halt if reached the end of the container.
      void skipDec()
      {
        do
          {
            --m_itr;
          }
        while ((m_itr >= m_begin) && (false == m_container->isOccupied(m_itr)));
      }
      //! \brief the address of the container to explore.
      const container<T>* m_container;
      //! \brief the iterator.
      uint32_t m_itr, m_begin, m_end;
    };

  public:
    //! \brief Constructor. Reserve space for the specified number of elements.
    container(const uint32_t no_elements = 1)
    {
      m_index = m_allocated = m_occupation = 0;
      reserveNbElements(no_elements);
    }
    //! \brief Destructor. Release all allocated pool of memory.
    ~container()
    {
      uint32_t i = m_allocated;

      while (i--)
        {
          delete m_pools[i];
        }
    }
    //! \brief Allocate additional pool of memory.
    //! \param no_pools the additional number of pool of memory to allocated.
    void reservePools(const uint32_t no_pools)
    {
      uint32_t i = no_pools;

      while (i--)
        {
          m_pools.push_back(new poolContainer<T>);
        }
      m_allocated += no_pools;
    }
    inline void reserveNbElements(const uint32_t no_elements)
    {
      reservePools((no_elements + M - 1) / M);
    }

    //! \brief Store an element at a given index. No check is made to
    //! verify if the memory is already used. Complexity is O(1).
    //! Several pool allocations can be called if index is greater
    //! than the current limit of the container.
    //! \param elt the element to store.
    //! \param index location of the slot to store (see like an array).
    void insert(const uint32_t id, T const& elt)
    {
      const uint32_t index = id / M;
      const uint32_t subindex = MODULO(id, M);

      // Ressources are full, create at least one new pool memory
      if (index >= m_allocated)
        {
          reservePools(1U + index - m_allocated);
        }
      insert(index, subindex, elt);
    }
    //! \brief Same than insert but at the current slot. Complexity is
    //! O(1). No check is made to verify if the memory is already
    //! used.
    //! \param elt the element to store.
    inline void append(T const& elt)
    {
      insert(m_index, elt);
      ++m_index;
    }
    //! \brief Remove an element at a given position. Complexity is
    //! O(1). Ignore the case where index > m_allocated. No
    //! desallocation/free is called.
    //! \param index the location of the slot to set as free (see like
    //! an array).
    void remove(const uint32_t id)
    {
      const uint32_t index = id / M;
      const uint32_t subindex = MODULO(id, M);

      if (index <= m_allocated)
        {
          remove(index, subindex);
        }
    }
    //! \brief Remove the last inserted element. Complexity is O(1).
    void remove()
    {
      if (m_index > 0)
        {
          --m_index;
        }
      remove(m_index);
    }
    //! \brief Check if the slot is occupied.
    //! \param id the location of the slot.
    //! \return true if the slot is occupied. Return false if empty.
    bool isOccupied(const uint32_t id) const
    {
      const uint32_t index = id / M;
      const uint32_t subindex = MODULO(id, M);

      if ((index < m_allocated) && (subindex < M))
        {
          return isOccupied(index, subindex);
        }
      return true;
    }
    //! \brief Check if the location of the slot belongs to the container.
    //! \param id the location of the slot.
    //! \return true if the location belongs to the container, else return false.
    inline bool isValidIndex(const uint32_t id) const
    {
      return isValidIndex(id / M, MODULO(id, M));
    }
    //! \brief Return the element stored at the given index. Complexity is O(1).
    //! \param id the location of the slot.
    //! \return the content of the slot or throw an out_of_range exception if
    //! the location is erroneous or the slot empty.
    T& get(const uint32_t id) const
    {
      return get(id / M, MODULO(id, M));
    }
    /*const T& get(const uint32_t id) const
    {
      return get(id / M, MODULO(id, M));
      }*/
    //! \brief Return an iterator of the first element of the container.
    inline iterator begin() const
    {
      return iterator(*this, false);
    }
    //! \brief Return an iterator on the next element of the last
    //! element of the container. Used for loop.
    inline iterator end() const
    {
      return iterator(*this, true);
    }
    //! \brief Return an iterator of the last element of the
    //! container. Contrary to end() where the content is undefined
    //! and is only used to stop a loop, back() will return an
    //! iterator on the last element.
    inline iterator back() const
    {
      iterator it(*this, true);
      --it.m_itr;
      return it;
    }
    //! \brief Return the number of elements stored.
    inline uint32_t occupation() const
    {
      return m_occupation;
    }
    //! \brief
    inline bool empty() const
    {
      return m_occupation == 0;
    }
    inline bool full() const
    {
      return m_occupation == (M * m_allocated);
    }

    //! \brief Set all slots as empty. No memory desalocation is called.
    void clear()
    {
      uint32_t i = m_allocated;
      while (i--)
        {
          Index j = I; while (j--) { m_pools[i]->m_occupied[j] = 0; }
        }
      m_index = m_occupation = 0;
    }
    //! \brief Pretty print the memory.
    void debug() const
    {
      for (uint32_t pool = 0; pool < m_allocated; ++pool)
        {
          std::cout << "Pool " << pool << ": " << std::endl;
          poolContainer<T> *e = m_pools[pool];

          // Show elements stored
          for (uint32_t i = 0; i < M; ++i)
            {
              if (isOccupied(pool, i))
                {
                  std::cout << " " << e->m_slots[i];
                }
              else
                {
                  std::cout << " <empty>";
                }
            }

          // Show occupation
          //std::cout << std::endl;
          //for (uint32_t i = 0; i < M; ++i)
          //  {
          //    std::cout << " " << isOccupied(pool, i);
          //  }
          std::cout << std::endl;
        }
    }

  private:
    //! \brief
    inline bool isValidIndex(const uint32_t index, const uint32_t subindex) const
    {
      return (index < m_allocated) && (subindex < M);
    }
    //! \brief
    inline bool isOccupied(const uint32_t index, const uint32_t subindex) const
    {
      return 0 != (m_pools[index]->m_occupied[subindex / S] &
                   (1 << (MODULO(subindex, S))));
    }
    //! \brief
    // No check on bounds
    uint32_t emptyPoolB() const // FIXME Chnager nom
    {
      const uint32_t size = m_pools.size();
      uint32_t index = 0;
      while (index < size)
        {
          for (uint32_t subindex = 0; subindex < S; ++subindex)
            {
              if (0 != m_pools[index]->m_occupied[subindex])
                {
                  return M * index + subindex - 1U;
                }
            }
          ++index;
        }
      return ((uint32_t) - 1U);
    }
    //!
    // No check on bounds
    uint32_t emptyPoolE() const // FIXME Chnager nom
    {
      uint32_t index = m_pools.size();
      while (index--)
        {
          uint32_t subindex = S;
          while (subindex--)
            {
              if (0 != m_pools[index]->m_occupied[subindex])
                {
                  return M * index + subindex + 1U;
                }
            }
        }
      return 1U;
    }
    // FIXME overloader operator[] pour acceder au slot sans passer par les securites de get()
    /*inline const T& get(const uint32_t index, const uint32_t subindex) const
    {
      return get_(index, subindex);
      }*/
    inline T& get(const uint32_t index, const uint32_t subindex) const
    {
      return get_(index, subindex);
    }
    T& get_(const uint32_t index, const uint32_t subindex) const
    {
      if (isValidIndex(index, subindex) && isOccupied(index, subindex))
        {
          return m_pools[index]->m_slots[subindex];
        }
      // Launch an exception
      std::string str;
      if (isValidIndex(index, subindex))
        {
          str = "Invalid index";
        }
      else
        {
          str = "Removed element";
        }
      throw std::out_of_range(str + " (" + std::to_string(index) +
                              ", " + std::to_string(subindex) + ')');
    }
    //!
    inline void insert(const uint32_t index,
                       const uint32_t subindex,
                       T const& elt)
    {
      // Insert the new element
      m_pools[index]->m_slots[subindex] = elt;

      if (!isOccupied(index, subindex))
        {
          ++m_occupation;

          // Flag it as 'occupied'
          m_pools[index]->m_occupied[subindex / S]
            |= (1 << (MODULO(subindex, S)));
        }
    }
    //!
    inline void remove(const uint32_t index,
                       const uint32_t subindex)
    {
      if (isOccupied(index, subindex))
        {
          assert(m_occupation > 0);
          --m_occupation;

          // Flag it as 'occupied'
          m_pools[index]->m_occupied[subindex / S]
            &= ~(1 << (MODULO(subindex, S)));
        }
    }
    //! Can be uint64_t. Index is used as 32-bits-field.
    typedef uint32_t Index;
    enum _S { S = sizeof (Index) };
    //! Number of storage slots. Must be a power of two for optimize
    //! the computation of modulo.
    enum _M { M = 4U };
    //! Size of the index indicating which slots are occupied.
    enum _I { I = M / S };
    //!
    inline uint32_t poolSizeAllocation() const
    {
      return M;
    }
    inline uint32_t allocated() const
    {
      return m_allocated;
    }
    //! Private class for storing elements in a large array.
    template<class U> class poolContainer
    {
    public:
      poolContainer()
      {
        // Set all slots are not occupied
        Index i = I; while (i--) { m_occupied[i] = 0; }
      }
      //! Pool of elements: the array is never re-alloced.
      U m_slots[M];
      //! Bit field grouped (into 32-bits) indicating which elements are
      //! occupied (true) or removed (false).
      Index m_occupied[I];
    };

    //! Index of pool memories which can be re-allocated because it
    //! will contain relatively few elements (only addresses on
    //! allocated pool).
    std::vector<poolContainer<T>*> m_pools;
    //! \brief Cursor on the first not occupied element.
    uint32_t m_index;
    uint32_t m_occupation;
    //! \brief Number of memory pool allocated.
    uint32_t m_allocated;
  };
//}

#endif /* GRAPHMEMORY_HPP_ */

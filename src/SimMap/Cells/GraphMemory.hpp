#ifndef GRAPHMEMORY_HPP_
#  define GRAPHMEMORY_HPP_

#  include <iostream>
#  include <vector>
#  include <cassert>

// Work if and only if M is a power of two number
#define MODULO(a, b) (a & (b - 1U))

//! \brief This file implements a class for allocating pool of memory
//! specialized for storing graph elements (nodes, links) where their
//! following particularities is to be stored in order given by their
//! ID in the aim to have a constant complexity access and avoid
//! search with complexity O(n) or O(log(n)) with uses more time or
//! memory ressources. In addition, we try to avoid reallocing
//! containers: when the container is full, a new one bigger is
//! created in another portion of the memory, old container is then
//! copied to the new one and finally old location released. This is
//! ressource disaster.

namespace SimTaDyn
{

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

    //! \class iterator allows to iterate on element of the container
    //! and try to be compatible with std::containers iterators.
    class iterator : public std::iterator<std::input_iterator_tag, T*>
    {
    public:
      typedef T value_type;
      typedef T& reference;
      typedef T* pointer;
      typedef std::forward_iterator_tag iterator_category;

      //! \brief Empty constructor.
      iterator() : m_container(nullptr), m_itr(0) { }
      //! \brief Constructor by copy.
      iterator(const iterator& it) : m_container(it.m_container), m_itr(0) { }
      //! \brief Constructor from a conatianer
      iterator(container<T> const& c, const bool end) : m_container(&c)
      {
        if (end) m_itr = m_container->size();
        else m_itr = 0;
      }
      //! \brief Iterate on the next occuped element.
      inline iterator operator++(int)
      {
        iterator copy(*this); operator++(); return copy;
      }
      //! \brief Iterate on the next occuped element.
      iterator operator++()
      {
        check();
        do
          {
            m_itr++;
          }
        while ((m_itr < m_container->size()) &&
               (false == m_container->isOccupied(m_itr)));
        return *this;
      }
      T& operator*()
      {
        check();
        T* res = m_container->get(m_itr);
        assert(nullptr != res && "Invalid iterator dereference");
        return *res;
      }
      inline T* operator->()
      {
        return m_container->get(m_itr);
      }
      inline bool operator==(const iterator& rhs) const
      {
        check();
        return (m_container == rhs.m_container) && (m_itr == rhs.m_itr);
      }
      inline bool operator!=(const iterator& rhs) const
      {
        return !(*this == rhs);
      }

    private:
      inline void check() const { assert(nullptr != m_container && "Uninitialized iterator"); }
      const container<T>* m_container;
      uint32_t m_itr;
    };

  public:
    //! \brief Constructor. Allocate the X first pools of memory and set
    //! them as empty.
    container(const uint32_t no_pools = 1);
    //! \brief Destructor. Release all allocated pool of memory.
    ~container();
    //! \brief Allocate additional pool of memory.
    void reserve(const uint32_t no_pools);
    //! \brief Store an element at a given index. Complexity is O(1).
    //! Several pool allocations can be called if index is greater than
    //! the current limit of the container.
    void insert(const uint32_t index, T const& elt);
    //! \brief Same than insert but at th
    void append(T const& elt);
    //! \brief Remove an element at a given index. Complexity is O(1).
    //! No desallocation is called.
    void remove(const uint32_t index);
    void remove();
    //! \brief Check if the slot is occupied
    bool isOccupied(const uint32_t index, const uint32_t subindex) const;
    inline bool isOccupied(const uint32_t id) const
    {
      return isOccupied(id / M, MODULO(id, M));
    }
    //! \brief Return the element stored at the given index. Complexity is O(1).
    T* get(const uint32_t id) const;
    T* get(const uint32_t index, const uint32_t subindex) const;
    //!
    iterator begin() const
    {
      return iterator(*this, false);
    }
    iterator end() const
    {
      return iterator(*this, true);
    }
    //!
    inline uint32_t size() const
    {
      return m_index;
    }
    //! \brief Pretty print the memory.
    void debug() const;

  private:
    //!
    inline void insert(const uint32_t index,
                       const uint32_t subindex,
                       T const& elt)
    {
      // Insert the new element
      m_pools[index]->m_slots[subindex] = elt;

      // Flag it as 'occupied'
      m_pools[index]->m_occupied[subindex / S]
        |= (1 << (MODULO(subindex, S)));
    }
    //!
    inline void remove(const uint32_t index,
                       const uint32_t subindex)
    {
      // Flag it as 'occupied'
      m_pools[index]->m_occupied[subindex / S]
        &= ~(1 << (MODULO(subindex, S)));
    }
    //! Can be uint64_t. Index is used as 32-bits-field.
    typedef uint32_t Index;
    enum _S { S = sizeof (Index) };
    //! Number of storage slots. Must be a power of two for optimize
    //! the computation of modulo.
    enum _M { M = 1024U };
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
    //! \brief Number of memory pool allocated.
    uint32_t m_allocated;
  };
}

#endif /* GRAPHMEMORY_HPP_ */

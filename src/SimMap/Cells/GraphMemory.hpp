#ifndef GRAPHMEMORY_HPP_
#  define GRAPHMEMORY_HPP_

#  include <iostream>
#  include <vector>

//! \brief This file implements a class for allocating chunck of
//! memory specialized for storing graph elements (nodes, links)
//! where all elements have a unique ID and are stored by ID order
//! and we desired to avoid allocating and moving memory too much.

namespace SimTaDyn
{

  //! \class container is a container for graph. It is a collection of
  //! large but fixed size arrays. The access for adding, removing and
  //! searching an element is made in a constant time.
  template<class T> class container
  {
  public:
    //! \brief Constructor. Allocate a first pool of element all empty.
    container();
    ~container();
    //! \brief Store an element in the first free slot. Complexity is O(1).
    //! One pool allocation can be called.
    inline void append(T const& elt)
    {
      insert(m_nth_slot, elt);
      ++m_nth_slot;
    }
    //! \brief Store an element at a given index. Complexity is O(1).
    //! Several pool allocations can be called.
    void insert(const uint32_t index, T const& elt);
    //! \brief Remove an element at a given index. Complexity is O(1).
    //! No desallocation is called.
    void remove(const uint32_t index);
    //! \brief Check if the slot is occupied
    bool isOccupied(const uint32_t id) const;
    //! \brief Return the element stored at the given index. Complexity is O(1).
    T* get(const uint32_t id);
    //! \brief Pretty print the memory.
    void debug() const;

  private:
    //! Can be uint64_t. Index is used as 32-bits-field.
    typedef uint32_t Index;
    enum _S { S = sizeof (Index) };
    //! Number of storage slots. Must be a power of two for optimize
    //! the computation of modulo.
    enum _M { M = 1024U };
    //! Size of the index indicating which slots are occupied.
    enum _I { I = M / S };
    //! Private class for storing elements in a large array.
    template<class U> class pool
    {
    public:
      pool()
      {
        // Set all slots are not occupied
        Index i = I; while (i--) m_occupied[i] = 0;
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
    std::vector<pool<T>*> m_index;
    //! \brief Number of slots occupied.
    uint32_t m_nth_slot;
    //! \brief Number of memory pool allocated.
    uint32_t m_allocated;
  };

}

#endif /* GRAPHMEMORY_HPP_ */

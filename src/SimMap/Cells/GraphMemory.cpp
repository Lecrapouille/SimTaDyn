#include "GraphMemory.hpp"

// **************************************************************
//! \param no_pools the number of pool of memory to allocated.
// **************************************************************
template<class T>
SimTaDyn::container<T>::container(const uint32_t no_pools)
{
  m_index = m_allocated = 0;
  reserve(no_pools);
}

// **************************************************************
//! \param no_pools the number of pool of memory to allocated.
// **************************************************************
template<class T>
void SimTaDyn::container<T>::reserve(const uint32_t no_pools)
{
  uint32_t i = no_pools;

  while (i--)
    {
      m_pools.push_back(new poolContainer<T>);
    }
  m_allocated += no_pools;
}

// **************************************************************
//!
// **************************************************************
template<class T>
SimTaDyn::container<T>::~container()
{
  uint32_t i = m_allocated;

  while (i--)
    {
      delete m_pools[i];
    }
}

// **************************************************************
//! No check is made to verify if the memory is already used
//! \param id the index to store the element \c elt.
//! \param elt the element to store.
// **************************************************************
template<class T>
void SimTaDyn::container<T>::insert(const uint32_t id, T const& elt)
{
  const uint32_t index = id / M;
  const uint32_t subindex = MODULO(id, M);

  // Ressources are full, create at least one new pool memory
  if (index >= m_allocated)
    {
      reserve(1U + index - m_allocated);
    }
  insert(index, subindex, elt);
}

// **************************************************************
//!
// **************************************************************
template<class T>
void SimTaDyn::container<T>::append(T const& elt)
{
  insert(m_index, elt);
  ++m_index;
}

// **************************************************************
//! Ignore the case where index > m_allocated
// **************************************************************
template<class T>
void SimTaDyn::container<T>::remove(const uint32_t id)
{
  const uint32_t index = id / M;
  const uint32_t subindex = MODULO(id, M);

  if (index <= m_allocated)
    {
      remove(index, subindex);
    }
}

// **************************************************************
//!
// **************************************************************
template<class T>
void SimTaDyn::container<T>::remove()
{
  if (m_index > 0)
    {
      --m_index;
    }
  remove(m_index);
}

// **************************************************************
//!
// **************************************************************
template<class T>
bool SimTaDyn::container<T>::isOccupied(const uint32_t index,
                                        const uint32_t subindex) const
{
  if (index <= m_allocated)
    {
      return 0 != (m_pools[index]->m_occupied[subindex / S] &
                   (1 << (MODULO(subindex, S))));
    }
  return true;
}

// **************************************************************
//!
// **************************************************************
template<class T>
T* SimTaDyn::container<T>::get(const uint32_t id) const
{
  return get(id / M, MODULO(id, M));
}

// **************************************************************
//!
// **************************************************************
template<class T>
T* SimTaDyn::container<T>::get(const uint32_t index,
                               const uint32_t subindex) const
{
  if (index < m_allocated)
    {
      // Occuped
      if (0 != (m_pools[index]->m_occupied[subindex / S] &
                (1 << (MODULO(subindex, S)))))
        {
          return &(m_pools[index]->m_slots[subindex]);
        }
      // Not occuped
      return nullptr;
    }
  // Out of bound
  return nullptr;
}

// **************************************************************
//!
// **************************************************************
template<class T>
void SimTaDyn::container<T>::debug() const
{
  for (uint32_t pool = 0; pool < m_allocated; ++pool)
    {
      std::cout << "Pool " << pool << ": " << std::endl;
      poolContainer<T> *e = m_pools[pool];

      // Show elements stored
      for (uint32_t i = 0; i < M; ++i)
        {
          std::cout << " " << e->m_slots[i];
        }

      // Show occupation
      std::cout << std::endl;
      for (uint32_t i = 0; i < M; ++i)
        {
          std::cout << " " << isOccupied(pool, i);
        }
      std::cout << std::endl;
    }
}

int main()
{
  SimTaDyn::container<int> c(1);

  c.append(41); c.debug();
  c.append(42); c.debug();
  c.append(43); c.debug();
  c.append(44); c.debug();
  c.append(45); c.debug();

  c.remove(); c.debug();
  c.remove(); c.debug();
  c.remove(); c.debug();

  c.append(46); c.debug();
  c.append(47); c.debug();
  c.append(48); c.debug();

  std::cout << "Test iterator\n";
  for (SimTaDyn::container<int>::iterator i = c.begin(); i != c.end(); ++i)
    {
      std::cout << "III " << *i << std::endl;
    }

  return 0;
}

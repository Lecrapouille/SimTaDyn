#include "GraphMemory.hpp"

// Work if and only if M is a power of two number
#define MODULO(a, b) (a & (b - 1U))

// **************************************************************
//!
// **************************************************************
template<class T>
SimTaDyn::container<T>::container()
{
  m_nth_slot = m_allocated = 0;
  m_index.push_back(new pool<T>);
}

// **************************************************************
//!
// **************************************************************
template<class T>
SimTaDyn::container<T>::~container()
{
  // TODO: free all memory pools
}

// **************************************************************
//!
// **************************************************************
template<class T>
void SimTaDyn::container<T>::insert(const uint32_t id, T const& elt)
{
  uint32_t index = id / M;
  uint32_t subindex = MODULO(id, M);

  // The pool memory is full, add a new one
  while (index > m_allocated)
    {
      ++m_allocated;
      m_index.push_back(new pool<T>);
    }

  // Insert the new element
  m_index[index]->m_slots[subindex] = elt;

  // Flag it as 'occupied'
  m_index[index]->m_occupied[subindex / S]
    |= (1 << (MODULO(subindex, S)));
}

// **************************************************************
//!
// **************************************************************
template<class T>
void SimTaDyn::container<T>::remove(const uint32_t id)
{
  const uint32_t index = id / M;
  const uint32_t subindex = MODULO(id, M);

  if (index <= m_allocated)
    {
      // Flag it as 'not occupied'
      m_index[index]->m_occupied[subindex / S]
        &= (~(1 << (MODULO(subindex, S))));
    }
  // Ignore the case where index > m_allocated
}

// TODO: remove last

// **************************************************************
//!
// **************************************************************
template<class T>
bool SimTaDyn::container<T>::isOccupied(const uint32_t id) const
{
  const uint32_t index = id / M;
  const uint32_t subindex = MODULO(id, M);

  if (index <= m_allocated)
    {
      return 0 != (m_index[index]->m_occupied[subindex / S] &
                   (1 << (MODULO(subindex, S))));
    }
  return true;
}

// **************************************************************
//!
// **************************************************************
template<class T>
T* SimTaDyn::container<T>::get(const uint32_t id)
{
  const uint32_t index = id / M;
  const uint32_t subindex = MODULO(id, M);

  if (index < m_allocated)
    {
      // Occuped
      if (0 != (m_index[index]->m_occupied[subindex / S] &
                (1 << (MODULO(subindex, S)))))
        {
          return &(m_index[index]->m_slots[subindex]);
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
  for (uint32_t i = 0; i < m_index.size(); ++i)
    {
      std::cout << "Slot " << i << ": " << std::endl;
      pool<T>* e = m_index[i];

      // Show elements stored
      for (uint32_t j = 0; j < M; ++j)
        {
          std::cout << " " << e->m_slots[j];
        }

      // Show occupation
      std::cout << std::endl;
      for (uint32_t j = 0; j < M; ++j)
        {
          std::cout << " " << isOccupied(j);
        }
      std::cout << std::endl;
    }
}

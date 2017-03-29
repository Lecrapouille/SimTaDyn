// -*- c++ -*- Coloration Syntaxique pour Emacs

// **************************************************************
//! \param elt is the element of type T to insert.
// **************************************************************
template<typename T, const uint32_t N>
void Collection<T,N>::insert(const uint32_t nth, T const& elt)
{
  const uint32_t id = nth >> N;
  const uint32_t sid = MODULO(nth, M);

  // Reserve a new block of elements if all current blocks are
  // occupied.
  if (id >= IContainer<T,N>::m_allocated_blocks)
    {
      IContainer<T,N>::reserveBlocks(1U + id - IContainer<T,N>::m_allocated_blocks);
    }

  // Insert element and add the 'Occupied' flag
  IContainer<T,N>::m_blocks[id]->m_block[sid] = elt;
  if (!IS_OCCUPIED(id, sid))
    {
      SET_OCCUPIED(id, sid);
      ++IContainer<T,N>::m_stored_elements;

      // Update min and max bounding box used for iterators
      if (nth >= m_end)
        m_end = nth + 1;
      if (nth < m_begin)
        m_begin = nth;
      //std::cout << "after inserted ==> Min: " << m_begin << "  Max: " << m_end << "\n";
    }
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to remove.
// **************************************************************
template<typename T, const uint32_t N>
void Collection<T,N>::remove(const uint32_t nth)
{
  if (outofbound(nth))
    return ;

  const uint32_t id = nth >> N;
  const uint32_t sid = MODULO(nth, M);

  // Remove the 'Occupied' flag
  if (IS_OCCUPIED(id, sid))
    {
      CLEAR_OCCUPIED(id, sid);
      --IContainer<T,N>::m_stored_elements;

      if (nth == m_begin)
        {
          do {
            ++m_begin;
          } while ((m_begin < m_end) && (!IContainer<T,N>::occupied(m_begin)));
        }
      else if (nth + 1U == m_end)
        {
          while (m_end && (!IContainer<T,N>::occupied(m_end - 1U)))
            {
              --m_end;
            }
        }
      if (m_end == m_begin)
        {
          m_begin = (uint32_t) - 1;
          m_end = 0;
        }
      std::cout << "after removing ==> Min: " << m_begin << "  Max: " << m_end << "\n";
    }
}

// -*- c++ -*- Coloration Syntaxique pour Emacs

// **************************************************************
//! \param elt is the element of type T to insert.
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void Set<T,N,Block>::append(T const& elt)
{
  // Next element
  m_subindex = MODULO(m_subindex + 1U, M);
  m_index += !m_subindex;
  ++m_last;

  // Reserve a new block of elements if all current blocks are
  // occupied.
  if (m_index >= IContainer<T,N,Block>::m_allocated_blocks)
    {
      IContainer<T,N,Block>::reserveBlocks(1U);
    }

  // Insert element and add the 'Occupied' flag
  IContainer<T,N,Block>::m_blocks[m_index]->m_block[m_subindex] = elt;
  SET_OCCUPIED(m_index, m_subindex);
  ++IContainer<T,N,Block>::m_stored_elements;

  // Mark elements that changed; useful for update() routine.
  IContainer<T,N,Block>::m_blocks[m_index]->addPendingData(m_subindex);
}

// **************************************************************
//! \param nth the n'th element (index) of the IContainer we want
//! to remove.
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void Set<T,N,Block>::remove(const uint32_t nth)
{
  if (outofbound(nth))
    return ;

  // Replace the nth 'th element by the last inserted element
  if (nth != m_last)
    {
      const uint32_t id = nth / M;
      const uint32_t sid = MODULO(nth, M);

      IContainer<T,N,Block>::m_blocks[id]->m_block[sid] =
        IContainer<T,N,Block>::m_blocks[m_index]->m_block[m_subindex];

      // Mark elements that changed; useful for update() routine.
      IContainer<T,N,Block>::m_blocks[id]->addPendingData(sid);
    }

  // And remove the last element
  removeLast();
}

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
void Set<T,N,Block>::removeLast()
{
  // Empty the last inserted element
  CLEAR_OCCUPIED(m_index, m_subindex);

  // Possible behavior:
  // IContainer<T,N,Block>::m_blocks[m_index]->addPendingData(m_subindex, m_subindex);

  // Restore index
  if ((0 == m_last) || (INITIAL_INDEX == m_last))
    {
      m_index = m_subindex = m_last = INITIAL_INDEX;
    }
  else
    {
      --m_last;
      m_index = m_last / M;
      m_subindex = MODULO(m_last, M);
    }
  if (IContainer<T,N,Block>::m_stored_elements)
    {
      --IContainer<T,N,Block>::m_stored_elements;
    }
}

// **************************************************************
//! \param index1 index of the 1st element to swap.
//! \param index2 index of the 2nd element to swap.
// **************************************************************
template<typename T, const uint32_t N,
         template<typename X, const uint32_t Y> class Block>
bool Set<T,N,Block>::swap(const uint32_t index1, const uint32_t index2)
{
  // Do not swapt itself
  if (index1 == index2)
    return false;
  if (outofbound(index1))
    return false;
  if (outofbound(index2))
    return false;

  const uint32_t id1 = index1 / M;
  const uint32_t sid1 = MODULO(index1, M);
  const uint32_t id2 = index2 / M;
  const uint32_t sid2 = MODULO(index2, M);

  T elt = IContainer<T,N,Block>::m_blocks[id2]->m_block[sid2];
  IContainer<T,N,Block>::m_blocks[id2]->m_block[sid2] = IContainer<T,N,Block>::m_blocks[id1]->m_block[sid1];
  IContainer<T,N,Block>::m_blocks[id1]->m_block[sid1] = elt;

  // Note: does not need swap occupied bits because holes are not
  // possible.

  // Possible behavior:
  //IContainer<T,N,Block>::m_blocks[id2]->addPendingData(sid2);
  //IContainer<T,N,Block>::m_blocks[id1]->addPendingData(sid1);
  return true;
}

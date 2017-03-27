// -*- c++ -*- Coloration Syntaxique pour Emacs

// **************************************************************
//! \param elt is the element of type T to insert.
// **************************************************************
template<typename T, const uint32_t M>
void Collection<T,M>::insert(const uint32_t nth, T const& elt)
{
  const uint32_t id = nth / M;
  const uint32_t sid = MODULO(nth, M);

  // Reserve a new block of elements if all current blocks are
  // occupied.
  if (id >= IContainer<T,M>::m_allocated_blocks)
    {
      IContainer<T,M>::reserveBlocks(1U + id - IContainer<T,M>::m_allocated_blocks);
    }

  // Insert element and add the 'Occupied' flag
  IContainer<T,M>::m_blocks[id]->m_block[sid] = elt;
  if (!IS_OCCUPIED(id, sid))
    {
      SET_OCCUPIED(id, sid);
      ++IContainer<T,M>::m_stored_elements;
    }
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to remove.
// **************************************************************
template<typename T, const uint32_t M>
void Collection<T,M>::remove(const uint32_t nth)
{
  if (outofbound(nth))
    return ;

  const uint32_t id = nth / M;
  const uint32_t sid = MODULO(nth, M);

  // Remove the 'Occupied' flag
  if (IS_OCCUPIED(id, sid))
    {
      CLEAR_OCCUPIED(id, sid);
      --IContainer<T,M>::m_stored_elements;
    }
}

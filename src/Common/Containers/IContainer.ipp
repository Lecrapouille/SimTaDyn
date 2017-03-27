// -*- c++ -*- Coloration Syntaxique pour Emacs

// Work if and only if M is a power of two number
#define MODULO(a, b)             ((a) & ((b) - 1U))
#define OCCUPIED(id, sid)        (IContainer<T,M>::m_blocks[id]->m_occupied[sid / S])
#define OP_OCCUPIED(op, id, sid) (OCCUPIED(id, sid) op (1 << (MODULO(sid, S))))
#define SET_OCCUPIED(id, sid)    (OP_OCCUPIED(|=, id, sid))
#define CLEAR_OCCUPIED(id, sid)  (OP_OCCUPIED(&= ~, id, sid))
#define IS_OCCUPIED(id, sid)     (OP_OCCUPIED(&, id, sid))

// **************************************************************
//! \param nb_elts is the number of elements of type T we want to
//! pre-allocate. This value is automaticly rounded up to have at
//! least one block of elements created. Note: 0 is a possible value
//! in this case no blocks are allocated. Default value is 1.
// **************************************************************
template<typename T, const uint32_t M>
IContainer<T,M>::IContainer(const uint32_t nb_elts)
{
  m_stored_elements = 0;
  m_allocated_blocks = 0;
  reserve(nb_elts);
}

// **************************************************************
//! \param nb_elts is the number of elements of type T we want to
//! pre-allocate. This value is automaticly rounded up to have at
//! least one block of elements created.
// **************************************************************
template<typename T, const uint32_t M>
void IContainer<T,M>::reserve(const uint32_t nb_elts)
{
  reserveBlocks((nb_elts + M - 1) / M);
}

// **************************************************************
//! \param nb_blocks is the number of blocks of elements of type T we
//! want to pre-allocate.
// **************************************************************
template<typename T, const uint32_t M>
void IContainer<T,M>::reserveBlocks(const uint32_t nb_blocks)
{
  std::cout << "Reserving " << nb_blocks << " blocks\n";
  m_blocks.reserve(m_blocks.capacity() + nb_blocks);
  uint32_t i = nb_blocks;

  while (i--)
    {
      m_blocks.push_back(newBlock());
    }
  m_allocated_blocks += nb_blocks;
}

// **************************************************************
//! \param nth the n'th element (index) of the IContainer we want
//! to get.
//! \return the reference of the element.
//! \throw std::out_of_range if n'th is an invalid index (outside
//! the position of the last insereted element) or if the element
//! has been removed.
// **************************************************************
template<typename T, const uint32_t M>
T& IContainer<T,M>::get(const uint32_t nth)
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const uint32_t id = nth / M;
  const uint32_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  return m_blocks[id]->m_block[sid];
}

template<typename T, const uint32_t M>
T const& IContainer<T,M>::get(const uint32_t nth) const
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const uint32_t id = nth / M;
  const uint32_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  return m_blocks[id]->m_block[sid];
}

// **************************************************************
//! \param
// **************************************************************
template<typename T, const uint32_t M>
bool IContainer<T,M>::occupied(const uint32_t nth)
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const uint32_t id = nth / M;
  const uint32_t sid = MODULO(nth, M);

  return IS_OCCUPIED(id, sid);
}

// **************************************************************
//! \param
// **************************************************************
template<typename T, const uint32_t M>
void IContainer<T,M>::debug() const
{
  if (empty())
    {
      std::cout << "The container is empty" << std::endl;
      return ;
    }
  for (uint32_t block = 0; block < m_allocated_blocks; ++block)
    {
      std::cout << "Block " << block << ": " << std::endl;
      block_t *e = m_blocks[block];

      // Show elements stored
      for (uint32_t i = 0; i < M; ++i)
        {
          if (e->m_occupied[i / S] & (1 << (MODULO(i, S))))
            {
              std::cout << " " << e->m_block[i];
            }
          else
            {
              std::cout << " <empty>";
            }
        }
      std::cout << std::endl;
    }
}

// -*- c++ -*- Coloration Syntaxique pour Emacs

// Perform a % b but works if and only if M is a power of two number.
#define MODULO(a, b)             ((a) & ((b) - 1U))
// Accessor to the bitfield indicationg which elements are stored.
#define OCCUPIED(id, sid)        (IContainer<T,N>::m_blocks[id]->m_occupied[sid / S])
// Perform a generic operation on the bitfield.
#define OP_OCCUPIED(op, id, sid) (OCCUPIED(id, sid) op (1 << (MODULO(sid, S))))
// Set the 'occupied' flag in the bitfield.
#define SET_OCCUPIED(id, sid)    (OP_OCCUPIED(|=, id, sid))
// Set the 'empty' flag in the bitfield.
#define CLEAR_OCCUPIED(id, sid)  (OP_OCCUPIED(&= ~, id, sid))
// Check if the element is stored.
#define IS_OCCUPIED(id, sid)     (OP_OCCUPIED(&, id, sid))

// **************************************************************
//! \param nb_elts is the number of elements of type T we want to
//! pre-allocate. This value is automaticly rounded up to have at
//! least one block of elements created. Note: 0 is a possible value
//! in this case no blocks are allocated. Default value is 1.
// **************************************************************
template<typename T, const uint32_t N>
IContainer<T,N>::IContainer(const uint32_t nb_elts)
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
template<typename T, const uint32_t N>
void IContainer<T,N>::reserve(const uint32_t nb_elts)
{
  reserveBlocks((nb_elts + M - 1) >> N);
}

// **************************************************************
//! \param nb_blocks is the number of blocks of elements of type T
//! we want to pre-allocate.
// **************************************************************
template<typename T, const uint32_t N>
void IContainer<T,N>::reserveBlocks(const uint32_t nb_blocks)
{
  std::cout << "Reserving " << nb_blocks << " blocks\n";
  m_blocks.reserve(m_blocks.capacity() + nb_blocks);

  uint32_t i = nb_blocks;
  while (i--)
    {
      m_blocks.push_back(newBlock());
    }
  m_allocated_blocks += nb_blocks;
  assert(m_allocated_blocks == m_blocks.size());
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to get.
//! \return the reference of the element.
//! \throw std::out_of_range if n'th is an invalid index (outside
//! the position of the last insereted element) or if the element
//! has been removed.
// **************************************************************
template<typename T, const uint32_t N>
T& IContainer<T,N>::get(const uint32_t nth)
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const uint32_t id = nth >> N;
  const uint32_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  return m_blocks[id]->m_block[sid];
}

// **************************************************************
// FIXME code in double with previous code
// **************************************************************
template<typename T, const uint32_t N>
T const& IContainer<T,N>::get(const uint32_t nth) const
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const uint32_t id = nth >> N;
  const uint32_t sid = MODULO(nth, M);

  if (!IS_OCCUPIED(id, sid))
    {
      throw std::out_of_range("Empty element at index " + std::to_string(nth));
    }
  return m_blocks[id]->m_block[sid];
}

// **************************************************************
//! \param nth the n'th element (index) of the container we want
//! to check.
//! \return true if an element is present, else return false.
// **************************************************************
template<typename T, const uint32_t N>
bool IContainer<T,N>::occupied(const uint32_t nth) const
{
  if (outofbound(nth))
    {
      throw std::out_of_range("Out of range index " + std::to_string(nth));
    }

  const uint32_t id = nth >> N;
  const uint32_t sid = MODULO(nth, M);

  return !(!(IS_OCCUPIED(id, sid)));
}

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N>
void IContainer<T,N>::garbage()
{
  // Important: start by the end
  uint32_t index = m_allocated_blocks;
  while (index--)
    {
      ContainerBitField subindex = M;
      while (subindex--)
        {
          // Stop algorithm at first non empty block
          if (IS_OCCUPIED(index, subindex))
            return ;
        }

      // Empty block
      delete m_blocks[index];
      --m_allocated_blocks;
      m_blocks.pop_back();
      assert(m_allocated_blocks == m_blocks.size());
    }
}

// **************************************************************
//!
// **************************************************************
template<typename T, const uint32_t N>
void IContainer<T,N>::debug() const
{
  if (empty())
    {
      std::cout << "The container is empty" << std::endl;
      return ;
    }
  for (uint32_t index = 0; index < m_allocated_blocks; ++index)
    {
      std::cout << "Block " << index << ": " << std::endl;

      // Show elements stored
      for (uint32_t subindex = 0; subindex < M; ++subindex)
        {
          if (IS_OCCUPIED(index, subindex))
            {
              std::cout << " " << m_blocks[index]->m_block[subindex];
            }
          else
            {
              std::cout << " <empty>";
            }
        }
      std::cout << std::endl;
    }
}

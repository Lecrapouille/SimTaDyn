  //! \brief Number of elements by block: 2^N
  enum { M = (1ul << N) };
  //! \brief Number of bytes by integer: 4 for uint32_t, 8 for uint64_t
  enum { B = sizeof (ContainerBitField) };
  //! \brief Get the number of bits instead of numbers of bytes
  enum { S = B * 8ul };
  //! \brief Precompute round(M / S)
  enum { E = (M + S - 1) / S };

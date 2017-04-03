  //! \brief Number of elements by block: 2^N
  enum { M = (1U << N) };
  //! \brief Number of bytes by integer: 4 for uint32_t, 8 for uint64_t
  enum { B = sizeof (ContainerBitField) };
  //! \brief Number of bits by integer: 32 bits for uint32_t
  enum { S = B * 8U };
  //! \brief Precompute round(M / S)
  enum { E = (M + S - 1U) / S };

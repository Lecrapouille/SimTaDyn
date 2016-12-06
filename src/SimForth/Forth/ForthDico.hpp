#ifndef FORTH_DICO_HPP_
#  define FORTH_DICO_HPP_

#  include "ForthExceptions.hpp"
#  include "ForthPrimitives.hpp"

class ForthDico
{
public:
  ForthDico();
  void add(const Cell16 token, std::string const& word, const bool immediate);
  bool find(std::string const& word, Cell16& token, bool& immediate) const;
  bool exists(std::string const& word) const;
  std::pair<bool, int32_t> find(const Cell16 token, const bool even_smudge) const;
  bool smudge(std::string const& word);
  bool dump(std::string const& filename);// FIXME const;
  bool load(std::string const& filename);
  void display() const;
  inline Cell16 last() const { return m_last; }
  inline Cell16 here() const { return m_here; }
  inline void here(const Cell16 here) { m_here = here; }
  void allot(const int32_t nb_bytes);
  inline void appendCell8(const Cell32 value)
  {
    write8at(m_here, value);
    m_here += 1U;
  }
  inline void appendCell16(const Cell32 value)
  {
    write16at(m_here, value);
    m_here += 2U;
  }
  inline void appendCell32(const Cell32 value)
  {
    write32at(m_here, value);
    m_here += 4U;
  }
  void write8at(const uint32_t addr, const Cell32 data);
  void write16at(const uint32_t addr, const Cell32 data);
  void write32at(const uint32_t addr, const Cell32 data);
  Cell32 read8at(const uint32_t addr) const;
  Cell32 read16at(const uint32_t addr) const;
  Cell32 read32at(const uint32_t addr) const;
  inline void move(const uint32_t destination,
                   const uint32_t source,
                   const uint32_t nbytes)
  {
    checkBounds(source, nbytes);
    checkBounds(destination, nbytes);
    std::memmove(m_dictionary + destination,
                 m_dictionary + source,
                 nbytes);
  }
  void displayToken(const Cell16 token) const;

protected:

  void checkBounds(const uint32_t addr, const int32_t nb_bytes) const;

  friend class Forth;

  Cell8   m_dictionary[DICTIONARY_SIZE];
  Cell16  m_last;  // NFA of the last dictionnary entry (Forth word: LAST).
  Cell16  m_here;  // Address of the next available dictionary location (word: HERE).
};

#endif /* FORTH_DICO_HPP_ */

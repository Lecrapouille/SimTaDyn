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
  std::pair<bool, int32_t> find(const Cell16 token) const;
  bool smudge(std::string const& word);
  bool dump(std::string const& filename) const;
  void display() const;
  Cell16 last() const;
  Cell16 here() const;
  void allot(const int32_t nb_bytes);
  void appendCell8(const Cell32 value);
  void appendCell16(const Cell32 value);
  void appendCell32(const Cell32 value);
  void write8at(const uint32_t addr, const Cell32 data);
  void write16at(const uint32_t addr, const Cell32 data);
  void write32at(const uint32_t addr, const Cell32 data);
  Cell32 read8at(const uint32_t addr) const;
  Cell32 read16at(const uint32_t addr) const;
  Cell32 read32at(const uint32_t addr) const;

protected:

  void checkBounds(const uint32_t addr, const int32_t nb_bytes) const;

  friend class Forth;

  Cell8   m_dictionary[DICTIONARY_SIZE];
  Cell16  m_last;  // NFA of the last dictionnary entry.
  Cell16  m_here;  // Address of the next available dictionary location.
};

#endif /* FORTH_DICO_HPP_ */

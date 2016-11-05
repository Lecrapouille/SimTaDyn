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
  std::pair<bool, int32_t> find(const Cell16 token) const;
  bool dump(std::string const& filename) const;
  void display() const;
  Cell16 last() const;
  Cell16 here() const;

  //void addPadding16(Cell8 *addr);
  //void addPadding32(Cell8 *addr);
  void appendCell8(Cell8 value);
  void appendCell16(Cell16 value);
  void appendCell32(Cell32 value);
  void write8at(Cell8 *addr, Cell8 data);
  void write16at(Cell8 *addr, Cell16 data);
  void write32at(Cell8 *addr, Cell32 data);
  Cell8  read8at(Cell8 *addr) const;
  Cell16 read16at(Cell8 *addr) const;
  Cell32 read32at(Cell8 *addr) const;
  Cell16 *ip(Cell16 token);

protected:

  Cell8   m_dictionary[DICTIONARY_SIZE];
  Cell16  m_last;  // NFA of the last dictionnary entry.
  Cell16  m_here;  // Address of the next available dictionary location.
};

#endif /* FORTH_DICO_HPP_ */

#ifndef FORTH_DICO_HPP_
#  define FORTH_DICO_HPP_

//! \brief This file contains the class managing the Forth dictionary.

#  include "ForthExceptions.hpp"
#  include "ForthPrimitives.hpp"

//! \class ForthDico
//! \brief class for managing the Forth dictionary.
//!
//! In Forth a dictionary is a consecutive set of Forth words. A Forth
//! word is known as "function" in language like C with a name and a
//! definition. In our implementation Forth words are stored in the
//! dictionary as a linked list of dictionary entries.
//!
//!  <--- DICTIONARY ENTRY (HEADER) ----------------------->
//!  +------------------------+--------+---------- - - - - +----------- - - - -
//!  | LINK POINTER           | LENGTH/| NAME              | DEFINITION
//!  |                        | FLAGS  |                   |
//!  +--- (2 bytes) ----------+- 1byte +- 1 .. 31 bytes  - +----------- - - - -
//!
//! Link pointer contains the RELATIVE address (not absolute) named
//! LFA to the previous entry. With 2 bytes Our dictionary has a
//! limited size of 65535 bytes. The most recently defined word is
//! memorized by the member m_last of the class ForthDico and in Forth
//! the word LAST refered to it.
//!
//! Flags contains the length of the word name coded on 5 bits. 3
//! additional bits are used: the 3th bit is always set to 1 as a
//! marker for detecting the begining of the definiton. The 2nd bit is
//! the smudge bit: hiding or not the definition and the 1st bit for
//! defining IMMEDIATE words (a word executed during the compilation
//! of a word).
//!
//! The definition of a Forth is a list of consecutive tokens. The
//! address of the begining of the definition is named Code Field
//! Address (CFA). A token is either the CFA of none-primitive word or
//! a value for primitive words. A primitive word is here a C++ code.
//!
//! In C language a space memory is used for storing variables (the heap)
//! but in Forth the dictionary is used as a heap.
class ForthDico
{
public:
  //! \brief Constructor.
  ForthDico();
  //! \brief Add a new Forth word in the dictionary.
  void add(const Cell16 token, std::string const& word, const bool immediate);
  //! \brief Look for a word in the dictionary.
  bool find(std::string const& word, Cell16& token, bool& immediate) const;
  //! \brief Same behavior than find(std::string const& word ...) but without outpout paramaters
  bool exists(std::string const& word) const;
  //! \brief Look for a token in the dictionary.
  std::pair<bool, int32_t> find(const Cell16 token, const bool even_smudge = false) const;
  //! \brief Look for a word name from a partial name (used for auto-completion).
  std::pair<bool, char*> completion(Cell16& last, std::string const& partial_word) const;
  //! \brief Hide or unhide a Forth definition from the user.
  bool smudge(std::string const& word);
  //! \brief Save the whole content of the dictionary in a binary file.
  bool dump(std::string const& filename);// FIXME const;
  //! \brief Load a dictionary from a binary file, append or replace the old one.
  bool load(std::string const& filename, const bool replace = true);
  //! \brief Pretty print the dictionary.
  virtual void display() const;
  //! \brief Accessor. Return the last word entry in the dictionary.
  inline Cell16 last() const { return m_last; }
  //! \brief Accessor. Return the address of the 1st free slot in the dictionary.
  inline Cell16 here() const { return m_here; }
  //! \brief Accessor. Modify the address of the 1st free slot in the dictionary.
  inline void here(const Cell16 here) { m_here = here; }
  //! \brief Reserve memory in the dictionary from the 1st free slot and modify it.
  void allot(const int32_t nb_bytes);
  //! \brief Store a byte at the end of the dictionnary.
  inline void appendCell8(const Cell32 value)
  {
    write8at(m_here, value);
    m_here += 1U;
  }
  //! \brief Store two consecutive bytes at the end of the dictionnary.
  inline void appendCell16(const Cell32 value)
  {
    write16at(m_here, value);
    m_here += 2U;
  }
  //! \brief Store four consecutive bytes at the end of the dictionnary.
  inline void appendCell32(const Cell32 value)
  {
    write32at(m_here, value);
    m_here += 4U;
  }
  //! \brief Read a byte at given address in the dictionnary.
  Cell32 read8at(const uint32_t addr) const;
  //! \brief Read two consecutive bytes at given address in the dictionnary.
  Cell32 read16at(const uint32_t addr) const;
  //! \brief Read four consecutive bytes at given address in the dictionnary.
  Cell32 read32at(const uint32_t addr) const;
  //! \brief Safe move a chunck of memory in the dictionary.
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

protected:
  //! \brief Pretty print a Forth token
  virtual void displayToken(const Cell16 token) const;
  //! \brief Store a byte at given address in the dictionnary.
  void write8at(const uint32_t addr, const Cell32 data);
  //! \brief Store two consecutive bytes at given address in the dictionnary.
  void write16at(const uint32_t addr, const Cell32 data);
  //! \brief Store four consecutive bytes at given address in the dictionnary.
  void write32at(const uint32_t addr, const Cell32 data);
  //! \brief Safe guard. Check if given address is inside the dictionary.
  void checkBounds(const uint32_t addr, const int32_t nb_bytes) const;

  //! \brief Allow the Forth context class to modify the dictionary.
  friend class Forth;

  Cell8   m_dictionary[DICTIONARY_SIZE]; //! The memory of the dictionary.
  Cell16  m_last;  //! Name Field Address (NFA) of the last dictionnary entry (Forth word: LAST).
  Cell16  m_here;  //! Address of the 1st free dictionary slot (Forth word: HERE, DP).
};

#endif /* FORTH_DICO_HPP_ */

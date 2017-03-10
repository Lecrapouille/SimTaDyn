#ifndef FORTH_DICTIONARY_HPP_
#  define FORTH_DICTIONARY_HPP_

//! \brief This file contains the class managing the Forth dictionary.

#  include "ForthExceptions.hpp"
#  include "ForthPrimitives.hpp"
#  include "TextColor.hpp"
#  include <cstring>

//! \class ForthDictionary
//!
//! In Forth, a dictionary is a consecutive set of Forth words. The
//! term word is equivalent to the term "function" in language like C
//! with a name and a definition (but contrary to C parameters are not
//! described in the dictionary). In this current implementation of
//! Forth, words are stored in the dictionary as a linked list of
//! dictionary entries like shwon in the next figure.
//!
//! \code{.unparsed}
//!  <--- DICTIONARY ENTRY (HEADER) ----------->
//!  +--------+-----------------+--------------+-----------+---- - - - -
//!  | LENGTH/| NAME            | LINK POINTER | TOKEN     | DEFINITION
//!  | FLAGS  |                 |              |           |
//!  + 1 byte +- 1 to 31 bytes -+-- 2 bytes ---+--2 bytes--+---- - - - -
//! \endcode
//!
//! Example:
//!
//! ForthDictionary::dump() saves a dictionary in a binary file. Calling hexdump -C
//! on this file will give a result similar to this:
//! \code{.unparsed}
//! 00000000  84 4e 4f 4f 50 00 00 00  00 c1 28 00 09 00 01  |.NOOP.....(.....|
//! \endcode
//!
//! Address 0x00000000 in the dictionary is the entry of the word NOOP.
//! 0x84 as flags means the word name is 4 characters. 0x4e 0x4f 0x4f 0x50 are
//! the ASCII code for "NOOP". 0x00 0x00 means the token 0 (primitive
//! word) 0x00  0x00 is the LFA of the previous name. In this case because
//! NOOP is the first entry there is no previous word so equivalent to NULL in C
//! language.
//!
//! Address 0x00000008 in the dictionary is the entry of the word (.
//! 0xc1 as flags means the word name is 1 character and immediate. 0x28 is
//! the ASCII code for (. 0x00 0x09 is LFA pointing to NOOP. 0x00 0x01
//! is the token 1 (primitive word).
//!
//! Description of the figure:
//!
//! LENGTH/FLAGS contains the length of the word name coded on 5 bits
//! (so a Forth word cannot have more than 31 caracters). 3
//! additional bits are used: the 3th bit is always set to 1 and is
//! used as a marker for detecting the begining of the entry. The 2nd
//! bit is the smudge bit: hiding or not the definition to find
//! functions and the first bit is used for defining IMMEDIATE words (a
//! immediate word is executed during the compilation of a word).
//!
//! NAME is the name of the word. Up to 31 caracter (usually ASCII).
//!
//! The LINK POINTER contains the RELATIVE address (not absolute) to
//! the previous entry and it's named LFA. Note that with 2 bytes our
//! dictionary has a maximum size of 65535 bytes. The most recently
//! defined word is memorized by the member ForthDictionary::m_last and in
//! Forth by the word LAST.
//!
//! TOKEN is an unique identifier for each word in dictionary. It also
//! allows to distinguish two kind of Forth words: primitives and
//! compiled words. Primitives are Forth words calling machine code.
//!
//! The DEFINITION of the Forth word is a list of consecutive tokens.
//! The address of the begining of the definition is named Code Field
//! Address (CFA).
//!
//! In C language a space memory is used for storing variables (the
//! heap) but in Forth the dictionary is both used as a heap and as
//! entries store. That is why Forth is considered as a virtual
//! machine.  The first free location in the dictionary is given by
//! ForthDictionary::m_here and in Forth by the word DP or the word HERE.
//! Contrary to other Forth virtual machine, here our dictionary does
//! not need to manage aligned memories (padding) or manage endianess.
class ForthDictionary
{
public:
  //! \brief Constructor.
  ForthDictionary();
  ~ForthDictionary();
  void add(const Cell16 token, char const* name, const bool immediate);
  //! \brief Append a new Forth entry in the dictionary.
  void add(const Cell16 token, std::string const& word, const bool immediate);
  void add(const Cell16 token, char const* name, const uint32_t length, const bool immediate);
  //! \brief Look for a word in the dictionary.
  bool find(std::string const& word, Cell16& token, bool& immediate) const;
  //! \brief Interface for ForthDictionary::find but hiding output parameters.
  bool exists(std::string const& word) const;
  //! \brief Look for a token in the dictionary.
  std::pair<bool, int32_t> find(const Cell16 token, const bool even_smudge = false) const;
  //! \brief Get the complete name given a partial Forth name (used for auto-completion).
  const char* completion(Cell16& last, std::string const& partial_word) const;
  //! \brief Hide or unhide a Forth definition from the user.
  bool smudge(std::string const& word);
  //! \brief Save the whole content of the dictionary in a binary file.
  bool dump(std::string const& filename);// FIXME const;
  //! \brief Load a dictionary from a binary file, append or replace the old one.
  bool load(std::string const& filename, const bool replace = true);
  //! \brief Pretty print the dictionary in std::cout.
  virtual void display(TextColor& display) const;
  //! \brief Accessor. Return the most recent entry in the dictionary.
  inline Cell16 last() const { return m_last; }
  //! \brief Accessor. Return the address of the first free location in the dictionary.
  inline Cell16 here() const { return m_here; }
  //! \brief Accessor. Modify the address of the first free location in the dictionary.
  inline void here(const Cell16 here) { m_here = here; }
  //! \brief Reserve or release a chunk of memory in the dictionary.
  void allot(const int32_t nb_bytes);
  //! \brief Store a byte at the end of the dictionnary. Endianess is hiden.
  //! ForthDictionary::m_here is updated.
  //! \param data is a 32-bits data (casted into Cell8) to store at location ForthDictionary::m_here
  //! \throw OutOfBoundDictionary if overflows/underflows is detected.
  inline void appendCell8(const Cell32 data)
  {
    write8at(m_here, data);
    m_here += 1U;
  }
  //! \brief Store two consecutive bytes at the end of the dictionnary. Endianess is hiden.
  //! ForthDictionary::m_here is updated.
  //! \param data is a 32-bits data (casted into Cell16) to store at location ForthDictionary::m_here
  //! \throw OutOfBoundDictionary if overflows/underflows is detected.
  inline void appendCell16(const Cell32 data)
  {
    write16at(m_here, data);
    m_here += 2U;
  }
  //! \brief Store four consecutive bytes at the end of the dictionnary. Endianess is hiden.
  //! ForthDictionary::m_here is updated.
  //! \param data is a 32-bits data to store at location ForthDictionary::m_here
  //! \throw OutOfBoundDictionary if overflows/underflows is detected.
  inline void appendCell32(const Cell32 data)
  {
    write32at(m_here, data);
    m_here += 4U;
  }
  //! \brief Read a byte at given address in the dictionnary. Endianess is hiden.
  Cell32 read8at(const uint32_t addr) const;
  //! \brief Read two consecutive bytes at given address in the dictionnary. Endianess is hiden.
  Cell32 read16at(const uint32_t addr) const;
  //! \brief Read four consecutive bytes at given address in the dictionnary. Endianess is hiden.
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
  //! \brief Pretty print a Forth token in std::cout.
  virtual std::string displayToken(const Cell16 token) const;
  //! \brief Store a byte at given address in the dictionnary. Endianess is hiden.
  void write8at(const uint32_t addr, const Cell32 data);
  //! \brief Store two consecutive bytes at given address in the dictionnary. Endianess is hiden.
  void write16at(const uint32_t addr, const Cell32 data);
  //! \brief Store four consecutive bytes at given address in the dictionnary. Endianess is hiden.
  void write32at(const uint32_t addr, const Cell32 data);
  //! \brief Safe guard. Check if given address is inside the dictionary.
  void checkBounds(const uint32_t addr, const int32_t nb_bytes) const;

  //! \brief Allow the Forth context class to modify the dictionary.
  friend class Forth;

  //! The memory of the dictionary.
  Cell8   m_dictionary[DICTIONARY_SIZE];
  //! Name Field Address (NFA) of the most recently entry (Forth word: LAST).
  Cell16  m_last;
  //! Address of the first free location in the dictionary (Forth word: HERE, DP).
  Cell16  m_here;
  // FIXME std::string m_name;
};

#endif /* FORTH_DICTIONARY_HPP_ */

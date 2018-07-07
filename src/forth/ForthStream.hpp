//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTH_READER_HPP_
#  define FORTH_READER_HPP_

//! \brief This file contains the class managing a stream for feeding a
//! Forth interpreter. See this class like a word splitter.

#  include "ForthHelper.hpp"
// #  include <fstream>

class ForthStream
{
public:
  //! \brief Constructor. No stream are yet opened.
  ForthStream();
  //! \brief Destructor. Close the opened stream.
  ~ForthStream();
  //! \brief For feeding a Forth interpreter with an ascii file.
  bool loadFile(std::string const& filename);
  //! \brief For feeding a Forth interpreter with a string.
  void loadString(std::string const& str,
                  std::string const& name = "<string>");
  //! \brief Drop the current line and load the next one.
  void skipLine();
  //! \brief Check if a word can be extracted it's extracted.
  bool hasMoreWords();
  //! \brief Return the current line
  std::string getLine();
  //! \brief Accessor. Return the current extracted Forth word
  //! called by ForthStream::hasMoreWords().
  inline const std::string& nextWord() const
  {
    //m_word_picked = true;
    //std::cout << "nextWord '" << m_word << "'" << std::endl;
    return m_word;
  }
  //! \brief Accessor. Give the current cursor (line and column)
  //! of the opened stream.
  //! \return the current line and column
  std::pair<size_t, size_t> position() const
  {
    return std::make_pair(m_lines, m_cursor_prev + 1U);
  }
  //! \brief Accessor. Give the name of the opened stream.
  //! \return the filename currently reading, else return "" if reading
  //! a string.
  inline const std::string& name() const
  {
    return m_filename;
  }
  inline void name(const std::string& filename)
  {
    m_filename = filename;
  }
  inline bool eol() const { return m_eol; }
  inline bool eof() const { return m_eof; }
  //! \brief release the opened stream.
  void close();

protected:
  //! \brief Reset the stream states except the stream is still open.
  void init();
  //! \brief When the current line has been parse refill with the next one.
  bool refill();
  //! \brief Read the next word delimited by space characters.
  bool split();

  enum Mode
  {
    NOTHING_TO_READ,
    READ_FILE,
    READ_STRING
  };

  // white-space characters (from isspace() doc)
  const std::string m_delimiters = " \t\n\v\f\r";

  enum Mode m_mode;       // Read from file or from string
  std::ifstream m_infile; // Opened file
  std::string m_filename; // The file name to read
  std::string m_str;      // The string to read
  std::string m_word;     // The Forth word read (token)
  size_t m_cursor_last;   // Split iterator on Forth words
  size_t m_cursor_next;   // Split iterator on Forth words
  size_t m_cursor_prev;
  size_t m_lines;         // Line counter
  bool m_eol;             // End of line reached ?
  bool m_eof;             // End of file reached ?
  //bool m_word_picked;     // The current word has been picked by the caller

public:

  int32_t m_base;         // Save the base (octal, decimal, hexa) for displaying numbers
};

#endif /* FORTH_READER_HPP_ */

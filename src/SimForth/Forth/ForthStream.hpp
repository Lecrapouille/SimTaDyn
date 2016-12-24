#ifndef FORTH_READER_HPP_
#  define FORTH_READER_HPP_

//! \brief This file contains the class managing a stream for feeding a
//! Forth interpreter. See this class like a word splitter.

#  include "ForthHelper.hpp"

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
  void loadString(std::string const& str);
  //! \brief Drop the current line and load the next one.
  void skipLine();
  //! \brief Check if a word can be extracted it's extracted.
  bool hasMoreWords();
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
  bool m_word_picked;     // The current word has been picked by the caller
};

#endif /* FORTH_READER_HPP_ */

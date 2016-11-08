#ifndef FORTH_READER_HPP_
#  define FORTH_READER_HPP_

#  include "ForthHelper.hpp"

class ForthReader
{
public:
  ForthReader();
  ~ForthReader();
  bool setFileToParse(std::string const& filename);
  void setStringToParse(std::string const& text);
  void skipLine();
  bool hasMoreWords();
  std::string nextWord();
  std::pair<size_t, size_t> cursors();
  std::string file();

protected:
  void init();
  bool refill();
  bool split();

  enum Mode
  {
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
  size_t m_lines;         // Line counter
  bool m_eol;             // End of line reached ?
  bool m_eof;             // End of file reached ?
  bool m_word_picked;     // The current word has been picked by the caller
};

#endif /* FORTH_READER_HPP_ */

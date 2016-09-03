#ifndef FORTHREADER_HPP_
#  define FORTHREADER_HPP_

#  include "simtypes.hpp"
#  include <fstream>
#  include <utility>

class ForthTxtReader
{
public:
  ForthTxtReader()
  {
    last = next = line = 0;
  }
  ~ForthTxtReader()
  {
    if (infile.is_open())
      {
        infile.close();
        infile.clear();
      }
  }
  bool openForthFile(std::string const& filename)
  {
    mode = READ_FILE;
    last = next = line = 0;
    eol = true;
    if (infile.is_open())
      {
        infile.close();
        infile.clear();
      }
    infile.open(filename, std::ios::in);
    return infile.good();
  }
  void useForthString(std::string const& codeForth)
  {
    mode = READ_STRING;
    last = next = line = 0;
    eol = false;
    code_forth = codeForth;
  }
  bool getNextWord(std::string& word)
  {
    std::pair<bool, std::string> res;

    // If read in a file, check if we need to read next line
    if ((READ_FILE == mode) && (infile.is_open()))
      {
        if (eol)
          {
            ++line;
            last = next = 0;
            eol = !(std::getline(infile, code_forth).good());
          }
      }

    if (!eol)
      {
        // There is still ascii data in the buffer
        res = split(code_forth);
        eol = res.first;
      }
    else
      {
        // We reached the end of the line
        return false;
      }

    word = res.second;
    return true;
  }

protected:
  // Return true if reached the end of the string, else false.
  std::pair<bool, std::string> split(std::string const& code_forth)
  {
    std::string delimiter = " \t\n";
    std::string word;

    if ((next = code_forth.find_first_of(delimiter, last)) != std::string::npos)
      {
        word = code_forth.substr(last, next - last);
        last = next + 1;
        return std::pair<bool, std::string>(false, word);
      }
    word = code_forth.substr(last);
    return std::pair<bool, std::string>(true, word);
  }

  enum Mode
  {
    READ_FILE = 0,
    READ_STRING
  };

  std::string code_forth;
  size_t last;
  size_t next;
  size_t line;
  bool eol;
  std::ifstream infile;
  enum Mode mode;
};

#endif /* FORTHREADER_HPP_ */

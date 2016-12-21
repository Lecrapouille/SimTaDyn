#include "ForthInner.hpp"
#include <limits.h>

// **************************************************************
//! Constructor: initialize the Forth interpretor context. The
//! dictionary is given as reference to allow the inheritance.
//! \param dico the reference of (an empty) Forth dictionary.
// **************************************************************
Forth::Forth(ForthDico& dico)
  : m_dico(dico)
{
  m_base = 10U;
  abort();
  m_last_completion = m_dico.last();
}

// **************************************************************
//! Reset the context like the Forth word ABORT does.
// **************************************************************
inline void Forth::abort()
{
  if ((COMPILATION_STATE == m_state) ||
      ((COMMENT_STATE == m_state) && (COMPILATION_STATE == m_saved_state)))
    {
      // Drop the Forth word not totaly compiled
      m_dico.m_last = m_last_at_colon;
      m_dico.m_here = m_here_at_colon;
    }

  m_state = INTERPRETER_STATE;
  m_dsp = m_data_stack;
  m_asp = m_alternative_stack;
  m_rsp = m_return_stack;
  m_opened_streams = 0;
  m_trace = false;
}

// **************************************************************
//! When displaying numbers on screen, display them on the
//! selected base. For example 10 in base 10 => A in base 16.
//! \param base the new displaying base.
//! \return if the new base can be used. If this is not the case
//! the older base is not changed.
// **************************************************************
bool Forth::changeDisplayBase(const uint8_t base)
{
  if ((base >= 2) && (base <= 36))
    {
      m_base = base;
      return true;
    }
  return false;
}

// **************************************************************
//! Used for trace and debug.
// **************************************************************
void Forth::displayDStack() const
{
  int32_t depth = m_dsp - m_data_stack;

  for (int32_t s = 0; s < depth; ++s)
    {
      std::cout << std::setbase(m_base) << m_data_stack[s] << ' ';
    }
  std::cout << std::endl;
}

// **************************************************************
//! Used for trace and debug.
// **************************************************************
void Forth::displayRStack() const
{
  int32_t depth = m_rsp - m_return_stack;

  for (int32_t s = 0; s < depth; ++s)
    {
      std::cout << std::hex << m_return_stack[s] << ' ';
    }
  std::cout << std::endl;
}

// **************************************************************
//! \param word the Forth name to store in the dictionary.
// **************************************************************
void Forth::create(std::string const& word)
{
  if (m_dico.exists(word))
    {
      std::cout << YELLOW << "[WARNING] Redefining '" << word << "'" << DEFAULT << std::endl;
    }
  else
    {
      if (m_trace) {
        std::cout << "Append new word definition '" << word << "' in dictionary" << std::endl;
      }
    }

  // Save informations which will be checked
  // when executing the SEMI_COLON primitive.
  m_creating_word = word;
  m_last_at_colon = m_dico.m_last;
  m_here_at_colon = m_dico.m_here;
  m_depth_at_colon = DStackDepth();

  // Add it in the dictionary
  Cell16 token = m_dico.here() + word.size() + 3U; // 3U: flags + NFA
  m_dico.add(token, word, 0);
}

// **************************************************************
//! \param tx token to execute (either a Forth primitive or a user
//! word definition).
//! \throw ForthStackOV if the data or the return stack overflowed.
// **************************************************************
void Forth::execToken(const Cell16 tx)
{
  int32_t depth;
  Cell16 token = tx;
  m_ip = 65535U;

  if (m_trace) {
    std::cout << "================ Etat initial ==================" << std::endl;
    std::cout << "DStack: "; displayDStack();
    std::cout << "RStack: "; displayRStack();
    std::cout << "Execute token " << std::hex << token << std::endl << std::endl;
  }

  // Always eat the top of the stack and store the value in a working register.
  DPOP(m_tos);

  // Traverse the word definition like a tree
  do
    {
      // A non primitive word is a consecutive set of primitive identifier
      while (!isPrimitive(token))
        {
          /* std::pair<bool, int32_t> res = m_dico.find(token);
          if (!res.first)
            {
              // FIXME: changer le message
              ForthUnknownPrimitive e(token, __PRETTY_FUNCTION__); throw e;
            } */

          if (m_trace) {
            std::cout << "Token "; m_dico.displayToken(token);
            std::cout << " is not a primitive " << std::endl;
          }

          // Save the next token to exec after the end of the definition
          Cell32 c = m_ip; RPUSH(c);
          if (m_trace) {
            std::cout << "PUSH "; m_dico.displayToken(m_ip);
            std::cout << std::endl;
          }

          m_ip = m_dico.read16at(token);
          m_ip += 2U;
          token = m_dico.read16at(m_ip);
          if (m_trace) {
            std::cout << "Next token is "; m_dico.displayToken(token);
            std::cout << std::endl << std::endl;
          }
        }

      //
      if (m_trace) {
        std::cout << "Token "; m_dico.displayToken(token);
        std::cout <<" is a primitive. Consum it"
                  << std::endl;
      }
      execPrimitive(token);

      if (m_trace) {
        DPUSH(m_tos); std::cout << "DStack: ";
        displayDStack(); DPOP(m_tos);
        std::cout << "RStack: "; displayRStack();
        std::cout << std::endl;
      }

      // Data stack overflow ?
      depth = DStackDepth();
      if (depth < -1)
        {
          ForthStackOV e(DATA_STACK); throw e;
        }

      // Do not forget than non-primitive words have the
      // token EXIT to pop the return stack to get back
      // IP value.
      if (65535U != m_ip)
        {
          m_ip += 2U;
          token = m_dico.read16at(m_ip);
          if (m_trace) {
            std::cout << "Next token is "; m_dico.displayToken(token);
            std::cout << std::endl;
          }
        }

      // Return stack overflow ?
      depth = RStackDepth();
      if (depth < 0)
        {
          ForthStackOV e(RETURN_STACK); throw e;
        }
      if (m_trace) {
        std::cout << "RStack: "; displayRStack();
      }
    } while (depth > 0);

  // Store the working register
  DPUSH(m_tos);

  if (m_trace) {
    std::cout << "================ Etat final ===================="
              << std::endl << std::endl;
  }
}

// **************************************************************
//!
// **************************************************************
void Forth::completion(std::string const& partial_name)
{
  std::pair<bool, char*> p;

  p = m_dico.completion(m_last_completion, partial_name);
  if (p.first)
    {
      std::cout << p.second << std::endl;
    }
  else
    {
      m_last_completion = dictionary().last();
    }
}

// **************************************************************
//! \param word (in) the number to convert
//! \param number (out) the result of the conversion if the function
//! returned true (else the number is undefined).
//! \return false if the number is malformed (not a number in the
//! current base or too huge to store in a cell.
//! \note: prefixes 'b', 'h' and '0x' are not Forth standard.
// **************************************************************
bool Forth::toNumber(std::string const& word, Cell32& number)
{
  int base = m_base;
  uint32_t negative = 0;
  uint32_t i = 0;

  // sign
  if ('-' == word[i])
    {
      ++i;
      negative = 1;
    }
  else if ('+' == word[i])
    {
      ++i;
    }

  // decimal
  if (('&' == word[i]) || ('#' == word[i]))
    {
      ++i;
      base = 10;
    }
  // binary ('b' is non standard)
  else if (('b' == word[i]) || ('%' == word[i]))
    {
      ++i;
      base = 2;
    }
  // hexadecimal ('h' is non standard)
  else if (('h' == word[i]) || ('$' == word[i]))
    {
      ++i;
      base = 16;
      if (m_base >= 33)
        return false;
    }
  // hexadecimal, if base < 33.
  else if (('0' == word[i]) && ('x' == word[i + 1]))
    {
      if (m_base < 33)
        {
          i += 2U;
          base = 16;
        }
      else
        {
          return false;
        }
    }
  // numeric value (e.g., ASCII code) an optional ' may be present after the character
  else if ('\'' == word[i])
    {
      int s = word.size();
      if ((2 == s) || ((3 == s) && ('\'' == word[i + 2])))
        {
          number = (negative ? -word[i + 1] : word[i + 1]);
          return true;
        }
      return false;
    }

  // Try to convert the string into number
  try
    {
      std::size_t sz;
      unsigned long val = std::stoul(word.substr(i, word.length() - i), &sz, base);
      number = (negative ? -val : val);
      return (sz + i) == word.length();
    }
  catch (const std::invalid_argument& ia)
    {
      return false;
    }
  catch (const std::out_of_range& oor)
    {
      // Two strategies:
#if (FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE == FORTH_TRUNCATE_OUT_OF_RANGE_NUMBERS)
      {
        // 1st strategy: runcate the number with a warning message to
        // avoid abort the program.
        number = (negative ? LONG_MIN : LONG_MAX);

        std::pair<size_t, size_t> p = READER.cursors();
        std::cerr << YELLOW << "[WARNING] " << READER.file() << ":"
                  << p.first << ":" << p.second
                  << " Out of range number '" + word + "' will be truncated"
                  << DEFAULT << std::endl;
        return true;
      }
#else // FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE == FORTH_OUT_OF_RANGE_NUMBERS_ARE_WORDS
      {
        // 2nd strategy: do not consider the word as a number which
        // probably result an "unknown word" error message.
        return false;
      }
#endif // FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE
    }
}

// **************************************************************
//! \param word the Forth word extracted from the stream.
//! \throw ForthUnknownWord if the word does not exist in the
//! dictionary and is not a number.
// **************************************************************
void Forth::interpreteWord(std::string const& word)
{
  Cell32 number;
  Cell16 token;
  bool immediate;

  if (INTERPRETER_STATE == m_state)
    {
      if (m_dico.find(word, token, immediate))
        {
          if (m_trace) std::cout << std::endl << "Execute word '" << word << "'" << std::endl;
          execToken(token); // FIXME: ce serait cool de pouvoir stocker le nombre de param dans la def du mot
        }
      else if (toNumber(word, number))
        {
          if (m_trace) std::cout << "PUSH number " << word << std::endl;
          DPUSH(number);
        }
      else
        {
          ForthUnknownWord e(word); throw e;
        }
    }
  else if (COMPILATION_STATE == m_state)
    {
      if (m_dico.find(word, token, immediate))
        {
          if (immediate)
            {
              if (m_trace) std::cout << std::endl << "Execute immediate word '" << word << "'" << std::endl;
              execToken(token);
            }
          else
            {
              if (m_trace) std::cout << "Append new word '" << word << "' in dictionary" << std::endl;
              m_dico.appendCell16(token);
            }
        }
      else if (toNumber(word, number))
        {
          if (m_trace) std::cout << "Append literal " << number << "' in dictionary" << std::endl;
          if (number <= 65535U)
            {
              m_dico.appendCell16(FORTH_PRIMITIVE_LITERAL_16);
              m_dico.appendCell16(number);
            }
          else
            {
              m_dico.appendCell16(FORTH_PRIMITIVE_LITERAL_32);
              m_dico.appendCell32(number);
            }
        }
      else
        {
          ForthUnknownWord e(word); throw e;
        }
    }
  else // COMMENT_STATE == m_state
    {
      // Nothing to do ignore the word except end of comment
      if (0 == word.compare(")"))
        {
          execToken(FORTH_PRIMITIVE_RPARENTHESIS);
        }
    }
}

// **************************************************************
//! \param filename the file name of the Forth script (ascii file)
//! to interprete.
//! \return a pair: a boolean indicating if the file has been
//! correctly interpreted (no error) and a string containing the
//! result of the interpreter.
// **************************************************************
std::pair<bool, std::string> Forth::interpreteFile(std::string const& filename)
{
  if (m_trace) std::cout << "eating File " << filename << std::endl;
  if (READER.setFileToParse(filename))
    {
      return parseStream();
    }
  else
    {
      return std::make_pair(false, "failed opening this file");
    }
}

// **************************************************************
//! \param code_forth the forth script stored as char*.
//! \return See Forth::interpreteFile
// FIXME: convert char* --> string can be consumns lot of memory ?!
// **************************************************************
std::pair<bool, std::string> Forth::interpreteString(const char* const code_forth)
{
  return Forth::interpreteString(std::string(code_forth));
}

// **************************************************************
//! \param code_forth the forth script stored as string.
//! \return See Forth::interpreteFile
// **************************************************************
std::pair<bool, std::string> Forth::interpreteString(std::string const& code_forth)
{
  READER.setStringToParse(code_forth);
  return parseStream();
}

// **************************************************************
//! \return See Forth::interpreteFile
// **************************************************************
std::pair<bool, std::string> Forth::parseStream()
{
  std::string word;

  try
    {
      // FIXME: le stream peut ne pas etre termine: attendre
      // FIXME: retourne ok si on lui donne une ligne vide dans un string
      while (READER.hasMoreWords())
        {
          interpreteWord(READER.nextWord());
        }

      // TODO: checker les piles
      #warning " Warning: stack depth changed during include!"

      // Check Forth state shall be in execute mode at the end of the
      // stream.  Else this means the end of the stream is truncated
      // with a non finished function definition or non finished
      // commentary.
      if (INTERPRETER_STATE != m_state)
        {
          // FIXME: the stream colum information is erroneous because
          // a ForthReader::refill() has been called before. But I like
          // this !
          ForthTruncatedStream e(m_state); throw e;
        }
      return std::make_pair(true, "ok");
    }

  catch (ForthException const& e)
    {
      return std::make_pair(false, e.what());
    }
}

// **************************************************************
//! \param res See Forth::interpreteFile
// **************************************************************
void Forth::ok(std::pair<bool, std::string> const& res)
{
  if (res.first)
    {
      std::cout << "    " << res.second << std::endl;
    }
  else
    {
      std::pair<size_t, size_t> p = READER.cursors();
      std::cerr << RED << "[ERROR] Ambiguous condition from "
                << READER.file() << ':'
                << p.first << ':'
                << p.second << ' '
                << res.second << DEFAULT << std::endl;
      abort(); // FIXME bad location
    }
}

// **************************************************************
//! Forth code: INCLUDE filename.fs
//! Note: this is an indirect recursive function trhough functions
//! eatFile() which can call this routine again through the function
//! execPrimitive() if the Forth word INCLUDE is found.
//! \param filename the new stream to parse.
//! \throw ForthException if too many streams are opened or if a
//! included file contains an error detected by the interpretor.
// **************************************************************
void Forth::includeFile(std::string const& filename)
{
  // FIXME memoriser m_base
  // Save the current stream in a stack
  ++m_opened_streams;

  // Reached maximum depth of include file including file including ... etc
  if (m_opened_streams >= MAX_OPENED_STREAMS)
    {
      ForthException e("too many opened streams");
      throw e;
    }

  // Parse the included file (recursive)
  std::pair<bool, std::string> res = interpreteFile(filename);
  if (res.first)
    {
      // Succes
      res.second += " parsed ";
      res.second += filename;
      ok(res);

      // Pop the previous stream
      assert(m_opened_streams > 0);
      --m_opened_streams;
    }
  else
    {
      // Compilation failure.
      // Concat the message error from the previous stream
      // given
      std::pair<size_t, size_t> p = READER.cursors();
      std::string msg("\n        including " + READER.file() + ':'
                      + std::to_string(p.first) + ':'
                      + std::to_string(p.second) + ' '
                      + res.second);

      // Pop the previous stream
      --m_opened_streams;

      // Call exception that will be caugh by the parseStream()
      ForthException e(msg);
      throw e;
    }
}

// **************************************************************
//! This method should be called after the contructor Forth().
//! boot() has been separated from the constructor to load the
//! dictionary at the desired moment.
// **************************************************************
void Forth::boot()
{
  // TODO: Les ranger par ordre lexico ?
  // FIXME: init m_last et m_here pour etre sur que le client ne
  // charge pas plusieurs fois les memes primitives

  // Dummy word and comments
  m_dico.add(FORTH_PRIMITIVE_NOP, "NOOP", 0);
  m_dico.add(FORTH_PRIMITIVE_LPARENTHESIS, "(", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_RPARENTHESIS, ")", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_COMMENTARY, "\\", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_INCLUDE, "INCLUDE", 0);

  // Words for definitions
  m_dico.add(FORTH_PRIMITIVE_COLON, ":", 0);
  m_dico.add(FORTH_PRIMITIVE_SEMICOLON, ";", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_PCREATE, "(CREATE)", 0);
  m_dico.add(FORTH_PRIMITIVE_CREATE, "CREATE", 0);
  m_dico.add(FORTH_PRIMITIVE_BUILDS, "<BUILDS", 0);
  m_dico.add(FORTH_PRIMITIVE_DOES, "DOES>", FLAG_IMMEDIATE);

  m_dico.add(FORTH_PRIMITIVE_IMMEDIATE, "IMMEDIATE", 0);
  m_dico.add(FORTH_PRIMITIVE_SMUDGE, "SMUDGE", 0);
  m_dico.add(FORTH_PRIMITIVE_STATE, "STATE", 0);
  m_dico.add(FORTH_PRIMITIVE_TRACE_ON, "TRACE.ON", 0);
  m_dico.add(FORTH_PRIMITIVE_TRACE_OFF, "TRACE.OFF", 0);

  // Words
  m_dico.add(FORTH_PRIMITIVE_TICK, "'", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_EXECUTE, "EXECUTE", 0);
  m_dico.add(FORTH_PRIMITIVE_COMPILE, "COMPILE", 0);
  m_dico.add(FORTH_PRIMITIVE_ICOMPILE, "[COMPILE]", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_POSTPONE, "POSTPONE", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_LBRACKET, "[", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_RBRACKET, "]", 0);

  // Dictionnary manipulation
  m_dico.add(FORTH_PRIMITIVE_LAST, "LAST", 0);
  m_dico.add(FORTH_PRIMITIVE_HERE, "HERE", 0);
  m_dico.add(FORTH_PRIMITIVE_ALLOT, "ALLOT", 0);
  m_dico.add(FORTH_PRIMITIVE_COMMA32, ",", 0);
  m_dico.add(FORTH_PRIMITIVE_COMMA16, "S,", 0);
  m_dico.add(FORTH_PRIMITIVE_COMMA8, "C,", 0);
  m_dico.add(FORTH_PRIMITIVE_FETCH, "@", 0);
  m_dico.add(FORTH_PRIMITIVE_STORE32, "!", 0);
  m_dico.add(FORTH_PRIMITIVE_STORE16, "S!", 0);
  m_dico.add(FORTH_PRIMITIVE_STORE8, "C!", 0);
  m_dico.add(FORTH_PRIMITIVE_CMOVE, "CMOVE", 0);

  // Words changing IP
  m_dico.add(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
  m_dico.add(FORTH_PRIMITIVE_BRANCH, "BRANCH", 0);
  m_dico.add(FORTH_PRIMITIVE_0BRANCH, "0BRANCH", 0);

  // Return Stack
  m_dico.add(FORTH_PRIMITIVE_TO_RSTACK, ">R", 0);
  m_dico.add(FORTH_PRIMITIVE_FROM_RSTACK, "R>", 0);
  m_dico.add(FORTH_PRIMITIVE_2TO_RSTACK, "2>R", 0);
  m_dico.add(FORTH_PRIMITIVE_2FROM_RSTACK, "2R>", 0);
  m_dico.add(FORTH_PRIMITIVE_I, "I", 0);
  m_dico.add(FORTH_PRIMITIVE_J, "J", 0);

  // cell sizeof
  m_dico.add(FORTH_PRIMITIVE_CELL, "CELL", 0);
  m_dico.add(FORTH_PRIMITIVE_CELLS, "CELLS", 0);

  // Literals
  m_dico.add(FORTH_PRIMITIVE_LITERAL_16, "LITERAL16", 0);
  m_dico.add(FORTH_PRIMITIVE_LITERAL_32, "LITERAL32", 0);

  // Arithmetic
  m_dico.add(FORTH_PRIMITIVE_ABS, "ABS", 0);
  m_dico.add(FORTH_PRIMITIVE_NEGATE, "NEGATE", 0);
  m_dico.add(FORTH_PRIMITIVE_MIN, "MIN", 0);
  m_dico.add(FORTH_PRIMITIVE_MAX, "MAX", 0);
  m_dico.add(FORTH_PRIMITIVE_PLUS, "+", 0);
  m_dico.add(FORTH_PRIMITIVE_1PLUS, "1+", 0);
  m_dico.add(FORTH_PRIMITIVE_2PLUS, "2+", 0);
  m_dico.add(FORTH_PRIMITIVE_MINUS, "-", 0);
  m_dico.add(FORTH_PRIMITIVE_1MINUS, "1-", 0);
  m_dico.add(FORTH_PRIMITIVE_2MINUS, "2-", 0);
  m_dico.add(FORTH_PRIMITIVE_TIMES, "*", 0);
  m_dico.add(FORTH_PRIMITIVE_DIV, "/", 0);
  m_dico.add(FORTH_PRIMITIVE_RSHIFT, ">>", 0);
  m_dico.add(FORTH_PRIMITIVE_LSHIFT, "<<", 0);
  m_dico.add(FORTH_PRIMITIVE_RSHIFT, "RSHIFT", 0);
  m_dico.add(FORTH_PRIMITIVE_LSHIFT, "LSHIFT", 0);

  // Base
  m_dico.add(FORTH_PRIMITIVE_BINARY, "BIN", 0);
  m_dico.add(FORTH_PRIMITIVE_OCTAL, "OCTAL", 0);
  m_dico.add(FORTH_PRIMITIVE_HEXADECIMAL, "HEX", 0);
  m_dico.add(FORTH_PRIMITIVE_DECIMAL, "DECIMAL", 0);
  m_dico.add(FORTH_PRIMITIVE_BASE, "BASE", 0);

  // Logic
  m_dico.add(FORTH_PRIMITIVE_FALSE, "FALSE", 0);
  m_dico.add(FORTH_PRIMITIVE_TRUE, "TRUE", 0);
  m_dico.add(FORTH_PRIMITIVE_GREATER, ">", 0);
  m_dico.add(FORTH_PRIMITIVE_GREATER_EQUAL, ">=", 0);
  m_dico.add(FORTH_PRIMITIVE_LOWER, "<", 0);
  m_dico.add(FORTH_PRIMITIVE_LOWER_EQUAL, "<=", 0);
  m_dico.add(FORTH_PRIMITIVE_EQUAL, "==", 0);
  m_dico.add(FORTH_PRIMITIVE_EQUAL, "=", 0);
  m_dico.add(FORTH_PRIMITIVE_0EQUAL, "0=", 0);
  m_dico.add(FORTH_PRIMITIVE_NOT_EQUAL, "<>", 0);
  m_dico.add(FORTH_PRIMITIVE_NOT_EQUAL, "!=", 0);
  m_dico.add(FORTH_PRIMITIVE_AND, "AND", 0);
  m_dico.add(FORTH_PRIMITIVE_OR, "OR", 0);
  m_dico.add(FORTH_PRIMITIVE_XOR, "XOR", 0);

  // Data Stack
  m_dico.add(FORTH_PRIMITIVE_DEPTH, "DEPTH", 0);
  m_dico.add(FORTH_PRIMITIVE_ROLL, "ROLL", 0);
  m_dico.add(FORTH_PRIMITIVE_NIP, "NIP", 0);
  m_dico.add(FORTH_PRIMITIVE_PICK, "PICK", 0);
  m_dico.add(FORTH_PRIMITIVE_DUP, "DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_QDUP, "?DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_DROP, "DROP", 0);
  m_dico.add(FORTH_PRIMITIVE_SWAP, "SWAP", 0);
  m_dico.add(FORTH_PRIMITIVE_OVER, "OVER", 0);
  m_dico.add(FORTH_PRIMITIVE_ROT, "ROT", 0);
  m_dico.add(FORTH_PRIMITIVE_TUCK, "TUCK", 0);
  m_dico.add(FORTH_PRIMITIVE_2DUP, "2DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_2DROP, "2DROP", 0);
  m_dico.add(FORTH_PRIMITIVE_2SWAP, "2SWAP", 0);
  m_dico.add(FORTH_PRIMITIVE_2OVER, "2OVER", 0);

  // Printf
  m_dico.add(FORTH_PRIMITIVE_DISP, ".", 0);
  m_dico.add(FORTH_PRIMITIVE_UDISP, "U.", 0);
  m_dico.add(FORTH_PRIMITIVE_CARRIAGE_RETURN, "CR", 0);
  m_dico.add(FORTH_PRIMITIVE_DISPLAY_DSTACK, ".S", 0);

  // Hide some words to user
  m_dico.smudge("(CREATE)");
}

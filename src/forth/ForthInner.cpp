#include "ForthInner.hpp"
#include <limits.h>

// **************************************************************
// Constructor: initialize the context but do not boot
// **************************************************************
Forth::Forth()
{
  m_state = EXECUTION_STATE;
  m_dsp = m_data_stack;
  m_asp = m_alternative_stack;
  m_rsp = m_return_stack;
  m_base = 10U;
}

// **************************************************************
//
// **************************************************************
inline void Forth::restore()
{
  if ((COMPILATION_STATE == m_state) ||
      ((COMMENT_STATE == m_state) && (COMPILATION_STATE == m_saved_state)))
    {
      m_dico.m_last = m_last_at_colon;
      m_dico.m_here = m_here_at_colon;
    }

   m_state = EXECUTION_STATE;
   m_dsp = m_data_stack;
   m_asp = m_alternative_stack;
   m_rsp = m_return_stack;
}

// **************************************************************
//
// **************************************************************
const ForthDico& Forth::dictionary() const
{
  return m_dico;
}

// **************************************************************
//
// **************************************************************
ForthDico& Forth::dictionary()
{
  return m_dico;
}

// **************************************************************
// When displaying numbers on screen, display them on the
// selected base.
// **************************************************************
bool Forth::changeDisplayBase(const uint8_t newbase)
{
  if ((newbase >= 2) && (newbase <= 36))
    {
      m_base = newbase;
      return true;
    }
  return false;
}

// **************************************************************
// Check if the token is a forth primitive
// **************************************************************
inline bool Forth::isPrimitive(const Cell16 id) const
{
  return /*(id >= 0) &&*/ (id < NUM_PRIMITIVES);
}

// **************************************************************
// Return the number of elements in the return stack
// **************************************************************
int32_t Forth::RStackDepth() const
{
  return m_rsp - m_return_stack;
}

// **************************************************************
// Return the number of elements in the data stack
// **************************************************************
int32_t Forth::DStackDepth() const
{
  return m_dsp - m_data_stack;
}

// **************************************************************
//
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
//
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
void Forth::execToken(const Cell16 tx)
{
  int32_t depth;
  Cell16 token = tx;
  m_ip = 65535U;

  //std::cout << "AVANT " << m_dsp - data_stack << std::endl;

  // Always eat the top of the stack and store the value in a working register.
  DPOP(m_tos);

  // Traverse the word definition like a tree
  do
    {
      // A non primitive word is a consecutive set of primitive identifier
      while (!isPrimitive(token))
        {
          // Save the next token to exec after the end of the definition
          Cell32 c = m_ip; RPUSH(c);
          //std::cout << "PUSH "; m_dico.displayToken(m_ip); std::cout << std::endl;

          m_ip = m_dico.read16at(token);
          m_ip += 2U;
          token = m_dico.read16at(m_ip);
        }

      //
      execPrimitive(token);

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
        }

      // Return stack overflow ?
      depth = RStackDepth();
      if (depth < 0)

        {
          ForthStackOV e(RETURN_STACK); throw e;
        }
    } while (depth > 0);

  // Store the working register
  DPUSH(m_tos);
  //std::cout << "APRES " << m_dsp - data_stack << std::endl;
}

// **************************************************************
// Convert a string to a number.
// Note prefixes 'b', 'h' and '0x' are not standard
// Return false if an error occured
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

        std::pair<size_t, size_t> p = m_reader.cursors();
        std::cerr << YELLOW << "[WARNING] " << m_reader.file() << ":"
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
// COMPILATION_STATE
// **************************************************************
void Forth::interprete(std::string const& word)
{
  Cell32 number;
  Cell16 token;
  bool immediate;

  if (EXECUTION_STATE == m_state)
    {
      if (m_dico.find(word, token, immediate))
        {
          //std::cout << "Forth::interprete '" << word << "' EXEC WORD\n";
          execToken(token); // FIXME: ce serait cool de pouvoir stocker le nombre de param dans la def du mot
        }
      else if (toNumber(word, number))
        {
          //std::cout << "Forth::interprete '" << word << "' EXEC NUMBER\n";
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
              //std::cout << "Forth::interprete '" << word << "' COMP + IMM" << std::endl;
              execToken(token);
            }
          else
            {
              //std::cout << "Forth::interprete '" << word << "' COMP -> append dico\n";
              m_dico.appendCell16(token);
            }
        }
      else if (toNumber(word, number))
        {
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
//
// **************************************************************
std::pair<bool, std::string> Forth::eatFile(std::string const& filename)
{
  if (m_reader.setFileToParse(filename))
    {
      return parseStream();
    }
  else
    {
      return std::make_pair(false, "failed opening this file");
    }
}

// **************************************************************
// FIXME: convert char* --> string can be consumns lot of memory ?!
// **************************************************************
std::pair<bool, std::string> Forth::eatString(const char* const code_forth)
{
  return Forth::eatString(std::string(code_forth));
}

// **************************************************************
//
// **************************************************************
std::pair<bool, std::string> Forth::eatString(std::string const& code_forth)
{
  m_reader.setStringToParse(code_forth);
  std::cout << code_forth << std::endl;
  return parseStream();
}

// **************************************************************
//
// **************************************************************
std::pair<bool, std::string> Forth::parseStream()
{
  std::string word;

  try
    {
      // FIXME: le stream peut ne pas etre termine: attendre
      // FIXME: retourne ok si on lui donne une ligne vide dans un string
      while (m_reader.hasMoreWords())
        {
          interprete(m_reader.nextWord());
        }

      // Check Forth state shall be in execute mode at the end of the
      // stream.  Else this means the end of the stream is truncated
      // with a non finished function definition or non finished
      // commentary.
      if (EXECUTION_STATE != m_state)
        {
          // FIXME: the stream colum information is erroneous because
          // a ForthReader::refill() has been called before. But I like
          // this !
          ForthTruncatedStream e(m_state); throw e;
        }
      return std::make_pair(true, "ok");
    }

  catch (std::exception const& e)
    {
      restore();
      return std::make_pair(false, e.what());
    }
}

// **************************************************************
//
// **************************************************************
void Forth::ok(std::pair<bool, std::string> const& res)
{
  if (res.first)
    {
      std::cout << "    " << res.second << std::endl;
    }
  else
    {
      std::pair<size_t, size_t> p = m_reader.cursors();
      std::cerr << RED << "[ERROR] " << m_reader.file() << ":"
                << p.first << ":" << p.second << ": "
                << res.second << DEFAULT << std::endl;
    }
}

// **************************************************************
// TODO: Les ranger par ordre lexico ?
// **************************************************************
void Forth::boot()
{
  // Dummy word and comments
  m_dico.add(FORTH_PRIMITIVE_NOP, "NOP", 0);
  m_dico.add(FORTH_PRIMITIVE_LPARENTHESIS, "(", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_RPARENTHESIS, ")", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_COMMENTARY, "\\", FLAG_IMMEDIATE);

  // Words for definitions
  m_dico.add(FORTH_PRIMITIVE_COLON, ":", 0);
  m_dico.add(FORTH_PRIMITIVE_SEMICOLON, ";", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_IMMEDIATE, "IMMEDIATE", 0);
  m_dico.add(FORTH_PRIMITIVE_SMUDGE, "SMUDGE", 0);

  // Words
  m_dico.add(FORTH_PRIMITIVE_TICK, "'", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_EXECUTE, "EXECUTE", 0);
  m_dico.add(FORTH_PRIMITIVE_COMPILE, "COMPILE", 0);
  m_dico.add(FORTH_PRIMITIVE_LBRACKET, "[", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_RBRACKET, "]", 0);

  // Dictionnary manipulation
  m_dico.add(FORTH_PRIMITIVE_HERE, "HERE", 0);
  m_dico.add(FORTH_PRIMITIVE_ALLOT, "ALLOT", 0);
  m_dico.add(FORTH_PRIMITIVE_COMMA, ",", 0);
  m_dico.add(FORTH_PRIMITIVE_FETCH, "@", 0);
  m_dico.add(FORTH_PRIMITIVE_STORE, "!", 0);

  // Words changing IP
  m_dico.add(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
  m_dico.add(FORTH_PRIMITIVE_BRANCH, "BRANCH", 0);
  m_dico.add(FORTH_PRIMITIVE_0BRANCH, "0BRANCH", 0);

  // Return Stack
  m_dico.add(FORTH_PRIMITIVE_TO_RSTACK, ">R", 0);
  m_dico.add(FORTH_PRIMITIVE_FROM_RSTACK, "R>", 0);
  m_dico.add(FORTH_PRIMITIVE_2TO_RSTACK, "2>R", 0);
  m_dico.add(FORTH_PRIMITIVE_2FROM_RSTACK, "2R>", 0);

  // cell sizeof
  m_dico.add(FORTH_PRIMITIVE_CELL, "CELL", 0);
  m_dico.add(FORTH_PRIMITIVE_CELL, "CELLS", 0);

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
  m_dico.add(FORTH_PRIMITIVE_NIP, "NIP", 0);
  m_dico.add(FORTH_PRIMITIVE_PICK, "PICK", 0);
  m_dico.add(FORTH_PRIMITIVE_DUP, "DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_QDUP, "?DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_DROP, "DROP", 0);
  m_dico.add(FORTH_PRIMITIVE_SWAP, "SWAP", 0);
  m_dico.add(FORTH_PRIMITIVE_OVER, "OVER", 0);
  m_dico.add(FORTH_PRIMITIVE_ROT, "ROT", 0);
  m_dico.add(FORTH_PRIMITIVE_TUK, "TUK", 0);
  m_dico.add(FORTH_PRIMITIVE_2DUP, "2DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_2DROP, "2DROP", 0);
  m_dico.add(FORTH_PRIMITIVE_2SWAP, "2SWAP", 0);
  m_dico.add(FORTH_PRIMITIVE_2OVER, "2OVER", 0);

  // Printf
  m_dico.add(FORTH_PRIMITIVE_DISP, ".", 0);
  m_dico.add(FORTH_PRIMITIVE_CARRIAGE_RETURN, "CR", 0);
  m_dico.add(FORTH_PRIMITIVE_DISPLAY_DSTACK, ".S", 0);
}

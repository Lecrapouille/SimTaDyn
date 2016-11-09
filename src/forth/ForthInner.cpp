#include "ForthInner.hpp"
#include <limits.h>

// **************************************************************
// Constructor: initialize the context but do not boot
// **************************************************************
Forth::Forth()
{
  m_state = EXECUTION_STATE;
  m_dsp = data_stack;
  m_rsp = return_stack;
  m_base = 10U;
}

// **************************************************************
//
// **************************************************************
inline void Forth::restore()
{
   if (COMPILATION_STATE == m_state)
     {
       m_dico.m_last = m_last_at_colon;
       m_dico.m_here = m_here_at_colon;
     }

   m_state = EXECUTION_STATE;
   m_dsp = data_stack;
   m_rsp = return_stack;
}

// **************************************************************
//
// **************************************************************
const ForthDico& Forth::dictionary() const
{
  return m_dico;
}

// **************************************************************
// When displaying numbers on screen, display them on the
// selected base.
// **************************************************************
inline void Forth::changeDisplayBase(const uint8_t newbase)
{
  m_base = newbase;
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
  return m_rsp - return_stack;
}

// **************************************************************
// Return the number of elements in the data stack
// **************************************************************
int32_t Forth::DStackDepth() const
{
  return m_dsp - data_stack;
}

// **************************************************************
//
// **************************************************************
void Forth::execToken(const Cell16 tx)
{
  int32_t depth;
  Cell16 token = tx;
  m_ip = nullptr;

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
          RPUSH(m_ip);

          m_ip = m_dico.ip(token);
          //checkDicoBoundaries(m_ip, "execToken");
          m_ip++;
          token = m_dico.read16at(ADDR8(m_ip));
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
      if (nullptr != m_ip)
        {
          m_ip++;
          token = m_dico.read16at(ADDR8(m_ip));
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
      if (m_base < 33)
        return false;
    }
  // hexadecimal, if base<33.
  else if ('0' == word[i])
    {
      ++i;
      if (('x' == word[i + 1]) && (m_base < 33))
        {
          ++i;
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

      unsigned long val = std::stoul(word, &sz, base);
      number = (negative ? -val : val);
      return sz == word.size();
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
          //std::cout << "Forth::interprete '" << word << "' COMP NUMBER\n";
          // Optim si number < 65536 alors
          // m_dicoAppendCell16(FORTH_PRIMITIVE_LITERAL16)
          // m_dicoAppendCell16(number);
          m_dico.appendCell16(FORTH_PRIMITIVE_LITERAL); // _LITERAL32
          // align @32 ?
          m_dico.appendCell16(number / 65536U);
          m_dico.appendCell16(number & 65535U);
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
      // FIXME: retourne ok si on lui donne une ligne vide
      while (m_reader.hasMoreWords())
        {
          interprete(m_reader.nextWord());
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
  m_dico.add(FORTH_PRIMITIVE_LITERAL, "LITERAL", 0);

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

  // Logic
  m_dico.add(FORTH_PRIMITIVE_GREATER, ">", 0);
  m_dico.add(FORTH_PRIMITIVE_GREATER_EQUAL, ">=", 0);
  m_dico.add(FORTH_PRIMITIVE_LOWER, "<", 0);
  m_dico.add(FORTH_PRIMITIVE_LOWER_EQUAL, "<=", 0);
  m_dico.add(FORTH_PRIMITIVE_EQUAL, "==", 0);
  m_dico.add(FORTH_PRIMITIVE_EQUAL, "=", 0);
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
}

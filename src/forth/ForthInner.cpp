#include "ForthInner.hpp"

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
inline int32_t Forth::RStackDepth() const
{
  return m_rsp - return_stack;
}

// **************************************************************
// Return the number of elements in the data stack
// **************************************************************
inline int32_t Forth::DStackDepth() const
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

      // Data stack overflow ?
      depth = DStackDepth();
      if (depth < -1)
        {
          ForthStackOV e(DATA_STACK); throw e;
        }

      //
      execPrimitive(token);

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
}

// **************************************************************
// Convert a string to a number.
// Note prefixes 'b', 'h' and '0x' are not standard
// Return false if an error occured
// **************************************************************
bool Forth::toNumber(std::string const& word, Cell32& number)
{
  int base = m_base;
  int32_t sign = 1;
  int32_t i = 0;

  // sign
  if ('-' == word[i])
    {
      ++i;
      sign = -1;
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
          number = sign * word[i + 1];
          return true;
        }
      return false;
    }

  // Try to convert the string into number
  try
    {
      std::size_t sz;
      std::string str = word;//FIXME word.substr(word, i, std::string::npos);

      number = sign * std::stoi(str, &sz, base);
      return sz == word.size();
    }
  catch (const std::invalid_argument& ia)
    {
      return false;
    }
  catch (const std::out_of_range& oor)
    {
      return false;
    }
}

// **************************************************************
//COMPILATION_STATE
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
          execToken(token);
        }
      else if (toNumber(word, number))
        {
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
              execToken(token);
            }
          else
            {
              m_dico.appendCell16(token);
            }
        }
      else if (toNumber(word, number))
        {
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
      return std::make_pair(false, "[ERROR]: failed opening file"); // reader.strerrno());
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
      while (m_reader.nextWord(word))
        {
          interprete(word);
        }
      return std::make_pair(true, "ok");
    }

  catch (std::exception const& e)
    {
      // TODO: restore Forth context
      std::cerr << e.what() << std::endl;
      return std::make_pair(false, e.what());
    }
}

// **************************************************************
//
// **************************************************************
void Forth::boot()
{
  // Les ranger par ordre lexico ?
  m_dico.add(FORTH_PRIMITIVE_NOP, "NOP", 0);
  m_dico.add(FORTH_PRIMITIVE_LPARENTHESIS, "(", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_RPARENTHESIS, ")", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_COMMENTARY, "\\", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_COLON, ":", 0);
  m_dico.add(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
  m_dico.add(FORTH_PRIMITIVE_SEMICOLON, ";", FLAG_IMMEDIATE);
  m_dico.add(FORTH_PRIMITIVE_LITERAL, "LITERAL", 0);
  m_dico.add(FORTH_PRIMITIVE_1MINUS, "1-", 0);
  m_dico.add(FORTH_PRIMITIVE_1PLUS, "1+", 0);
  m_dico.add(FORTH_PRIMITIVE_2MINUS, "2-", 0);
  m_dico.add(FORTH_PRIMITIVE_2PLUS, "2+", 0);

  m_dico.add(FORTH_PRIMITIVE_NIP, "NIP", 0);
  m_dico.add(FORTH_PRIMITIVE_DUP, "DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_DROP, "DROP", 0);
  m_dico.add(FORTH_PRIMITIVE_SWAP, "SWAP", 0);
  m_dico.add(FORTH_PRIMITIVE_OVER, "OVER", 0);
  m_dico.add(FORTH_PRIMITIVE_ROT, "ROT", 0);
  m_dico.add(FORTH_PRIMITIVE_TUK, "TUK", 0);
  m_dico.add(FORTH_PRIMITIVE_2DUP, "2DUP", 0);
  m_dico.add(FORTH_PRIMITIVE_2DROP, "2DROP", 0);
  m_dico.add(FORTH_PRIMITIVE_2SWAP, "2SWAP", 0);
  m_dico.add(FORTH_PRIMITIVE_2OVER, "2OVER", 0);

  m_dico.add(FORTH_PRIMITIVE_PLUS, "+", 0);
  m_dico.add(FORTH_PRIMITIVE_MINUS, "-", 0);
  m_dico.add(FORTH_PRIMITIVE_DIV, "/", 0);
  m_dico.add(FORTH_PRIMITIVE_TIMES, "*", 0);
  m_dico.add(FORTH_PRIMITIVE_RSHIFT, ">>", 0);
  m_dico.add(FORTH_PRIMITIVE_LSHIFT, "<<", 0);
  m_dico.add(FORTH_PRIMITIVE_GREATER_EQUAL, ">=", 0);
  m_dico.add(FORTH_PRIMITIVE_LOWER_EQUAL, "<=", 0);
  m_dico.add(FORTH_PRIMITIVE_GREATER, ">", 0);
  m_dico.add(FORTH_PRIMITIVE_LOWER, "<", 0);
  m_dico.add(FORTH_PRIMITIVE_EQUAL, "==", 0);
  m_dico.add(FORTH_PRIMITIVE_AND, "AND", 0);
  m_dico.add(FORTH_PRIMITIVE_OR, "OR", 0);
  m_dico.add(FORTH_PRIMITIVE_XOR, "XOR", 0);
  m_dico.add(FORTH_PRIMITIVE_DISP, ".", 0);
}

#include "Forth.hpp"

// **************************************************************
// Constructor: initialize the context but do not boot
// **************************************************************
Forth::Forth()
{
  state = EXECUTION_STATE;
  dsp = data_stack;
  rsp = return_stack;
  here = 0U;
  last = 0U;
  changeDisplayBase(10);
}

// **************************************************************
// When displaying numbers on screen, display them on the
// selected base.
// **************************************************************
inline void Forth::changeDisplayBase(const uint8_t newbase)
{
  base = newbase;
}

// **************************************************************
//
// **************************************************************
inline bool Forth::isPrimitive(const Cell16 id) const
{
  //std::cout << "isPrimitive " << (uint32_t) id << " " << (id < NUM_PRIMITIVES) << std::endl;
  return /*(id >= 0) &&*/ (id < NUM_PRIMITIVES);
}

// **************************************************************
// Return the number of element in the return stack
// **************************************************************
inline int32_t Forth::RStackSize() const
{
  int32_t size = rsp - return_stack;
  //std::cout << "RSTACK SIZE " << size << std::endl;
  if (size < 0)
    {
      ForthRStackOV e; throw e;
    }
  return size;
}

// **************************************************************
//
// **************************************************************
#define TOKEN2ADDR(a) (CADDR(a + dictionary))

//((cell_t *) ( ((cell_t) a) + CodeBase) )


void Forth::execToken(const Cell16 tx)
{
  Cell16 token = tx;
  ip = NULL;

  //std::cout << "EXEC Token " << tx << std::endl;

  // Always eat the top of the stack and store the value in a working register.
  DPOP(tos);

  // Traverse the word definition like a tree
  do
    {
      // A non primitive word is a consecutive set of primitive identifier
      while (!isPrimitive(token))
        {
          //std::cout << "RPUSH(ip) " << (uint32_t*) ip << std::endl;
          RPUSH(ip);

          ip = TOKEN2ADDR(token);
          //checkDicoBoundaries(ip, "execToken");
          ip++;
          token = readCell16at(ADDR8(ip));
          //std::cout << "New token: " << token << std::endl;
        }

      //
      execPrimitive(token);

      // Do not forget than non-primitive words have the
      // token EXIT to pop the return stack to get back
      // IP value.
      if (NULL != ip)
        {
          //std::cout << "IP != NULL " << ip << std::endl;
          ip++;
          token = readCell16at(ADDR8(ip));
          //std::cout << "New token " << token << std::endl;
        }
    } while (RStackSize() > 0);

  // Store the working register
  DPUSH(tos);
}

// **************************************************************
// Convert a string to a number.
// Return false if an error occured
// **************************************************************
bool Forth::toNumber(std::string const& word, Cell32& number)
{
  int base = 10;

  if (word[0] == 'b')
    {
      base = 2;
    }
  else if (word[0] == 'h')
    {
      base = 16;
    }
  else if ((word[0] == '0') && (word[1] == 'x'))
    {
      base = 16;
    }

  try
    {
      std::size_t sz;

      number = std::stoi(word, &sz, base);
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
void Forth::eat(std::string const& word)
{
  Cell32 number;
  Cell16 token;
  Cell16 immediate;

  if (EXECUTION_STATE == state)
    {
      if (toToken(word, token, immediate))
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
  else // COMPILATION_STATE
    {
      if (toToken(word, token, immediate))
        {
          if (immediate)
            {
              execToken(token);
            }
          else
            {
              dicoAppendCell16(token);
            }
        }
      else if (toNumber(word, number))
        {;
          // Optim si number < 65536 alors
          // dicoAppendCell16(FORTH_PRIMITIVE_LITERAL16)
          // dicoAppendCell16(number);
          dicoAppendCell16(FORTH_PRIMITIVE_LITERAL); // _LITERAL32
          // align @32 ?
          dicoAppendCell16(number / 65536U);
          dicoAppendCell16(number & 65535U);
        }
      else
        {
          ForthUnknownWord e(word); throw e;
        }
    }
}


// **************************************************************
//
// **************************************************************
bool Forth::eatString(std::string const& code_forth)
{
  std::string word;

  try
    {
      reader.useForthString(code_forth);
      while (reader.getNextWord(word))
        {
          eat(word);
        }
      // TODO: (peut etre pas EOL) or EOF && toujours en mode compilation
      std::cout << "ok" << std::endl;
      return true;
    }
  catch(std::exception const& e)
    {
      std::cerr << "[ERROR]: " << e.what() << std::endl;
      // TODO: restore Forth context
      return false;
    }
}

// **************************************************************
//
// **************************************************************
bool Forth::eatFile(std::string const& filename)
{
  std::string word;
  bool res;

  try
    {
      res = reader.openForthFile(filename);
      if (res)
        {
          while (reader.getNextWord(word))
            {
              eat(word);
            }
          // TODO: (peut etre pas EOL) or EOF && toujours en mode compilation
          std::cout << "ok" << std::endl;
          return true;
        }
      return false;
    }
  catch(std::exception const& e)
    {
      std::cerr << "[ERROR]: " << e.what() << std::endl;
      // TODO: restore Forth context
      return false;
    }
}


// **************************************************************
//
// **************************************************************
void Forth::boot()
{
  // Les ranger par ordre lexico ?
  createDicoEntry(FORTH_PRIMITIVE_NOP, "NOP", 0);
  createDicoEntry(FORTH_PRIMITIVE_COLON, ":", 0);
  createDicoEntry(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
  createDicoEntry(FORTH_PRIMITIVE_SEMICOLON, ";", FLAG_IMMEDIATE);
  createDicoEntry(FORTH_PRIMITIVE_LITERAL, "LITERAL", 0);
  createDicoEntry(FORTH_PRIMITIVE_1MINUS, "1-", 0);
  createDicoEntry(FORTH_PRIMITIVE_1PLUS, "1+", 0);
  createDicoEntry(FORTH_PRIMITIVE_2MINUS, "2-", 0);
  createDicoEntry(FORTH_PRIMITIVE_2PLUS, "2+", 0);

  createDicoEntry(FORTH_PRIMITIVE_NIP, "NIP", 0);
  createDicoEntry(FORTH_PRIMITIVE_DUP, "DUP", 0);
  createDicoEntry(FORTH_PRIMITIVE_DROP, "DROP", 0);
  createDicoEntry(FORTH_PRIMITIVE_SWAP, "SWAP", 0);
  createDicoEntry(FORTH_PRIMITIVE_OVER, "OVER", 0);
  createDicoEntry(FORTH_PRIMITIVE_ROT, "ROT", 0);
  createDicoEntry(FORTH_PRIMITIVE_TUK, "TUK", 0);
  createDicoEntry(FORTH_PRIMITIVE_2DUP, "2DUP", 0);
  createDicoEntry(FORTH_PRIMITIVE_2DROP, "2DROP", 0);
  createDicoEntry(FORTH_PRIMITIVE_2SWAP, "2SWAP", 0);
  createDicoEntry(FORTH_PRIMITIVE_2OVER, "2OVER", 0);

  createDicoEntry(FORTH_PRIMITIVE_PLUS, "+", 0);
  createDicoEntry(FORTH_PRIMITIVE_MINUS, "-", 0);
  createDicoEntry(FORTH_PRIMITIVE_DIV, "/", 0);
  createDicoEntry(FORTH_PRIMITIVE_TIMES, "*", 0);
  createDicoEntry(FORTH_PRIMITIVE_RSHIFT, ">>", 0);
  createDicoEntry(FORTH_PRIMITIVE_LSHIFT, "<<", 0);
  createDicoEntry(FORTH_PRIMITIVE_GREATER_EQUAL, ">=", 0);
  createDicoEntry(FORTH_PRIMITIVE_LOWER_EQUAL, "<=", 0);
  createDicoEntry(FORTH_PRIMITIVE_GREATER, ">", 0);
  createDicoEntry(FORTH_PRIMITIVE_LOWER, "<", 0);
  createDicoEntry(FORTH_PRIMITIVE_EQUAL, "==", 0);
  createDicoEntry(FORTH_PRIMITIVE_AND, "AND", 0);
  createDicoEntry(FORTH_PRIMITIVE_OR, "OR", 0);
  createDicoEntry(FORTH_PRIMITIVE_XOR, "XOR", 0);
  createDicoEntry(FORTH_PRIMITIVE_DISP, ".", 0);
}

Forth forth;

int main()
{
  forth.boot();

  forth.eatString(": DOUBLE DUP + ;");
  forth.eatString(": NUM 42 ;");
  forth.eatString(": QUADRUPLE DOUBLE DOUBLE DUP . NUM . ;");
  forth.displayDico();

  forth.eatString("1 2 QUADRUPLE + .");
  forth.dumpDico("Dico.fbin");

  return 0;
}

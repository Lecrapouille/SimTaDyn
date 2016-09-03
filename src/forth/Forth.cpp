#include "Forth.hpp"

// **************************************************************
// Constructor: initialize the context but do not boot
// **************************************************************
Forth::Forth()
{
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
  return /*(id >= 0) &&*/ (id < NUM_PRIMITIVES);
}

// **************************************************************
//
// **************************************************************
void Forth::execPrimitive(const Cell16 idPrimitive)
{
  switch (idPrimitive)
    {
    case FORTH_PRIMITIVE_NOP:
      break;

    case FORTH_PRIMITIVE_COLON:
      //getNextWord(word);
      //createDicoEntry(FORTH_PRIMITIVE_PLUS, word, 0);
      break;

    case FORTH_PRIMITIVE_EXIT:
      RPOP(ip);
      break;

    case FORTH_PRIMITIVE_1MINUS:
      tos--;
      break;
    case FORTH_PRIMITIVE_1PLUS:
      tos++;
      break;
    case FORTH_PRIMITIVE_2MINUS:
      tos -= 2;
      break;
    case FORTH_PRIMITIVE_2PLUS:
      tos += 2;
      break;

      // drop ( a -- )
    case FORTH_PRIMITIVE_DROP:
      DPOP(tos);
      break;

      // nip ( a b -- b ) swap drop ;
    case FORTH_PRIMITIVE_NIP:
      DDROP();
      break;

      // dup ( a -- a a )
    case FORTH_PRIMITIVE_DUP:
      DPUSH(tos);
      break;

      // swap ( a b -- b a )
    case FORTH_PRIMITIVE_SWAP:
      tos2 = tos;
      DPOP(tos);
      DPUSH(tos2);
      break;

      // over ( a b -- a b a )
    case FORTH_PRIMITIVE_OVER:
      DPUSH(tos);
      tos = DPICK(1);
      break;

      // rot ( a b c -- b c a )
    case FORTH_PRIMITIVE_ROT:
      DPOP(tos2);
      DPOP(tos3);
      DPUSH(tos2);
      DPUSH(tos);
      tos = tos3;
      break;

      // tuck ( a b -- b a b ) swap over ;
    case FORTH_PRIMITIVE_TUK:
      DPOP(tos2);
      DPUSH(tos);
      DPUSH(tos2);
      break;

      // 2dup ( a b -- a b a b ) over over ;
    case FORTH_PRIMITIVE_2DUP:
      DPUSH(tos);
      tos2 = DPICK(1);
      DPUSH(tos2);
      break;

      // 2over ( a b c d -- a b c d a b )
    case FORTH_PRIMITIVE_2OVER:
      DPUSH(tos);
      tos2 = DPICK(3);
      DPUSH(tos2);
      tos = DPICK(3);
      break;

      // 2swap ( a b c d -- c d a b )
    case FORTH_PRIMITIVE_2SWAP:
      DPOP(tos2);
      DPOP(tos3);
      DPOP(tos4);
      DPUSH(tos3);
      DPUSH(tos4);
      DPUSH(tos2);
      break;

      // 2drop ( a b -- ) drop drop ;
    case FORTH_PRIMITIVE_2DROP:
      DDROP();
      DPOP(tos);
      break;

    case FORTH_PRIMITIVE_PLUS:
      BINARY_OP(+);
      break;
    case FORTH_PRIMITIVE_MINUS:
      BINARY_OP(-);
      break;
    case FORTH_PRIMITIVE_DIV:
      BINARY_OP(/);
      break;
    case FORTH_PRIMITIVE_TIMES:
      BINARY_OP(*);
      break;
    case FORTH_PRIMITIVE_RSHIFT:
      BINARY_OP(>>);
      break;
    case FORTH_PRIMITIVE_LSHIFT:
      BINARY_OP(<<);
      break;
    case FORTH_PRIMITIVE_GREATER_EQUAL:
      BINARY_OP(>=);
      break;
    case FORTH_PRIMITIVE_LOWER_EQUAL:
      BINARY_OP(<=);
      break;
    case FORTH_PRIMITIVE_GREATER:
      BINARY_OP(>);
      break;
    case FORTH_PRIMITIVE_LOWER:
      BINARY_OP(<);
      break;
    case FORTH_PRIMITIVE_EQUAL:
      BINARY_OP(==);
      break;
    case FORTH_PRIMITIVE_AND:
      BINARY_OP(&);
      break;
    case FORTH_PRIMITIVE_OR:
      BINARY_OP(|);
      break;
    case FORTH_PRIMITIVE_XOR:
      BINARY_OP(^);
      break;
    case FORTH_PRIMITIVE_DISP:
      std::cout << std::setbase(base) << tos << " ";//std::endl;
      DPOP(tos);
      break;
    default:
      ForthUnknownPrimitive e(idPrimitive, __PRETTY_FUNCTION__); throw e;
      break;
    }
}

// **************************************************************
// Return the number of element in the return stack
// **************************************************************
inline int32_t Forth::RStackSize() const
{
  int32_t size = return_stack - rsp;
  if (size < 0)
    {
      ForthRStackOV e; throw e;
    }
  return size;
}

// **************************************************************
//
// **************************************************************
void Forth::execToken(const Cell16 tx)
{
  Cell16 token = tx;
  ip = NULL;

  // Always eat the top of the stack and store the value in a
  // working register.
  DPOP(tos);

  // Traverse the word definition like a tree
  do
    {
      // A non primitive word is a consecutive set of primitive
      // identifier
      while (!isPrimitive(token))
        {
          RPUSH(ip);
          ip = (Cell16*) (dictionary + token);
          //checkDicoBoundaries(ip, "execToken");
          token = *(ip++);
        }

      //
      execPrimitive(token);

      // Do not forget than non-primitive words have the
      // token EXIT to pop the return stack to get back
      // IP value.
      if (NULL != ip)
        {
          token = *ip++;
        }
    } while (RStackSize() > 0);

  // Store the working register
  DPUSH(tos);
}

// **************************************************************
// Convert a string to a number.
// Return false if an error occured
// **************************************************************
bool Forth::toNumber(const std::string& word, Cell32& number)
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
//
// **************************************************************
void Forth::eat(std::string const& word)
{
  Cell32 number;
  Cell16 token;

  if (toToken(word, token))
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
  createDicoEntry(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
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
  forth.dumpDico("Dico.fbin");
  forth.displayDico();

  std::string txt = "1 2 3 4 2OVER . . . . . .";
  //forth.eatString(txt);

  forth.eatFile("hello.simforth");

  return 0;
}

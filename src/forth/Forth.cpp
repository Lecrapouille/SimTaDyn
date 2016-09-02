#include "Forth.hpp"

#define FLAG_IMMED           0x01
#define FLAG_HIDDEN          0x20
#define FLAG_NODISASM        0x40
#define FLAG_LENMASK         0x1f
#define FLAG_MARKER          0x80
#define FLAG_FLAGSMASK       0x7f

// Data stack
#define DPUSH(t) (*(dsp++) = CELL(t))
#define DDROP()  (*(--dsp))
#define DPOP(t)  (t = DDROP())
#define DPICK(n) (*(dsp - n - 1))

// Return stack
#define RPUSH(t) (*(rsp++) = CADDR(t))
#define RPOP(t)  (t = *(--rsp))

//
#define BINARY_OP(op) { tos = DDROP() op tos; }

//
#define SWAP(x, y) { x = x ^ y; y = x ^ y; x = x ^ y; }

enum ForthPrimitive
  {
    FORTH_PRIMITIVE_NOP = 0,
    FORTH_PRIMITIVE_EXIT,

    FORTH_PRIMITIVE_1MINUS,
    FORTH_PRIMITIVE_1PLUS,
    FORTH_PRIMITIVE_2MINUS,
    FORTH_PRIMITIVE_2PLUS,

    FORTH_PRIMITIVE_NIP,
    FORTH_PRIMITIVE_DUP,
    FORTH_PRIMITIVE_DROP,
    FORTH_PRIMITIVE_SWAP,
    FORTH_PRIMITIVE_OVER,
    FORTH_PRIMITIVE_ROT,
    FORTH_PRIMITIVE_TUK,

    FORTH_PRIMITIVE_2DUP,
    FORTH_PRIMITIVE_2OVER,
    FORTH_PRIMITIVE_2SWAP,
    FORTH_PRIMITIVE_2DROP,

    FORTH_PRIMITIVE_PLUS,
    FORTH_PRIMITIVE_MINUS,
    FORTH_PRIMITIVE_DIV,
    FORTH_PRIMITIVE_TIMES,
    FORTH_PRIMITIVE_RSHIFT,
    FORTH_PRIMITIVE_LSHIFT,
    FORTH_PRIMITIVE_GREATER_EQUAL,
    FORTH_PRIMITIVE_LOWER_EQUAL,
    FORTH_PRIMITIVE_GREATER,
    FORTH_PRIMITIVE_LOWER,
    FORTH_PRIMITIVE_EQUAL,
    FORTH_PRIMITIVE_AND,
    FORTH_PRIMITIVE_OR,
    FORTH_PRIMITIVE_XOR,
    FORTH_PRIMITIVE_DISP,
    NUM_PRIMITIVES
  };

#define CELL(x)  reinterpret_cast<Cell>(x)
#define CADDR(x) reinterpret_cast<Cell*>(x)

Forth::Forth()
{
  dsp = data_stack;
  rsp = return_stack;
  here = 0U;
  last = 0U;
  changeBase(10);
  //doForth();
}

inline int32_t Forth::dstackSize()
{
  return return_stack - rsp;
}

void Forth::execPrimitive(const Cell token)
{
  switch (token)
    {
    case FORTH_PRIMITIVE_NOP:
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
      std::cout << std::setbase(base) << tos << std::endl;
      DPOP(tos);
      break;
    default:
      std::cerr << "Unrecognised token = " << token << std::endl;
      break;
    }
}

inline bool Forth::isPrimitive(const Cell token)
{
  return /*(token >= 0) &&*/ (token < NUM_PRIMITIVES);
}

inline void Forth::changeBase(const uint32_t newbase)
{
  base = newbase;
}

void Forth::eval(const Cell tx)
{
  Cell token = tx;

  DPOP(tos);
  do
    {
      /* while (!isPrimitive(token))
        {
          RPUSH(ip);
          ip = dictionary + token;
          token = *(ip++);
          }*/
      execPrimitive(token);
      if (ip)
        {
          token = *ip++;
        }
    } while ((return_stack - rsp) > 0);
  DPUSH(tos);
}

// Convert a string into a number
bool Forth::toInt(const std::string& word, Cell& number)
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

bool Forth::interprete(const std::string& word)
{
  Cell number, token;

  //std::cout << "Interprete: " << word << std::endl;
  if (toInt(word, number))
    {
      DPUSH(number);
    }
  else if (toToken(word, token))
    {
      eval(token);
    }
  else
    return false;
  return true;
}

bool Forth::readString(const std::string& code_forth)
{
  std::string delimiter = " ";
  std::string word;
  size_t last = 0;
  size_t next = 0;

  while ((next = code_forth.find(delimiter, last)) != std::string::npos)
    {
      word = code_forth.substr(last, next - last);
      last = next + 1;
      if (false == interprete(word))
        return false;
    }
  word = code_forth.substr(last);
  if (false == interprete(word))
    return false;
  return true;
}

Forth forth;

// Debug
static void lookup(const std::string& word)
{
  bool res;
  uint32_t ptr;

  std::cout << "-------------------------------------\n";
  std::cout << "Looking for " << word << std::endl;
  res = forth.toToken(word, ptr);
  if (res)
    {
      std::cout << "Found " << ptr << std::endl;
    }
  else
    {
      std::cout << "Not found" << std::endl;
    }
}

int main()
{
  // Les ranger par ordre lexico ?
  forth.createDicoEntry(FORTH_PRIMITIVE_NOP, "NOP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_1MINUS, "1-", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_1PLUS, "1+", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_2MINUS, "2-", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_2PLUS, "2+", 0);

  forth.createDicoEntry(FORTH_PRIMITIVE_NIP, "NIP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_DUP, "DUP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_DROP, "DROP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_SWAP, "SWAP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_OVER, "OVER", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_ROT, "ROT", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_TUK, "TUK", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_2DUP, "2DUP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_2DROP, "2DROP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_2SWAP, "2SWAP", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_2OVER, "2OVER", 0);

  forth.createDicoEntry(FORTH_PRIMITIVE_PLUS, "+", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_MINUS, "-", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_DIV, "/", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_TIMES, "*", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_RSHIFT, ">>", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_LSHIFT, "<<", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_GREATER_EQUAL, ">=", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_LOWER_EQUAL, "<=", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_GREATER, ">", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_LOWER, "<", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_EQUAL, "==", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_AND, "AND", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_OR, "OR", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_XOR, "XOR", 0);
  forth.createDicoEntry(FORTH_PRIMITIVE_DISP, ".", 0);
  forth.dumpDico();
  forth.displayDico();

  lookup(".");
  lookup("1+");
  lookup("PLUS");
  lookup("DUP");
  std::cout << "-------------------------------------\n";

  std::string txt = "1 2 3 4 2OVER . . . . . .";
  forth.readString(txt);

  return 0;
}

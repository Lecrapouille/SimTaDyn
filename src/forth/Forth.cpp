#include "Forth.hpp"

#define FLAG_IMMED           0x01
#define FLAG_HIDDEN          0x20
#define FLAG_NODISASM        0x40
#define FLAG_LENMASK         0x1f
#define FLAG_MARKER          0x80
#define FLAG_FLAGSMASK       0x7f

// Data stack
#define DPUSH(t) (*(dsp++) = CELL(t))
#define DPOP()   (*(--dsp))
#define DDROP()  (tos = DPOP())
#define DPICK(n) (dsp[n])

// Return stack
#define RPUSH(t) (*(rsp++) = CADDR(t))
#define RPOP()   (*(--rsp))

#define BINARY_OP(op) { tos = DPOP() op tos; }
enum ForthPrimitive
  {
    TOKEN_PRIM_DUP,
    TOKEN_PRIM_1MINUS,
    TOKEN_PRIM_1PLUS,
    TOKEN_PRIM_2MINUS,
    TOKEN_PRIM_2PLUS,
    TOKEN_PRIM_PLUS,
    TOKEN_PRIM_MINUS,
    TOKEN_PRIM_DIV,
    TOKEN_PRIM_TIMES,
    TOKEN_PRIM_RSHIFT,
    TOKEN_PRIM_LSHIFT,
    TOKEN_PRIM_GREATER_EQUAL,
    TOKEN_PRIM_LOWER_EQUAL,
    TOKEN_PRIM_GREATER,
    TOKEN_PRIM_LOWER,
    TOKEN_PRIM_EQUAL,
    TOKEN_PRIM_AND,
    TOKEN_PRIM_OR,
    TOKEN_PRIM_XOR,
    TOKEN_PRIM_DISP,
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
    case TOKEN_PRIM_DUP:
      DPUSH(tos);
      break;
    case TOKEN_PRIM_1MINUS:
      tos--;
      break;
    case TOKEN_PRIM_1PLUS:
      tos++;
      break;
    case TOKEN_PRIM_2MINUS:
      tos -= 2;
      break;
    case TOKEN_PRIM_2PLUS:
      tos += 2;
      break;
    case TOKEN_PRIM_PLUS:
      BINARY_OP(+);
      break;
    case TOKEN_PRIM_MINUS:
      BINARY_OP(-);
      break;
    case TOKEN_PRIM_DIV:
      BINARY_OP(/);
      break;
    case TOKEN_PRIM_TIMES:
      BINARY_OP(*);
      break;
    case TOKEN_PRIM_RSHIFT:
      BINARY_OP(>>);
      break;
    case TOKEN_PRIM_LSHIFT:
      BINARY_OP(<<);
      break;
    case TOKEN_PRIM_GREATER_EQUAL:
      BINARY_OP(>=);
      break;
    case TOKEN_PRIM_LOWER_EQUAL:
      BINARY_OP(<=);
      break;
    case TOKEN_PRIM_GREATER:
      BINARY_OP(>);
      break;
    case TOKEN_PRIM_LOWER:
      BINARY_OP(<);
      break;
    case TOKEN_PRIM_EQUAL:
      BINARY_OP(==);
      break;
    case TOKEN_PRIM_AND:
      BINARY_OP(&);
      break;
    case TOKEN_PRIM_OR:
      BINARY_OP(|);
      break;
    case TOKEN_PRIM_XOR:
      BINARY_OP(^);
      break;
    case TOKEN_PRIM_DISP:
      std::cout << std::setbase(base) << tos << std::endl;
      DDROP();
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

  tos = DPOP();
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

// Convert a string into a token
bool Forth::toToken(const std::string& word, Cell& token)
{
  if (word == std::string("DUP"))
    token = TOKEN_PRIM_DUP;
  else if (word == std::string("<="))
    token = TOKEN_PRIM_LOWER_EQUAL;
  else if (word == std::string(">"))
    token = TOKEN_PRIM_GREATER;
  else if (word == std::string("+"))
    token = TOKEN_PRIM_PLUS;
  else if (word == std::string("."))
    token = TOKEN_PRIM_DISP;
  else
    {
      std::cerr << "Unknow word '" << word << "'" << std::endl;
      return false;
    }
  return true;
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

static void lookup(const std::string& word)
{
  bool res;
  uint32_t ptr;

  std::cout << "-------------------------------------\n";
  std::cout << "Looking for " << word << std::endl;
  res = forth.searchDicoEntry(word, ptr);
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
  std::string txt = "1 2 + DUP + .";

  forth.readString(txt);

  forth.createDicoEntry(TOKEN_PRIM_DUP, "DUP", 0);
  forth.createDicoEntry(TOKEN_PRIM_1MINUS, "1-", 0);
  forth.createDicoEntry(TOKEN_PRIM_1PLUS, "1+", 0);
  forth.createDicoEntry(TOKEN_PRIM_2MINUS, "2-", 0);
  forth.createDicoEntry(TOKEN_PRIM_2PLUS, "2+", 0);
  forth.createDicoEntry(TOKEN_PRIM_PLUS, "+", 0);
  forth.createDicoEntry(TOKEN_PRIM_MINUS, "-", 0);
  forth.createDicoEntry(TOKEN_PRIM_DIV, "/", 0);
  forth.createDicoEntry(TOKEN_PRIM_TIMES, "*", 0);
  forth.createDicoEntry(TOKEN_PRIM_RSHIFT, ">>", 0);
  forth.createDicoEntry(TOKEN_PRIM_LSHIFT, "<<", 0);
  forth.createDicoEntry(TOKEN_PRIM_GREATER_EQUAL, ">=", 0);
  forth.createDicoEntry(TOKEN_PRIM_LOWER_EQUAL, "<=", 0);
  forth.createDicoEntry(TOKEN_PRIM_GREATER, ">", 0);
  forth.createDicoEntry(TOKEN_PRIM_LOWER, "<", 0);
  forth.createDicoEntry(TOKEN_PRIM_EQUAL, "==", 0);
  forth.createDicoEntry(TOKEN_PRIM_AND, "AND", 0);
  forth.createDicoEntry(TOKEN_PRIM_OR, "OR", 0);
  forth.createDicoEntry(TOKEN_PRIM_XOR, "XOR", 0);
  forth.createDicoEntry(TOKEN_PRIM_DISP, ".U", 0);
  forth.dumpDico();
  forth.displayDico();

  lookup("PLUS");
  lookup("1+");

  return 0;
}

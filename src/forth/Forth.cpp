#include "Forth.hpp"

#define FLAG_IMMED           0x01
#define FLAG_HIDDEN          0x20
#define FLAG_NODISASM        0x40
#define FLAG_LENMASK         0x1f
#define FLAG_MARKER          0x80
#define FLAG_FLAGSMASK       0x7f

// Data stack
#define DPUSH(t) (*(dsp++) = CELL(t))
#define DPOP     (*(--dsp))
#define DROP     DPOP
#define DPICK(n) (dsp[n])

// Return stack
#define RPUSH(t) (*(rsp++) = CADDR(t))
#define RPOP     (*(--rsp))

#define BINARY_OP(op) { tos = DPOP op tos; }
enum ForthPrimitive
  {
    TOKEN_PRIM_ADD,
    TOKEN_PRIM_DISP,
    NUM_PRIMITIVES
  };

#define CELL(x)  reinterpret_cast<Cell>(x)
#define CADDR(x) reinterpret_cast<Cell*>(x)

Forth::Forth()
{
  dsp = data_stack;
  rsp = return_stack;
  here = dictionnary;
}

inline bool Forth::isPrimitive(const Cell token)
{
  return /*(token >= 0) &&*/ (token < NUM_PRIMITIVES);
}

void Forth::eval(const Cell tx)
{
  Cell token = tx;

  tos = DPOP;
  do
    {
      while (!isPrimitive(token))
        {
          RPUSH(ip);
          ip = dictionnary + token;
          token = *(ip++);
        }
      switch (token)
        {
        case TOKEN_PRIM_ADD:
          std::cout << tos << " + " << std::endl;
          BINARY_OP(+);
          break;
        case TOKEN_PRIM_DISP:
          std::cout << tos << std::endl;
          break;
        default:
          std::cerr << "Unrecognised token = 0x" << token << std::endl;
          break;
        }

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
  if (word == std::string("+"))
    token = TOKEN_PRIM_ADD;
  else if (word == std::string("."))
    token = TOKEN_PRIM_DISP;
  else
    return false;
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

  std::cout << "Interprete: " << word << std::endl;
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


int main()
{
  Forth forth;
  std::string txt = "1 2 + 3 4 + + .";

  forth.readString(txt);

  return 0;
}

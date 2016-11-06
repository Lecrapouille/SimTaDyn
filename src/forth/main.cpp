#include "Forth.hpp"

int main(void)
{
  Forth forth;
  std::pair<bool, std::string> res;

  forth.boot();
  res = forth.eatString(": FOO + + ;");
  //res = forth.eatString(": BAR FOO + . ;");
  //res = forth.eatString("1 1 + .");
  //std::cout << res.second << std::endl;
  //res = forth.eatString("2 ( COUCOU ) 3 FOO ( 3 ) .");
  //res = forth.eatString(" BUG ");
  //res = forth.eatString(" 1    2 + .");
  //res = forth.eatString("42 24 + .");
  //res = forth.eatString("      ");
  if (res.first)
    {
      std::cout << res.second << std::endl;
    }
  else
    {
      std::cerr << "[ERROR]: " << res.second << std::endl;
    }
  forth.dictionary().display();
  forth.dictionary().dump("toto.bin");

  return 0;
}

#include "Forth.hpp"

int main(void)
{
  Forth forth;
  std::pair<bool, std::string> res;

  forth.boot();

  res = forth.eatString("$1400 .");
  forth.ok(res);

  res = forth.eatString("\'A' .");
  forth.ok(res);
  return 0;


  res = forth.eatString("55 >R R> .");
  forth.ok(res);

  res = forth.eatString("1 2 MIN .");
  forth.ok(res);
  res = forth.eatString("1 2 MAX .");
  forth.ok(res);
  res = forth.eatString("HERE . DEPTH .");
  forth.ok(res);
  res = forth.eatString("42 HERE !");
  forth.ok(res);
  forth.dictionary().dump("toto2.bin");

  res = forth.eatString("HERE @ .");
  forth.ok(res);

  return 0;

  res = forth.eatString("1 1 + .");
  forth.ok(res);

  res = forth.eatString(": PLUS + + ;");
  forth.ok(res);

  res = forth.eatString(": DPLUS PLUS . ;");
  forth.ok(res);

  res = forth.eatString("1 2 3 DPLUS");
  forth.ok(res);

  res = forth.eatString(": NB 42 ;");
  forth.ok(res);
forth.dictionary().dump("toto2.bin");
 forth.dictionary().display();
  res = forth.eatString("NB 24 + .");
  forth.ok(res);

  return 0;





#warning "BUG non detecte"
  res = forth.eatString(": NB 42 ;");
  forth.ok(res);
  res = forth.eatString("NB + 24 .");
  forth.ok(res);
  return 0;

  /*
  res = forth.eatString("\\ AAA \n: FOOBAR + + . ;\n1 2 3 FOOBAR \\ POUET");
  //res = forth.eatFile("hello.fs");
  res = forth.eatString(": FOO . ;");
  //res = forth.eatString(": BAR FOO + . ;");
  //res = forth.eatString("1 1 + .");
  //std::cout << res.second << std::endl;
  res = forth.eatString("2 ( COUCOU ) 3 FOO ( 3 ) .");
  //res = forth.eatString(" BUG ");
  //res = forth.eatString(" 1    2 + .");
  //res = forth.eatString("42 24 + .");
  //res = forth.eatString("      ");
  */
  //res = forth.eatFile("hello.fs");

  res = forth.eatString("+");
  forth.ok(res);

  res = forth.eatString("( aahhhh ) ( )");
  forth.ok(res);

  res = forth.eatString(": FOO + ;");
  forth.ok(res);
  forth.dictionary().dump("toto.bin");
  res = forth.eatString("IMMEDIATE");
  forth.ok(res);

  //res = forth.eatString("66 2");
  //forth.ok(res);

  res = forth.eatString(": BAR FOO . ; BAR");
  forth.ok(res);

  res = forth.eatString(": BAR + + ;");
  forth.ok(res);
  res = forth.eatString(": BAR + + . ;");
  forth.ok(res);
  res = forth.eatString("4 5 6 BAR");
  forth.ok(res);
  res = forth.eatString("DEPTH . CR");
  forth.ok(res);

  res = forth.eatString(": NB 42 ;");
  forth.ok(res);
  //#warning "BUG non detecte"
  //res = forth.eatString("44 +");
  //forth.ok(res);
  //res = forth.eatString("NB + 24 .");
  //forth.ok(res);
  res = forth.eatString("NB 24 + .");
  forth.ok(res);

  forth.dictionary().display();
  forth.dictionary().dump("toto2.bin");

  return 0;
}


// Bug1: forth.eatString("\\ AAA \n: FOO + + . ;\n1 2 3 FOO");
// Tout le string est pris en compte alors qu'il devrait s'arreter au \n

// Bug2

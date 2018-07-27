#include "ForthPrimitives.hpp"
#include "Forth.hpp"
#include "Logger.hpp"

#define LABEL(a) c_primitives[TOK_##a] = &&L_##a
#define ENTRY(a)                         a, ((sizeof a) - 1U)
#define COMPILE(token, name, immediate)  LABEL(token); m_dictionary.compileWord(TOK_ ## token, ENTRY(name), immediate)
#define PRIMITIVE(token, name)           COMPILE(token, name, false)
#define IMMEDIATE_PRIMITIVE(token, name) COMPILE(token, name, true)

// **************************************************************
//! This method should be called after the contructor Forth()
//! and has been separated from the constructor to load the
//! dictionary at the desired moment.
// **************************************************************
void Forth::boot()
{
  LOGI("Forth booting ...");
  /*PRIMITIVE(NOOP, "NOOP");
  PRIMITIVE(DUP,  "DUP");
  PRIMITIVE(ADD,  "+");
  PRIMITIVE(DISP, ".");*/
//#include "primitives/dstack.h"
}

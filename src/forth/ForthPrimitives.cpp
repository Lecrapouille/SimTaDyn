//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Forth.hpp"

#  define BINARY_OP(op) { m_tos = ((int32_t) DDROP()) op ((int32_t) m_tos); } // Pop a value, apply it the operation op with the content of the register tos (Top Of Stack)
#  define LOGICAL_OP(op) { m_tos = -1 * (((int32_t) DDROP()) op ((int32_t) m_tos)); }

// **************************************************************
//! \param idPrimitive the token of a primitive else an exception
//! is triggered.
//! UnknownForthPrimitive if idPrimitive is not the token of a
//! primitive (idPrimitive >= NUM_PRIMITIVES).
//! \throw UnfinishedStream called by Forth::nextWord() if the
//! stream does not contain a Forth word.
//! \throw UnknownForthWord if the extracted word by Forth::nextWord()
//! is not present in the dictionary.
//! \throw AbortForth if triggered by the Forth word ABORT.
//! \throw ModifiedStackDepth if the data stack depth is modified
//! during a definition.
//! \throw OutOfBoundDictionary if attempting to store data outside
//! the dictionary bounds.
//! \throw NoSpaceDictionary if attempting to add a new entry in a
//! full dictionary.
// **************************************************************
void Forth::execPrimitive(const Cell16 idPrimitive)
{
  //std::cout << "execPrimitive " << idPrimitive << std::endl;
  switch (idPrimitive)
    {
      // Dummy word / No operation
    case FORTH_PRIMITIVE_NOP:
      std::cout << "NOP\n";
      break;

      // Begin of commentary
    case FORTH_PRIMITIVE_LPARENTHESIS:
      m_saved_state = m_state;
      m_state = forth::Comment;
      break;

      // End of commentary
    case FORTH_PRIMITIVE_RPARENTHESIS:
      m_state = m_saved_state;
      break;

      // Line of commentary
    case FORTH_PRIMITIVE_COMMENTARY:
      STREAM.skipLine();
      break;

      // Begin the definition of a new word
    case FORTH_PRIMITIVE_COLON:
      create(nextWord());
      m_state = forth::Compile;
      break;

      // End the definition of a new word
    case FORTH_PRIMITIVE_SEMICOLON:
      m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
      m_state = forth::Interprete;
      if (m_depth_at_colon != stackDepth(forth::DataStack))
        {
          m_dictionary.m_last = m_last_at_colon;
          m_dictionary.m_here = m_here_at_colon;
          ModifiedStackDepth e(m_creating_word);
          throw e;
        }
      break;

      // Push in data stack the next free slot in the dictionary
    case FORTH_PRIMITIVE_PCREATE:
      DPUSH(m_tos);
      m_tos = m_ip + 4U;
      break;

      // Give a name to the next free slot in the dictionary.
      // (note: HERE is not moved)
      // https://fr.wikiversity.org/wiki/Forth/Conserver_des_donn%C3%A9es
    case FORTH_PRIMITIVE_CREATE:
      create(nextWord());
      m_dictionary.appendCell16(FORTH_PRIMITIVE_PCREATE);
      m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
      break;

    case FORTH_PRIMITIVE_BUILDS:
      create(nextWord());
      break;

    case FORTH_PRIMITIVE_DOES:
      break;

      // Set immediate the last word
    case FORTH_PRIMITIVE_IMMEDIATE:
      m_dictionary.m_dictionary[m_dictionary.m_last] |= FLAG_IMMEDIATE;
      break;

    case FORTH_PRIMITIVE_INCLUDE:
      // Restore TOS register (because execToken() poped TOS)
      DPUSH(m_tos);

      includeFile(nextWord());

      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_STATE:
      DPUSH(m_tos);
      m_tos = m_state;
      break;

    case FORTH_PRIMITIVE_TRACE_ON:
      m_trace = true;
      break;

    case FORTH_PRIMITIVE_TRACE_OFF:
      m_trace = false;
      break;

    case FORTH_PRIMITIVE_SMUDGE:
      {
        std::string const& word = nextWord();
        if (false == m_dictionary.smudge(word))
          {
            std::cout << FORTH_WARNING_COLOR << "[WARNING] Unknown word '"
                      << word << "'. Word SMUDGE Ignored !"
                      << FORTH_NORMAL_COLOR << std::endl;
          }
      }
      break;

    case FORTH_PRIMITIVE_TICK:
      {
        std::string const& word = nextWord();
        Cell16 token;
        bool immediate;
        if (false == m_dictionary.find(word, token, immediate))
          {
            token = 0;
            std::cout << FORTH_WARNING_COLOR << "[WARNING] Unknown word '"
                      << word << "'. Word TICK Ignored !"
                      << FORTH_NORMAL_COLOR << std::endl;
          }
        DPUSH(m_tos);
        m_tos = token;
      }
      break;

     case FORTH_PRIMITIVE_COMPILE:
       m_ip += 2;
       m_dictionary.appendCell16(m_dictionary.read16at(m_ip));
       break;

       // Append to the dictionary the next word
     case FORTH_PRIMITIVE_ICOMPILE:
       {
         Cell16 token;
         bool immediate;
         std::string const& word = nextWord();

         //m_ip += 2;
         if (m_dictionary.find(word, token, immediate))
           {
             m_dictionary.appendCell16(token);
           }
         else
           {
             UnknownForthWord e(word); throw e;
           }
       }
       break;

       // ANSI word to replace [COMPILE] and COMPILE
    case FORTH_PRIMITIVE_POSTPONE:
      {
        Cell16 token;
        bool immediate;
        std::string const& word = nextWord();

        if (m_dictionary.find(word, token, immediate))
          {
            if (immediate)
              {
                m_dictionary.appendCell16(token);
              }
            else
              {
                m_ip += 2;
                m_dictionary.appendCell16(m_dictionary.read16at(m_ip));
              }
          }
        else
          {
            UnknownForthWord e(word); throw e;
          }
      }
      break;

     case FORTH_PRIMITIVE_ABORT:
       abort("COUCOU");
       break;

     case FORTH_PRIMITIVE_EXECUTE:
       execToken(m_tos);
       break;

     case FORTH_PRIMITIVE_LBRACKET:
       m_state = forth::Interprete;
       break;

    case FORTH_PRIMITIVE_RBRACKET:
       m_state = forth::Compile;
       break;

    case FORTH_PRIMITIVE_HERE:
      DPUSH(m_tos);
      m_tos = m_dictionary.here();
      break;

    case FORTH_PRIMITIVE_LAST:
      DPUSH(m_tos);
      m_tos = m_dictionary.last();
      break;

    case FORTH_PRIMITIVE_ALLOT:
      m_dictionary.allot((int32_t) m_tos);
      DPOP(m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA32:
      m_dictionary.appendCell32(m_tos);
      DPOP(m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA16:
      m_dictionary.appendCell16(m_tos);
      DPOP(m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA8:
      m_dictionary.appendCell8(m_tos);
      DPOP(m_tos);
      break;

      // ( a-addr -- x )
      // x is the value stored at a-addr.
    case FORTH_PRIMITIVE_FETCH:
      m_tos = m_dictionary.read32at(m_tos);
      break;

      // Store x at a-addr.
      // ( x a-addr -- )
    case FORTH_PRIMITIVE_STORE32:
      DPOP(m_tos1);
      m_dictionary.write32at(m_tos, m_tos1);
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_STORE16:
      DPOP(m_tos1);
      m_dictionary.write16at(m_tos, m_tos1);
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_STORE8:
      DPOP(m_tos1);
      m_dictionary.write8at(m_tos, m_tos1);
      DPOP(m_tos);
      break;

      /* ( src dst n -- ) */
    case FORTH_PRIMITIVE_CMOVE:
      DPOP(m_tos2); // dst
      DPOP(m_tos1); // src
      m_dictionary.move(m_tos2, m_tos1, m_tos);
      DPOP(m_tos);
      break;

      /*  case FORTH_PRIMITIVE_THROW:
      if (m_tos)
        {
          M_THROW(m_tos);
        }
      else DPOP(m_tos);
      break;*/

      // Restore the IP when interpreting the definition
      // of a non primitive word
    case FORTH_PRIMITIVE_EXIT:
      RPOP(m_ip);
      if (m_trace) {
        std::cout << "POPed: " << std::hex << (int) m_ip << std::endl;
      }
      break;

      // Change IP
    case FORTH_PRIMITIVE_BRANCH:
      m_ip += m_dictionary.read16at(m_ip + 2U);
      // Do not forget that m_ip will be += 2 next iteration
      break;

      // Change IP if top of stack is 0
    case FORTH_PRIMITIVE_0BRANCH:
      if (0 == m_tos)
        {
          m_ip += m_dictionary.read16at(m_ip + 2U);
        }
      else
        {
          m_ip += 2U;
        }
      // Do not forget that m_ip will be += 2 next iteration
      DPOP(m_tos);
      break;

      // Move x to the return stack.
      // ( x -- ) ( R: -- x )
    case FORTH_PRIMITIVE_TO_RSTACK:
      RPUSH(m_tos);
      DPOP(m_tos);
      break;

      // Transfer cell pair x1 x2 to the return stack
      // ( x1 x2 -- ) ( R: -- x1 x2 )
    case FORTH_PRIMITIVE_2TO_RSTACK:
      DPOP(m_tos1);
      RPUSH(m_tos1);
      RPUSH(m_tos);
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_FROM_RSTACK:
      DPUSH(m_tos);
      RPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_2FROM_RSTACK:
      DPUSH(m_tos);
      RPOP(m_tos);
      RPOP(m_tos1);
      DPUSH(m_tos1);
      break;

#if 0
    case FORTH_PRIMITIVE_DO:
      std::cout << "(DO)" << std::endl;
      RPUSH(m_tos);
      DPOP(m_tos1); // FIXME: optim: RPUSH(DPOP);
      RPUSH(m_tos1);
      m_tos = DDROP();
      break;

    case FORTH_PRIMITIVE_LOOP:
      std::cout << "(LOOP)" << std::endl;
      RPOP(m_tos1); /* limit */
      RPOP(m_tos2); /* index */
      ++m_tos2;
      if (m_tos2 == m_tos1)
        {
          ++m_ip;   /* skip branch offset, exit loop */
        }
      else
        {
/* Push index and limit back to R */
          RPUSH(m_tos2);
          RPUSH(m_tos1);
/* Branch back to just after (DO) */
          m_ip = m_dictionary.read16at(m_ip);
        }
      break;
#endif

    case FORTH_PRIMITIVE_I:
      DPUSH(m_tos);
      m_tos = RPICK(0);
      break;

    case FORTH_PRIMITIVE_J:
      DPUSH(m_tos);
      m_tos = RPICK(2);
      break;

    case FORTH_PRIMITIVE_CELL:
      DPUSH(m_tos);
      m_tos = sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_CELLS:
      m_tos = m_tos * sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_LITERAL_16:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITERAL
      m_tos = m_dictionary.read16at(m_ip);
      break;

    case FORTH_PRIMITIVE_LITERAL_32:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITERAL
      m_tos = m_dictionary.read32at(m_ip);
      m_ip += 2U; // Skip the number - 2 because of next ip
      break;

    case FORTH_PRIMITIVE_1MINUS:
      --m_tos;
      break;
    case FORTH_PRIMITIVE_1PLUS:
      ++m_tos;
      break;
    case FORTH_PRIMITIVE_2MINUS:
      m_tos -= 2;
      break;
    case FORTH_PRIMITIVE_2PLUS:
      m_tos += 2;
      break;

      // drop ( a -- )
    case FORTH_PRIMITIVE_DROP:
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_DEPTH:
      DPUSH(m_tos);
      m_tos = stackDepth(forth::DataStack);
      break;

      // nip ( a b -- b ) swap drop ;
    case FORTH_PRIMITIVE_NIP:
      DPOP(m_tos1);
      break;

    case FORTH_PRIMITIVE_ROLL:
      {
        m_tos1 = DPICK(m_tos);
        uint32_t *src = &DPICK(m_tos - 1);
        uint32_t *dst = &DPICK(m_tos);
        uint32_t ri = m_tos;
        while (ri--)
          {
            *dst-- = *src--;
          }
        m_tos = m_tos1;
        ++m_dsp;
      }
      break;

      // ( ... n -- sp(n) )
    case FORTH_PRIMITIVE_PICK:
      m_tos = DPICK(m_tos);
      break;

      // dup ( a -- a a )
    case FORTH_PRIMITIVE_DUP:
      DPUSH(m_tos);
      break;

    case FORTH_PRIMITIVE_QDUP:
      if (m_tos)
        {
          DPUSH(m_tos);
        }
      break;

      // swap ( a b -- b a )
    case FORTH_PRIMITIVE_SWAP:
      m_tos2 = m_tos;
      DPOP(m_tos);
      DPUSH(m_tos2);
      break;

      // over ( a b -- a b a )
    case FORTH_PRIMITIVE_OVER:
      DPUSH(m_tos);
      m_tos = DPICK(1);
      break;

      // rot ( a b c -- b c a )
    case FORTH_PRIMITIVE_ROT:
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPUSH(m_tos2);
      DPUSH(m_tos);
      m_tos = m_tos3;
      break;

      // tuck ( a b -- b a b ) swap over ;
    case FORTH_PRIMITIVE_TUCK:
      DPOP(m_tos2);
      DPUSH(m_tos);
      DPUSH(m_tos2);
      break;

      // 2dup ( a b -- a b a b ) over over ;
    case FORTH_PRIMITIVE_2DUP:
      DPUSH(m_tos);
      m_tos2 = DPICK(1);
      DPUSH(m_tos2);
      break;

      // 2over ( a b c d -- a b c d a b )
    case FORTH_PRIMITIVE_2OVER:
      DPUSH(m_tos);
      m_tos2 = DPICK(3);
      DPUSH(m_tos2);
      m_tos = DPICK(3);
      break;

      // 2swap ( a b c d -- c d a b )
    case FORTH_PRIMITIVE_2SWAP:
      DPOP(m_tos1);
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPUSH(m_tos1);
      DPUSH(m_tos);
      DPUSH(m_tos3);
      m_tos = m_tos2;
      break;

      // 2drop ( a b -- ) drop drop ;
    case FORTH_PRIMITIVE_2DROP:
      DPOP(m_tos);
      DPOP(m_tos);
      break;

      //
    case FORTH_PRIMITIVE_BINARY:
      changeDisplayBase(2U);
      break;
    case FORTH_PRIMITIVE_OCTAL:
      changeDisplayBase(8U);
      break;
    case FORTH_PRIMITIVE_HEXADECIMAL:
      changeDisplayBase(16U);
      break;
    case FORTH_PRIMITIVE_DECIMAL:
      changeDisplayBase(10U);
      break;
    case FORTH_PRIMITIVE_GET_BASE:
      DPUSH(m_tos);
      m_tos = m_base;
      break;
    case FORTH_PRIMITIVE_SET_BASE:
      if (false == changeDisplayBase(m_tos))
        {
          std::cerr << FORTH_WARNING_COLOR << "[WARNING] '"
                    << m_tos << "' is an invalid base and shall be [2..36]. Ignored !"
                    << FORTH_NORMAL_COLOR << std::endl;
        }
      break;

    case FORTH_PRIMITIVE_NEGATE:
      m_tos = -m_tos;
      break;
    case FORTH_PRIMITIVE_ABS:
      m_tos = std::abs((int32_t) m_tos);
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
    case FORTH_PRIMITIVE_FALSE:
      m_tos = 0;
      DPUSH(m_tos);
      break;
    case FORTH_PRIMITIVE_TRUE:
      m_tos = -1;
      DPUSH(m_tos);
      break;
    case FORTH_PRIMITIVE_GREATER_EQUAL:
      LOGICAL_OP(>=);
      break;
    case FORTH_PRIMITIVE_LOWER_EQUAL:
      LOGICAL_OP(<=);
      break;
    case FORTH_PRIMITIVE_GREATER:
      LOGICAL_OP(>);
      break;
    case FORTH_PRIMITIVE_LOWER:
      LOGICAL_OP(<);
      break;
    case FORTH_PRIMITIVE_EQUAL:
      LOGICAL_OP(==);
      break;
    case FORTH_PRIMITIVE_0EQUAL:
      DPUSH(0U);
      LOGICAL_OP(==);
      break;
    case FORTH_PRIMITIVE_NOT_EQUAL:
      LOGICAL_OP(!=);
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
    case FORTH_PRIMITIVE_MIN:
      DPOP(m_tos1);
      m_tos = ((int32_t) m_tos < (int32_t) m_tos1) ? m_tos : m_tos1;
      break;
    case FORTH_PRIMITIVE_MAX:
      DPOP(m_tos1);
      m_tos = ((int32_t) m_tos > (int32_t) m_tos1) ? m_tos : m_tos1;
      break;
    case FORTH_PRIMITIVE_DISP:
      std::cout << std::setbase(m_base) << (int32_t) m_tos << " ";
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_UDISP:
      std::cout << std::setbase(m_base) << (uint32_t) m_tos << " ";
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_CARRIAGE_RETURN:
      std::cout << std::endl;
      break;
    case FORTH_PRIMITIVE_DISPLAY_DSTACK:
      DPUSH(m_tos);
      displayStack(std::cout, forth::DataStack);
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_BEGIN_C_LIB:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.begin(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_END_C_LIB:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.end(/*nth*/); // FIXME
        if (res.first)
          {
            uint16_t i = 0;
            for (auto it: m_dynamic_libs/*[nth]*/.m_functions)
              {
                create(it.func_forth_name);
                m_dictionary.appendCell16(FORTH_PRIMITIVE_EXEC_C_FUNC);
                m_dictionary.appendCell16(i);
                m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
                ++i;
              }
          }
        else
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_EXEC_C_FUNC:
      DPUSH(m_tos);
      m_ip += 2U; // Skip the index of pointer function
      m_tos = m_dictionary.read16at(m_ip);
      //std::cout << "Pile avant: ";
      //displayStack(std::cout, forth::DataStack);

      m_dynamic_libs/*[nth]*/.m_functions[m_tos].fun_ptr(&m_dsp);
      //std::cout << "Pile apres: ";
      //displayStack(std::cout, forth::DataStack);
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_C_FUNCTION:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.function(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_C_CODE:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.code(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_ADD_EXT_C_LIB:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.extlib(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;
    default:
      UnknownForthPrimitive e(idPrimitive, __PRETTY_FUNCTION__); throw e;
      break;
    }
}

// **************************************************************
//! This method should be called after the contructor Forth()
//! and has been separated from the constructor to load the
//! dictionary at the desired moment.
// **************************************************************
void Forth::boot()
{
  LOGI("Forth booting ...");

  // TODO: Les ranger par ordre lexico ?
  // FIXME: init m_last et m_here pour etre sur que le client ne
  // charge pas plusieurs fois les memes primitives

  // Dummy word and comments
  m_dictionary.add(FORTH_PRIMITIVE_NOP, FORTH_DICO_ENTRY("NOOP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_LPARENTHESIS, FORTH_DICO_ENTRY("("), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_RPARENTHESIS, FORTH_DICO_ENTRY(")"), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_COMMENTARY, FORTH_DICO_ENTRY("\\"), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_INCLUDE, FORTH_DICO_ENTRY("INCLUDE"), 0);

  // Words for definitions
  m_dictionary.add(FORTH_PRIMITIVE_COLON, FORTH_DICO_ENTRY(":"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_SEMICOLON, FORTH_DICO_ENTRY(";"), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_PCREATE, FORTH_DICO_ENTRY("(CREATE)"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_CREATE, FORTH_DICO_ENTRY("CREATE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_BUILDS, FORTH_DICO_ENTRY("<BUILDS"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_DOES, FORTH_DICO_ENTRY("DOES>"), FLAG_IMMEDIATE);

  m_dictionary.add(FORTH_PRIMITIVE_IMMEDIATE, FORTH_DICO_ENTRY("IMMEDIATE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_SMUDGE, FORTH_DICO_ENTRY("SMUDGE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_STATE, FORTH_DICO_ENTRY("STATE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_TRACE_ON, FORTH_DICO_ENTRY("TRACE.ON"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_TRACE_OFF, FORTH_DICO_ENTRY("TRACE.OFF"), 0);

  // Words
  m_dictionary.add(FORTH_PRIMITIVE_TICK, FORTH_DICO_ENTRY("'"), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_ABORT, FORTH_DICO_ENTRY("ABORT"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_EXECUTE, FORTH_DICO_ENTRY("EXECUTE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMPILE, FORTH_DICO_ENTRY("COMPILE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_ICOMPILE, FORTH_DICO_ENTRY("[COMPILE]"), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_POSTPONE, FORTH_DICO_ENTRY("POSTPONE"), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_LBRACKET, FORTH_DICO_ENTRY("["), FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_RBRACKET, FORTH_DICO_ENTRY("]"), 0);

  // Dictionnary manipulation
  m_dictionary.add(FORTH_PRIMITIVE_LAST, FORTH_DICO_ENTRY("LAST"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_HERE, FORTH_DICO_ENTRY("HERE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_ALLOT, FORTH_DICO_ENTRY("ALLOT"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMMA32, FORTH_DICO_ENTRY(","), 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMMA16, FORTH_DICO_ENTRY("S,"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMMA8, FORTH_DICO_ENTRY("C,"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_FETCH, FORTH_DICO_ENTRY("@"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_STORE32, FORTH_DICO_ENTRY("!"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_STORE16, FORTH_DICO_ENTRY("S!"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_STORE8, FORTH_DICO_ENTRY("C!"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_CMOVE, FORTH_DICO_ENTRY("CMOVE"), 0);

  // Words changing IP
  m_dictionary.add(FORTH_PRIMITIVE_EXIT, FORTH_DICO_ENTRY("EXIT"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_BRANCH, FORTH_DICO_ENTRY("BRANCH"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_0BRANCH, FORTH_DICO_ENTRY("0BRANCH"), 0);

  // Return Stack
  m_dictionary.add(FORTH_PRIMITIVE_TO_RSTACK, FORTH_DICO_ENTRY(">R"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_FROM_RSTACK, FORTH_DICO_ENTRY("R>"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2TO_RSTACK, FORTH_DICO_ENTRY("2>R"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2FROM_RSTACK, FORTH_DICO_ENTRY("2R>"), 0);

  // Loop
  //m_dictionary.add(FORTH_PRIMITIVE_DO, FORTH_DICO_ENTRY("(DO)"), 0);
  //m_dictionary.add(FORTH_PRIMITIVE_LOOP, FORTH_DICO_ENTRY("(LOOP)"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_I, FORTH_DICO_ENTRY("I"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_J, FORTH_DICO_ENTRY("J"), 0);

  // cell sizeof
  m_dictionary.add(FORTH_PRIMITIVE_CELL, FORTH_DICO_ENTRY("CELL"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_CELLS, FORTH_DICO_ENTRY("CELLS"), 0);

  // Literals
  m_dictionary.add(FORTH_PRIMITIVE_LITERAL_16, FORTH_DICO_ENTRY("LITERAL16"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_LITERAL_32, FORTH_DICO_ENTRY("LITERAL32"), 0);

  // Arithmetic
  m_dictionary.add(FORTH_PRIMITIVE_ABS, FORTH_DICO_ENTRY("ABS"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_NEGATE, FORTH_DICO_ENTRY("NEGATE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_MIN, FORTH_DICO_ENTRY("MIN"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_MAX, FORTH_DICO_ENTRY("MAX"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_PLUS, FORTH_DICO_ENTRY("+"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_1PLUS, FORTH_DICO_ENTRY("1+"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2PLUS, FORTH_DICO_ENTRY("2+"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_MINUS, FORTH_DICO_ENTRY("-"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_1MINUS, FORTH_DICO_ENTRY("1-"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2MINUS, FORTH_DICO_ENTRY("2-"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_TIMES, FORTH_DICO_ENTRY("*"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_DIV, FORTH_DICO_ENTRY("/"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_RSHIFT, FORTH_DICO_ENTRY(">>"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_LSHIFT, FORTH_DICO_ENTRY("<<"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_RSHIFT, FORTH_DICO_ENTRY("RSHIFT"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_LSHIFT, FORTH_DICO_ENTRY("LSHIFT"), 0);

  // Base
  m_dictionary.add(FORTH_PRIMITIVE_BINARY, FORTH_DICO_ENTRY("BIN"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_OCTAL, FORTH_DICO_ENTRY("OCTAL"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_HEXADECIMAL, FORTH_DICO_ENTRY("HEX"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_DECIMAL, FORTH_DICO_ENTRY("DECIMAL"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_GET_BASE, FORTH_DICO_ENTRY("BASE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_SET_BASE, FORTH_DICO_ENTRY("BASE!"), 0);

  // Logic
  m_dictionary.add(FORTH_PRIMITIVE_FALSE, FORTH_DICO_ENTRY("FALSE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_TRUE, FORTH_DICO_ENTRY("TRUE"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_GREATER, FORTH_DICO_ENTRY(">"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_GREATER_EQUAL, FORTH_DICO_ENTRY(">="), 0);
  m_dictionary.add(FORTH_PRIMITIVE_LOWER, FORTH_DICO_ENTRY("<"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_LOWER_EQUAL, FORTH_DICO_ENTRY("<="), 0);
  m_dictionary.add(FORTH_PRIMITIVE_EQUAL, FORTH_DICO_ENTRY("=="), 0);
  m_dictionary.add(FORTH_PRIMITIVE_EQUAL, FORTH_DICO_ENTRY("="), 0);
  m_dictionary.add(FORTH_PRIMITIVE_0EQUAL, FORTH_DICO_ENTRY("0="), 0);
  m_dictionary.add(FORTH_PRIMITIVE_NOT_EQUAL, FORTH_DICO_ENTRY("<>"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_NOT_EQUAL, FORTH_DICO_ENTRY("!="), 0);
  m_dictionary.add(FORTH_PRIMITIVE_AND, FORTH_DICO_ENTRY("AND"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_OR, FORTH_DICO_ENTRY("OR"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_XOR, FORTH_DICO_ENTRY("XOR"), 0);

  // Data Stack
  m_dictionary.add(FORTH_PRIMITIVE_DEPTH, FORTH_DICO_ENTRY("DEPTH"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_ROLL, FORTH_DICO_ENTRY("ROLL"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_NIP, FORTH_DICO_ENTRY("NIP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_PICK, FORTH_DICO_ENTRY("PICK"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_DUP, FORTH_DICO_ENTRY("DUP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_QDUP, FORTH_DICO_ENTRY("?DUP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_DROP, FORTH_DICO_ENTRY("DROP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_SWAP, FORTH_DICO_ENTRY("SWAP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_OVER, FORTH_DICO_ENTRY("OVER"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_ROT, FORTH_DICO_ENTRY("ROT"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_TUCK, FORTH_DICO_ENTRY("TUCK"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2DUP, FORTH_DICO_ENTRY("2DUP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2DROP, FORTH_DICO_ENTRY("2DROP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2SWAP, FORTH_DICO_ENTRY("2SWAP"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_2OVER, FORTH_DICO_ENTRY("2OVER"), 0);

  // Printf
  m_dictionary.add(FORTH_PRIMITIVE_DISP, FORTH_DICO_ENTRY("."), 0);
  m_dictionary.add(FORTH_PRIMITIVE_UDISP, FORTH_DICO_ENTRY("U."), 0);
  m_dictionary.add(FORTH_PRIMITIVE_CARRIAGE_RETURN, FORTH_DICO_ENTRY("CR"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_DISPLAY_DSTACK, FORTH_DICO_ENTRY(".S"), 0);

  // C dynamic lib
  m_dictionary.add(FORTH_PRIMITIVE_BEGIN_C_LIB, FORTH_DICO_ENTRY("C-LIB"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_END_C_LIB, FORTH_DICO_ENTRY("END-C-LIB"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_ADD_EXT_C_LIB, FORTH_DICO_ENTRY("ADD-LIB"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_C_FUNCTION, FORTH_DICO_ENTRY("C-FUNCTION"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_C_CODE, FORTH_DICO_ENTRY("\\C"), 0);
  m_dictionary.add(FORTH_PRIMITIVE_EXEC_C_FUNC, FORTH_DICO_ENTRY("(EXEC-C)"), 0);

  // Hide some words to user
  m_dictionary.smudge("(CREATE)");
  m_dictionary.smudge("(EXEC-C)");

  m_last_completion = dictionary().last();
}

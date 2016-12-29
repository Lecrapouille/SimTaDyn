#include "Forth.hpp"

#  define BINARY_OP(op) { m_tos = ((int32_t) DDROP()) op ((int32_t) m_tos); } // Pop a value, apply it the operation op with the content of the register tos (Top Of Stack)
#  define LOGICAL_OP(op) { m_tos = -1 * (((int32_t) DDROP()) op ((int32_t) m_tos)); }

// **************************************************************
//! Called by Forth words needed to extract the next word. The
//! stream shall contains at least one word else an exception is
//! triggered.
//! \return the next Forth word in the stream.
//! \throw UnfinishedStream
// **************************************************************
std::string Forth::nextWord()
{
  if (!STREAM.hasMoreWords())
    {
      UnfinishedStream e(m_state);
      throw e;
    }
  return STREAM.nextWord();
}

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
      m_state = COMMENT_STATE;
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
      m_state = COMPILATION_STATE;
      break;

      // End the definition of a new word
    case FORTH_PRIMITIVE_SEMICOLON:
      m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
      m_state = INTERPRETER_STATE;
      if (m_depth_at_colon != DStackDepth())
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
        std::string word = nextWord();
        if (false == m_dictionary.smudge(word))
          {
            m_color.yellow();
            std::cout << m_color << "[WARNING] Unknown word '"
                      << word << "'. Word SMUDGE Ignored !";
            m_color.normal();
            std::cout << m_color << std::endl;
          }
      }
      break;

    case FORTH_PRIMITIVE_TICK:
      {
        std::string word = nextWord();
        Cell16 token;
        bool immediate;
        if (false == m_dictionary.find(word, token, immediate))
          {
            token = 0;
            m_color.yellow();
            std::cout << m_color << "[WARNING] Unknown word '"
                      << word << "'. Word TICK Ignored !";
            m_color.normal();
            std::cout << m_color << std::endl;
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
         std::string word = nextWord();

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
        std::string word = nextWord();

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
       m_state = INTERPRETER_STATE;
       break;

    case FORTH_PRIMITIVE_RBRACKET:
       m_state = COMPILATION_STATE;
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
      m_ip += 2U; // Skip primitive LITTERAL
      m_tos = m_dictionary.read16at(m_ip);
      break;

    case FORTH_PRIMITIVE_LITERAL_32:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITTERAL
      m_tos = m_dictionary.read32at(m_ip);
      m_ip += 2U; // Skip the number - 2 because of next ip
      break;

    case FORTH_PRIMITIVE_1MINUS:
      m_tos--;
      break;
    case FORTH_PRIMITIVE_1PLUS:
      m_tos++;
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
      m_tos = DStackDepth();
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
        m_dsp++;
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
    case FORTH_PRIMITIVE_BASE:
      if (DStackDepth() < 0)
        {
          DPUSH(m_tos);
          m_tos = m_base;
        }
      else if (false == changeDisplayBase(m_tos))
        {
          m_color.yellow();
          std::cerr << m_color << "[WARNING] '"
                    << m_tos << "' is an invalid base and shall be [2..36]. Ignored !";
          m_color.normal();
          std::cerr << m_color << std::endl;
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
      displayDStack();
      DPOP(m_tos);
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
  // TODO: Les ranger par ordre lexico ?
  // FIXME: init m_last et m_here pour etre sur que le client ne
  // charge pas plusieurs fois les memes primitives

  // Dummy word and comments
  m_dictionary.add(FORTH_PRIMITIVE_NOP, "NOOP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_LPARENTHESIS, "(", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_RPARENTHESIS, ")", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_COMMENTARY, "\\", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_INCLUDE, "INCLUDE", 0);

  // Words for definitions
  m_dictionary.add(FORTH_PRIMITIVE_COLON, ":", 0);
  m_dictionary.add(FORTH_PRIMITIVE_SEMICOLON, ";", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_PCREATE, "(CREATE)", 0);
  m_dictionary.add(FORTH_PRIMITIVE_CREATE, "CREATE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_BUILDS, "<BUILDS", 0);
  m_dictionary.add(FORTH_PRIMITIVE_DOES, "DOES>", FLAG_IMMEDIATE);

  m_dictionary.add(FORTH_PRIMITIVE_IMMEDIATE, "IMMEDIATE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_SMUDGE, "SMUDGE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_STATE, "STATE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_TRACE_ON, "TRACE.ON", 0);
  m_dictionary.add(FORTH_PRIMITIVE_TRACE_OFF, "TRACE.OFF", 0);

  // Words
  m_dictionary.add(FORTH_PRIMITIVE_TICK, "'", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_ABORT, "ABORT", 0);
  m_dictionary.add(FORTH_PRIMITIVE_EXECUTE, "EXECUTE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMPILE, "COMPILE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_ICOMPILE, "[COMPILE]", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_POSTPONE, "POSTPONE", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_LBRACKET, "[", FLAG_IMMEDIATE);
  m_dictionary.add(FORTH_PRIMITIVE_RBRACKET, "]", 0);

  // Dictionnary manipulation
  m_dictionary.add(FORTH_PRIMITIVE_LAST, "LAST", 0);
  m_dictionary.add(FORTH_PRIMITIVE_HERE, "HERE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_ALLOT, "ALLOT", 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMMA32, ",", 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMMA16, "S,", 0);
  m_dictionary.add(FORTH_PRIMITIVE_COMMA8, "C,", 0);
  m_dictionary.add(FORTH_PRIMITIVE_FETCH, "@", 0);
  m_dictionary.add(FORTH_PRIMITIVE_STORE32, "!", 0);
  m_dictionary.add(FORTH_PRIMITIVE_STORE16, "S!", 0);
  m_dictionary.add(FORTH_PRIMITIVE_STORE8, "C!", 0);
  m_dictionary.add(FORTH_PRIMITIVE_CMOVE, "CMOVE", 0);

  // Words changing IP
  m_dictionary.add(FORTH_PRIMITIVE_EXIT, "EXIT", 0);
  m_dictionary.add(FORTH_PRIMITIVE_BRANCH, "BRANCH", 0);
  m_dictionary.add(FORTH_PRIMITIVE_0BRANCH, "0BRANCH", 0);

  // Return Stack
  m_dictionary.add(FORTH_PRIMITIVE_TO_RSTACK, ">R", 0);
  m_dictionary.add(FORTH_PRIMITIVE_FROM_RSTACK, "R>", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2TO_RSTACK, "2>R", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2FROM_RSTACK, "2R>", 0);
  m_dictionary.add(FORTH_PRIMITIVE_I, "I", 0);
  m_dictionary.add(FORTH_PRIMITIVE_J, "J", 0);

  // cell sizeof
  m_dictionary.add(FORTH_PRIMITIVE_CELL, "CELL", 0);
  m_dictionary.add(FORTH_PRIMITIVE_CELLS, "CELLS", 0);

  // Literals
  m_dictionary.add(FORTH_PRIMITIVE_LITERAL_16, "LITERAL16", 0);
  m_dictionary.add(FORTH_PRIMITIVE_LITERAL_32, "LITERAL32", 0);

  // Arithmetic
  m_dictionary.add(FORTH_PRIMITIVE_ABS, "ABS", 0);
  m_dictionary.add(FORTH_PRIMITIVE_NEGATE, "NEGATE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_MIN, "MIN", 0);
  m_dictionary.add(FORTH_PRIMITIVE_MAX, "MAX", 0);
  m_dictionary.add(FORTH_PRIMITIVE_PLUS, "+", 0);
  m_dictionary.add(FORTH_PRIMITIVE_1PLUS, "1+", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2PLUS, "2+", 0);
  m_dictionary.add(FORTH_PRIMITIVE_MINUS, "-", 0);
  m_dictionary.add(FORTH_PRIMITIVE_1MINUS, "1-", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2MINUS, "2-", 0);
  m_dictionary.add(FORTH_PRIMITIVE_TIMES, "*", 0);
  m_dictionary.add(FORTH_PRIMITIVE_DIV, "/", 0);
  m_dictionary.add(FORTH_PRIMITIVE_RSHIFT, ">>", 0);
  m_dictionary.add(FORTH_PRIMITIVE_LSHIFT, "<<", 0);
  m_dictionary.add(FORTH_PRIMITIVE_RSHIFT, "RSHIFT", 0);
  m_dictionary.add(FORTH_PRIMITIVE_LSHIFT, "LSHIFT", 0);

  // Base
  m_dictionary.add(FORTH_PRIMITIVE_BINARY, "BIN", 0);
  m_dictionary.add(FORTH_PRIMITIVE_OCTAL, "OCTAL", 0);
  m_dictionary.add(FORTH_PRIMITIVE_HEXADECIMAL, "HEX", 0);
  m_dictionary.add(FORTH_PRIMITIVE_DECIMAL, "DECIMAL", 0);
  m_dictionary.add(FORTH_PRIMITIVE_BASE, "BASE", 0);

  // Logic
  m_dictionary.add(FORTH_PRIMITIVE_FALSE, "FALSE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_TRUE, "TRUE", 0);
  m_dictionary.add(FORTH_PRIMITIVE_GREATER, ">", 0);
  m_dictionary.add(FORTH_PRIMITIVE_GREATER_EQUAL, ">=", 0);
  m_dictionary.add(FORTH_PRIMITIVE_LOWER, "<", 0);
  m_dictionary.add(FORTH_PRIMITIVE_LOWER_EQUAL, "<=", 0);
  m_dictionary.add(FORTH_PRIMITIVE_EQUAL, "==", 0);
  m_dictionary.add(FORTH_PRIMITIVE_EQUAL, "=", 0);
  m_dictionary.add(FORTH_PRIMITIVE_0EQUAL, "0=", 0);
  m_dictionary.add(FORTH_PRIMITIVE_NOT_EQUAL, "<>", 0);
  m_dictionary.add(FORTH_PRIMITIVE_NOT_EQUAL, "!=", 0);
  m_dictionary.add(FORTH_PRIMITIVE_AND, "AND", 0);
  m_dictionary.add(FORTH_PRIMITIVE_OR, "OR", 0);
  m_dictionary.add(FORTH_PRIMITIVE_XOR, "XOR", 0);

  // Data Stack
  m_dictionary.add(FORTH_PRIMITIVE_DEPTH, "DEPTH", 0);
  m_dictionary.add(FORTH_PRIMITIVE_ROLL, "ROLL", 0);
  m_dictionary.add(FORTH_PRIMITIVE_NIP, "NIP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_PICK, "PICK", 0);
  m_dictionary.add(FORTH_PRIMITIVE_DUP, "DUP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_QDUP, "?DUP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_DROP, "DROP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_SWAP, "SWAP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_OVER, "OVER", 0);
  m_dictionary.add(FORTH_PRIMITIVE_ROT, "ROT", 0);
  m_dictionary.add(FORTH_PRIMITIVE_TUCK, "TUCK", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2DUP, "2DUP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2DROP, "2DROP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2SWAP, "2SWAP", 0);
  m_dictionary.add(FORTH_PRIMITIVE_2OVER, "2OVER", 0);

  // Printf
  m_dictionary.add(FORTH_PRIMITIVE_DISP, ".", 0);
  m_dictionary.add(FORTH_PRIMITIVE_UDISP, "U.", 0);
  m_dictionary.add(FORTH_PRIMITIVE_CARRIAGE_RETURN, "CR", 0);
  m_dictionary.add(FORTH_PRIMITIVE_DISPLAY_DSTACK, ".S", 0);

  // Hide some words to user
  m_dictionary.smudge("(CREATE)");

  m_last_completion = dictionary().last();
}

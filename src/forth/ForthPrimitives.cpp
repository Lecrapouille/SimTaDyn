#include "ForthInner.hpp"

// **************************************************************
//
// **************************************************************
std::string Forth::getWord()
{
  if (!m_reader.hasMoreWords())
    {
      ForthReaderTruncatedFile e(m_reader.file());
      throw e;
    }
  return m_reader.nextWord();
}

// **************************************************************
//
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
      m_reader.skipLine();
      break;

      // Begin the definition of a new word
    case FORTH_PRIMITIVE_COLON:
      {
        // Extract next word
        m_creating_word = getWord();
        if (m_dico.exists(m_creating_word))
          {
            std::cout << YELLOW << "[WARNING] Redefining '" << m_creating_word << "'" << DEFAULT << std::endl;
          }

        // Save informations which will be checked
        // when executing the SEMI_COLON primitive.
        m_last_at_colon = m_dico.m_last;
        m_here_at_colon = m_dico.m_here;
        m_depth_at_colon = DStackDepth();

        // Add it in the dictionary
        Cell16 token = m_dico.here() + m_creating_word.size() + 1U + 2U; // 1: flags, 2: NFA
        m_dico.add(token, m_creating_word, 0);

        // Compilation mode
        m_state = COMPILATION_STATE;
      }
      break;

      // End the definition of a new word
    case FORTH_PRIMITIVE_SEMICOLON:
      m_dico.appendCell16(FORTH_PRIMITIVE_EXIT); // FIXME check size word definition
      m_state = EXECUTION_STATE;
      if (m_depth_at_colon != DStackDepth())
        {
          m_dico.m_last = m_last_at_colon;
          m_dico.m_here = m_here_at_colon;
          ForthUnbalancedDef e(m_creating_word); throw e;
        }
      break;

      // Set immediate the last word
    case FORTH_PRIMITIVE_IMMEDIATE:
      m_dico.m_dictionary[m_dico.m_last] |= FLAG_IMMEDIATE;
      break;

      // Restore the IP when interpreting the definition
      // of a non primitive word
    case FORTH_PRIMITIVE_EXIT:
      RPOP16(m_ip);
      break;

      // Change IP
    case FORTH_PRIMITIVE_BRANCH:
      m_ip = CADDR(ADDR8(m_ip) +  m_dico.read16at(ADDR8(m_ip)));
      break;

      // Change IP if top of stack is 0
    case FORTH_PRIMITIVE_0BRANCH:
      if (0 == m_tos)
        {
          m_ip = CADDR(ADDR8(m_ip) +  m_dico.read16at(ADDR8(m_ip)));
        }
      else
        {
          ++m_ip;
        }
      DDROP();
      break;

      // Move x to the return stack.
      // ( x -- ) ( R: -- x )
    case FORTH_PRIMITIVE_TO_RSTACK:
      RPUSH(m_tos);
      DDROP();
      break;

      // Transfer cell pair x1 x2 to the return stack
      // ( x1 x2 -- ) ( R: -- x1 x2 )
    case FORTH_PRIMITIVE_2TO_RSTACK:
      DPOP(m_tos1);
      RPUSH(m_tos1);
      RPUSH(m_tos);
      DDROP();
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

    case FORTH_PRIMITIVE_CELL:
      DPUSH(m_tos);
      m_tos = sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_CELLS:
      m_tos = m_tos * sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_LITERAL:
      DPUSH(m_tos);
      ++m_ip; // Skip primitive LITTERAL
      m_tos = m_dico.read32at(ADDR8(m_ip));
      std::cout << "LIT " << std::hex << m_tos << std::endl;
      m_ip += (4U / sizeof (m_ip)); // Skip the number
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
      DDROP();
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
    case FORTH_PRIMITIVE_TUK:
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
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPOP(m_tos4);
      DPUSH(m_tos3);
      DPUSH(m_tos4);
      DPUSH(m_tos2);
      break;

      // 2drop ( a b -- ) drop drop ;
    case FORTH_PRIMITIVE_2DROP:
      DDROP();
      DPOP(m_tos);
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
    case FORTH_PRIMITIVE_NOT_EQUAL:
      BINARY_OP(!=);
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
      m_tos = (m_tos < m_tos1) ? m_tos : m_tos1;
      break;
    case FORTH_PRIMITIVE_MAX:
      DPOP(m_tos1);
      m_tos = (m_tos > m_tos1) ? m_tos : m_tos1;
      break;
    case FORTH_PRIMITIVE_DISP:
      std::cout << std::setbase(m_base) << m_tos << " ";
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_CARRIAGE_RETURN:
      std::cout << std::endl;
      break;
    default:
      ForthUnknownPrimitive e(idPrimitive, __PRETTY_FUNCTION__); throw e;
      break;
    }
}

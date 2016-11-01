#include "ForthInner.hpp"

// **************************************************************
//
// **************************************************************
void Forth::execPrimitive(const Cell16 idPrimitive)
{
  //std::cout << "execPrimitive " << idPrimitive << std::endl;
  switch (idPrimitive)
    {
    case FORTH_PRIMITIVE_NOP:
      std::cout << "NOP\n";
      break;

    case FORTH_PRIMITIVE_LPARENTHESIS:
      m_saved_state = m_state;
      m_state = COMMENT_STATE;
      break;

    case FORTH_PRIMITIVE_RPARENTHESIS:
      m_state = m_saved_state;
      break;

    case FORTH_PRIMITIVE_COMMENTARY:
      m_reader.skipLine();
      break;

    case FORTH_PRIMITIVE_COLON:
      {
        DPUSH(m_tos);
        m_state = COMPILATION_STATE;
        //mode++;

        // FIXME: afficher redifined
        std::string word;
        if (m_reader.nextWord(word))
          {
            Cell16 token = m_dico.here() + word.size() + 1U + 2U; // 1: flags, 2: NFA
            m_dico.add(token, word, 0); // TODO FLAG_SMUDGE);
          }
      }
      break;

    case FORTH_PRIMITIVE_EXIT:
      RPOP(m_ip);
      break;

    case FORTH_PRIMITIVE_SEMICOLON:
      DPUSH(m_tos);
      m_dico.appendCell16(FORTH_PRIMITIVE_EXIT); // FIXME check size wor definition
      // TODO clear FLAG_SMUDGE
      m_state = EXECUTION_STATE;
      //mode--;
      break;

    case FORTH_PRIMITIVE_LITERAL:
      DPUSH(m_tos);
      //std::cout << "------------\n";
      std::cerr << "TODO\n";//m_ip++; m_tos1 = m_dico.read16at(m_ip);
      std::cerr << "TODO\n";//m_ip++; m_tos2 = m_dico.read16at(m_ip);
      std::cerr << "TODO\n";//m_tos = m_tos1 * 65536U + m_tos2;
      //std::cout << m_tos << "\n------------\n";
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

      // nip ( a b -- b ) swap drop ;
    case FORTH_PRIMITIVE_NIP:
      DDROP();
      break;

      // dup ( a -- a a )
    case FORTH_PRIMITIVE_DUP:
      DPUSH(m_tos);
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
      std::cout << std::setbase(m_base) << m_tos << " ";//std::endl;
      DPOP(m_tos);
      break;
    default:
      ForthUnknownPrimitive e(idPrimitive, __PRETTY_FUNCTION__); throw e;
      break;
    }
}

#include "Forth.hpp"

// **************************************************************
//
// **************************************************************
void Forth::execPrimitive(const Cell16 idPrimitive)
{
  //std::cout << "execPrimitive " << idPrimitive << std::endl;
  switch (idPrimitive)
    {
    case FORTH_PRIMITIVE_NOP:
      std::cout << "PERDUUUUUUUU\n";
      break;

    case FORTH_PRIMITIVE_COLON:
      {
        DPUSH(tos);
        state = COMPILATION_STATE;
        //mode++;

        // FIXME: afficher redifined
        std::string word;
        if (reader.getNextWord(word))
          {
            Cell32 length = word.size();
            createDicoEntry(here + NEXT_MULTIPLE_OF_2(length + 1U) + 2U,
                            word, 0); // TODO FLAG_SMUDGE);
          }
      }
      break;

    case FORTH_PRIMITIVE_EXIT:
      RPOP(ip);
      break;

    case FORTH_PRIMITIVE_SEMICOLON:
      DPUSH(tos);
      dicoAppendCell16(FORTH_PRIMITIVE_EXIT);
      // TODO clear FLAG_SMUDGE
      state = EXECUTION_STATE;
      //mode--;
      break;

    case FORTH_PRIMITIVE_LITERAL:
      DPUSH(tos);
      //std::cout << "------------\n";
      ip++; tos1 = readCell16at(ip);
      ip++; tos2 = readCell16at(ip);
      tos = tos1 * 65536U + tos2;
      //std::cout << tos << "\n------------\n";
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
      std::cout << std::setbase(base) << tos << " ";//std::endl;
      DPOP(tos);
      break;
    default:
      ForthUnknownPrimitive e(idPrimitive, __PRETTY_FUNCTION__); throw e;
      break;
    }
}

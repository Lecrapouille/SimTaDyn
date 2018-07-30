//
CODE(BINARY)
changeDisplayBase(2U);
NEXT;

CODE(OCTAL)
changeDisplayBase(8U);
NEXT;

CODE(HEXADECIMAL)
changeDisplayBase(16U);
NEXT;

CODE(DECIMAL)
changeDisplayBase(10U);
NEXT;

CODE(GET_BASE)
DPUSH(m_base);
NEXT;

CODE(SET_BASE)
changeDisplayBase(DPOP());
NEXT;

// ----------------------------------------
CODE(DEPTH) DPUSH(m_data_stack.depth()); NEXT;

// ----------------------------------------
// ( ... n -- sp(n) )
CODE(PICK) DPUSH(DPICK(DPOP())); NEXT;

// ----------------------------------------
CODE(ROLL)
#if 0
m_tos = DPOP();
m_tos1 = DPICK(m_tos);
forth::cell* src = &DPICK(m_tos - 1);
forth::cell* dst = &DPICK(m_tos);
forth::cell ri = m_tos;
while (ri--)
  {
    *dst-- = *src--;
  }
m_tos = m_tos1;
++m_dsp;
DPUSH(m_tos);
#endif
NEXT;

// ----------------------------------------
// nip ( a b -- b ) swap drop
CODE(NIP) m_tos = DPOP(); DDROP(); DPUSH(m_tos); NEXT;

// ----------------------------------------
// swap ( a b -- b a )
CODE(SWAP)
m_tos2 = DPOP();
m_tos = DPOP();
DPUSH(m_tos2);
DPUSH(m_tos);
NEXT;

// ----------------------------------------
// over ( a b -- a b a )
CODE(OVER)
DPUSH(DPICK(1));
NEXT;

// ----------------------------------------
CODE(QDUP)
m_tos = DPOP();
if (m_tos) { DPUSH(m_tos); }
NEXT;

// ----------------------------------------
// dup ( a -- a a )
CODE(DUP) DPUSH(DPICK(0)); NEXT;

// ----------------------------------------
// drop ( a -- )
CODE(DROP) DDROP(); NEXT;

// ----------------------------------------
// 2drop ( a b -- ) drop drop
CODE(2DROP) DDROP(); DDROP(); NEXT;

// ----------------------------------------
CODE(AND) BINARY_I_OP(&); NEXT;

// ----------------------------------------
CODE(OR) BINARY_I_OP(|); NEXT;

// ----------------------------------------
CODE(XOR) BINARY_I_OP(^); NEXT;

// ----------------------------------------
CODE(PLUS) BINARY_I_OP(+); NEXT;

// ----------------------------------------
CODE(MINUS) BINARY_I_OP(-); NEXT;

// ----------------------------------------
CODE(DIV) BINARY_I_OP(/); NEXT;

// ----------------------------------------
CODE(TIMES) BINARY_I_OP(*); NEXT;

// ----------------------------------------
CODE(RSHIFT) BINARY_I_OP(>>); NEXT;

// ----------------------------------------
CODE(LSHIFT) BINARY_I_OP(<<); NEXT;

// ----------------------------------------
CODE(ABS) UNARY_I_FUN(std::abs); NEXT;

// ----------------------------------------
CODE(MIN) BINARY_I_FUN(std::min); NEXT;

// ----------------------------------------
CODE(MAX) BINARY_I_FUN(std::max); NEXT;

// ----------------------------------------
CODE(TRUE) DPUSH(-1); NEXT;

// ----------------------------------------
CODE(FALSE) DPUSH(0); NEXT;

// ----------------------------------------
CODE(GREATER_EQUAL) LOGICAL_I_OP(>=); NEXT;

// ----------------------------------------
CODE(GREATER) LOGICAL_I_OP(>); NEXT;

// ----------------------------------------
CODE(LOWER_EQUAL) LOGICAL_I_OP(<=); NEXT;

// ----------------------------------------
CODE(LOWER) LOGICAL_I_OP(<); NEXT;

// ----------------------------------------
CODE(0EQUAL) L_UNARY_I_OP(0==); NEXT;

// ----------------------------------------
CODE(EQUAL) LOGICAL_I_OP(==); NEXT;

// ----------------------------------------
CODE(NOT_EQUAL) LOGICAL_I_OP(!=); NEXT;

// ----------------------------------------
CODE(NEGATE) L_UNARY_I_OP(-); NEXT;

// ----------------------------------------
CODE(1MINUS) R_UNARY_I_OP(-1); NEXT;

// ----------------------------------------
CODE(1PLUS) R_UNARY_I_OP(+1); NEXT;

// ----------------------------------------
CODE(DISP)
//std::ios_base::fmtflags ifs(os.flags());
std::cout //<< std::setbase(m_base)
<< static_cast<forth::cell>(DPOP()) << " ";
//os.flags(ifs);
NEXT;

// ----------------------------------------
CODE(UDISP)
//std::ios_base::fmtflags ifs(os.flags());
std::cout //<< std::setbase(m_base)
<< static_cast<forth::ucell>(DPOP()) << " ";
//os.flags(ifs);
NEXT;

// ----------------------------------------
CODE(DISP_STACK)
//std::ios_base::fmtflags ifs(os.flags());
std::cout //<< std::setbase(m_base)
          << m_data_stack
          << std::endl;
//os.flags(ifs);
NEXT;

#if 0
    CODE(NEGATE)
      m_tos = -m_tos;
      NEXT;







      // Push in data stack the next free slot in the dictionary
    CODE(PCREATE)
      DPUSH(m_tos);
      m_tos = m_ip + 4U;
      NEXT;


    CODE(2MINUS)
      m_tos -= 2;
      NEXT;
    CODE(2PLUS)
      m_tos += 2;
      NEXT;

      // rot ( a b c -- b c a )
    CODE(ROT)
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPUSH(m_tos2);
      DPUSH(m_tos);
      m_tos = m_tos3;
      NEXT;

      // tuck ( a b -- b a b ) swap over ;
    CODE(TUCK)
      DPOP(m_tos2);
      DPUSH(m_tos);
      DPUSH(m_tos2);
      NEXT;

      // 2dup ( a b -- a b a b ) over over ;
    CODE(2DUP)
      DPUSH(m_tos);
      m_tos2 = DPICK(1);
      DPUSH(m_tos2);
      NEXT;

      // 2over ( a b c d -- a b c d a b )
    CODE(2OVER)
      DPUSH(m_tos);
      m_tos2 = DPICK(3);
      DPUSH(m_tos2);
      m_tos = DPICK(3);
      NEXT;

      // 2swap ( a b c d -- c d a b )
    CODE(2SWAP)
      DPOP(m_tos1);
      DPOP(m_tos2);
      DPOP(m_tos3);
      DPUSH(m_tos1);
      DPUSH(m_tos);
      DPUSH(m_tos3);
      m_tos = m_tos2;
      NEXT;
#endif

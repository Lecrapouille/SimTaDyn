#ifndef FORTH_PRIMITIVES_HPP_
#  define FORTH_PRIMITIVES_HPP_

// TODO: primitive INCLUDE
enum ForthPrimitives
  {
    // Dummy word and comments
    FORTH_PRIMITIVE_NOP = 0,
    FORTH_PRIMITIVE_LPARENTHESIS,
    FORTH_PRIMITIVE_RPARENTHESIS,
    FORTH_PRIMITIVE_COMMENTARY,

    // Words for definitions
    FORTH_PRIMITIVE_COLON,
    FORTH_PRIMITIVE_SEMICOLON,
    FORTH_PRIMITIVE_PCREATE,
    FORTH_PRIMITIVE_CREATE,
    FORTH_PRIMITIVE_BUILDS,
    FORTH_PRIMITIVE_DOES,
    FORTH_PRIMITIVE_IMMEDIATE,
    FORTH_PRIMITIVE_SMUDGE,
    FORTH_PRIMITIVE_STATE,

    // Words
    FORTH_PRIMITIVE_TICK,
    FORTH_PRIMITIVE_COMPILE,
    FORTH_PRIMITIVE_ICOMPILE,
    FORTH_PRIMITIVE_POSTPONE,
    FORTH_PRIMITIVE_EXECUTE,
    FORTH_PRIMITIVE_LBRACKET,
    FORTH_PRIMITIVE_RBRACKET,

    // Dictionnary manipulation
    FORTH_PRIMITIVE_LAST,
    FORTH_PRIMITIVE_HERE,
    FORTH_PRIMITIVE_ALLOT,
    FORTH_PRIMITIVE_COMMA8,
    FORTH_PRIMITIVE_COMMA16,
    FORTH_PRIMITIVE_COMMA32,
    FORTH_PRIMITIVE_FETCH,
    FORTH_PRIMITIVE_STORE8,
    FORTH_PRIMITIVE_STORE16,
    FORTH_PRIMITIVE_STORE32,

    // Words changing IP
    FORTH_PRIMITIVE_EXIT,
    FORTH_PRIMITIVE_BRANCH,
    FORTH_PRIMITIVE_0BRANCH,

    // Return Stack
    FORTH_PRIMITIVE_TO_RSTACK,
    FORTH_PRIMITIVE_FROM_RSTACK,
    FORTH_PRIMITIVE_2TO_RSTACK,
    FORTH_PRIMITIVE_2FROM_RSTACK,

    // cell sizeof
    FORTH_PRIMITIVE_CELL,
    FORTH_PRIMITIVE_CELLS,

    // Literals
    FORTH_PRIMITIVE_LITERAL_16,
    FORTH_PRIMITIVE_LITERAL_32,

    // Arithmetic
    FORTH_PRIMITIVE_ABS,
    FORTH_PRIMITIVE_NEGATE,
    FORTH_PRIMITIVE_MIN,
    FORTH_PRIMITIVE_MAX,
    FORTH_PRIMITIVE_PLUS,
    FORTH_PRIMITIVE_1PLUS,
    FORTH_PRIMITIVE_2PLUS,
    FORTH_PRIMITIVE_MINUS,
    FORTH_PRIMITIVE_1MINUS,
    FORTH_PRIMITIVE_2MINUS,
    FORTH_PRIMITIVE_TIMES,
    FORTH_PRIMITIVE_DIV,
    FORTH_PRIMITIVE_RSHIFT,
    FORTH_PRIMITIVE_LSHIFT,

    // Base
    FORTH_PRIMITIVE_BINARY,
    FORTH_PRIMITIVE_OCTAL,
    FORTH_PRIMITIVE_HEXADECIMAL,
    FORTH_PRIMITIVE_DECIMAL,
    FORTH_PRIMITIVE_BASE,

    // Logic
    FORTH_PRIMITIVE_FALSE,
    FORTH_PRIMITIVE_TRUE,
    FORTH_PRIMITIVE_GREATER,
    FORTH_PRIMITIVE_GREATER_EQUAL,
    FORTH_PRIMITIVE_LOWER,
    FORTH_PRIMITIVE_LOWER_EQUAL,
    FORTH_PRIMITIVE_EQUAL,
    FORTH_PRIMITIVE_0EQUAL,
    FORTH_PRIMITIVE_NOT_EQUAL,
    FORTH_PRIMITIVE_AND,
    FORTH_PRIMITIVE_OR,
    FORTH_PRIMITIVE_XOR,

    // Data Stack
    FORTH_PRIMITIVE_DEPTH,
    FORTH_PRIMITIVE_NIP,
    FORTH_PRIMITIVE_PICK,
    FORTH_PRIMITIVE_DUP,
    FORTH_PRIMITIVE_QDUP,
    FORTH_PRIMITIVE_DROP,
    FORTH_PRIMITIVE_SWAP,
    FORTH_PRIMITIVE_OVER,
    FORTH_PRIMITIVE_ROT,
    FORTH_PRIMITIVE_TUCK,
    FORTH_PRIMITIVE_2DUP,
    FORTH_PRIMITIVE_2SWAP,
    FORTH_PRIMITIVE_2DROP,
    FORTH_PRIMITIVE_2OVER,

    // Printf
    FORTH_PRIMITIVE_DISP,
    FORTH_PRIMITIVE_UDISP,
    FORTH_PRIMITIVE_CARRIAGE_RETURN,
    FORTH_PRIMITIVE_DISPLAY_DSTACK,

    // Files
    FORTH_PRIMITIVE_INCLUDE,

    NUM_PRIMITIVES
  };

// TODO: ASSERT_COMPILE_TIME(NUM_PRIMITIVES < sizeof (Cell16))

#endif /* FORTH_PRIMITIVES_HPP_ */

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

#ifndef FORTH_PRIMITIVES_HPP_
#  define FORTH_PRIMITIVES_HPP_

#define FORTH_DICO_ENTRY(a) a, ((sizeof a) - 1U)

enum ForthPrimitives
  {
    // Dummy word and comments
    FORTH_PRIMITIVE_NOP = 0,
    FORTH_PRIMITIVE_LPARENTHESIS,
    FORTH_PRIMITIVE_RPARENTHESIS,
    FORTH_PRIMITIVE_COMMENTARY,

    FORTH_PRIMITIVE_LOAD_SHARED_LIB,
    FORTH_PRIMITIVE_SYMBOL,

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
    FORTH_PRIMITIVE_TRACE_ON,
    FORTH_PRIMITIVE_TRACE_OFF,

    // Words
    FORTH_PRIMITIVE_TICK,
    FORTH_PRIMITIVE_COMPILE,
    FORTH_PRIMITIVE_ICOMPILE,
    FORTH_PRIMITIVE_POSTPONE,
    FORTH_PRIMITIVE_ABORT,
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
    FORTH_PRIMITIVE_CMOVE,

    // Words changing IP
    FORTH_PRIMITIVE_EXIT,
    FORTH_PRIMITIVE_BRANCH,
    FORTH_PRIMITIVE_0BRANCH,

    // Return Stack
    FORTH_PRIMITIVE_TO_RSTACK,
    FORTH_PRIMITIVE_FROM_RSTACK,
    FORTH_PRIMITIVE_2TO_RSTACK,
    FORTH_PRIMITIVE_2FROM_RSTACK,

    // Loops
    //FORTH_PRIMITIVE_DO,
    //FORTH_PRIMITIVE_LOOP,
    FORTH_PRIMITIVE_I,
    FORTH_PRIMITIVE_J,

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
    FORTH_PRIMITIVE_SET_BASE,
    FORTH_PRIMITIVE_GET_BASE,

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
    FORTH_PRIMITIVE_ROLL,
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

    // C lib
    FORTH_PRIMITIVE_BEGIN_C_LIB,
    FORTH_PRIMITIVE_END_C_LIB,
    FORTH_PRIMITIVE_ADD_EXT_C_LIB,
    FORTH_PRIMITIVE_C_FUNCTION,
    FORTH_PRIMITIVE_C_CODE,
    FORTH_PRIMITIVE_EXEC_C_FUNC,

    // Files
    FORTH_PRIMITIVE_INCLUDE,

    FORTH_MAX_PRIMITIVES
  };

#endif /* FORTH_PRIMITIVES_HPP_ */

#ifndef FORTH_HELPER_HPP_
#  define FORTH_HELPER_HPP_

#  include <fstream>
#  include <sstream>
#  include <iostream>
#  include <stdexcept>
#  include <exception>
#  include <cstring>
#  include <string>
#  include <iomanip>
#  include <utility>
#  include <algorithm>
#  include <assert.h>

// **************************************************************
//
// Virtual machine
//
// **************************************************************

// Architecture independent type
typedef unsigned int   uint32_t;
typedef signed int     int32_t;
typedef unsigned short uint16_t;
typedef signed short   int16_t;
typedef unsigned char  uint8_t;
typedef signed char    int8_t;

// Forth Type
typedef uint32_t       Cell32;
typedef uint16_t       Cell16;
typedef uint8_t        Cell8;

// Memory cast
#  define ADDR8(x)       (reinterpret_cast<Cell8*>(x))
#  define CELL8(x)       (reinterpret_cast<Cell8>(x))
#  define CELL16(x)      (reinterpret_cast<Cell16>(x))
#  define CELL32(x)      (reinterpret_cast<Cell32>(x))
#  define CADDR(x)       (reinterpret_cast<Cell16*>(x))

#  define TOKEN2ADDR(a) (CADDR(a + dictionary))

//! Memory for data stack, return stack
#  define STACK_SIZE     (1024U) // of Cells

//! Rerserve additional bytes to detect stack underflows and prevent
//! data corruption.
#  define STACK_UNDERFLOW_MARGIN (8U) // bytes
//ASSERT_COMPILE_TIME(STACK_UNDERFLOW_MARGIN < STACK_SIZE)

// Memory for the dictionnary
// 64 Ko is the maximal reachable address with a 16-bits word
// FIXME: Maybe we can divide / 2 addresses because they are aligned.
#  define DICTIONARY_SIZE (64U * 1024U) // of bytes

//
#  define CELL16_MAX_VALUE (65535U)
// FIXME: COMPIL_ASSERT_TIME((DICTIONARY_SIZE - 1U) <= CELL16_MAX_VALUE)

// Used for aligning 32-bits addresses
#  define NEXT_MULTIPLE_OF_4(x) (((x) + 3) & ~0x03)

// Used for aligning 16-bits addresses
#  define NEXT_MULTIPLE_OF_2(x) (((x) + 1) & ~0x01)

// Padding
#define UINT32_MASK  ((sizeof (Cell32) - 1))
#define UINT16_MASK  ((sizeof (Cell16) - 1))

// **************************************************************
//
// Stack manipulation
//
// **************************************************************

// Data stack (function parameters manipulation)
#  define DPUSH(n) (*(m_dsp++) = CELL32(n))  // Store the number n on the top of stack
#  define DDROP()  (*(--m_dsp))              // Discard the top of the stack
#  define DPOP(r)  (r = DDROP())             // Discard the top of the stack, save its value in the register r
#  define DPICK(n) (*(m_dsp - n - 1))        // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)

// Alternative data stack (function parameters manipulation)
#  define APUSH(n) (*(m_asp++) = CELL32(n))  // Store the number n on the top of stack
#  define ADROP()  (*(--m_asp))              // Discard the top of the stack
#  define APOP(r)  (r = ADROP())             // Discard the top of the stack, save its value in the register

// Return stack (store calling functions (tokens))
#  define RPUSH(a)  (*(m_rsp++) = CELL32(a)) // Store an address a on the top of stack
#  define RPOP(r)   (r = *(--m_rsp))         // Discard the top of the stack
#  define RPICK(n)  (*(m_rsp - n - 1))       // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)

// **************************************************************
//
// Forth word flags
//
// **************************************************************

#  define FLAG_PRECEDENCE (0x80) // A word has always this bit set (historical)
#  define FLAG_IMMEDIATE  (0x40) // A word immediate is interpreted during the compilation
#  define FLAG_SMUDGE     (0x20) // When set this word is forgotten by the dictionnary

#  define MASK_FORTH_NAME_SIZE  (0x1F) // 5 first bits are used to store the word name length
#  define MASK_FORTH_FLAGS      (0x7F)

// **************************************************************
//
// Forth mode
//
// **************************************************************
#  define INTERPRETER_STATE  (0U)
#  define COMPILATION_STATE  (1U)
#  define COMMENT_STATE      (2U)

// **************************************************************
//
// Enum used when an exception occured
//
// **************************************************************
#  define DATA_STACK         (0U)
#  define RETURN_STACK       (1U)

// **************************************************************
// Stream
// **************************************************************
#define STREAM m_streams_stack[m_opened_streams] // current stream parsed
#define MAX_OPENED_STREAMS   (16U)    // depth of INCLUDE calls

// Dictionnary display
#  define WORD_GROUPING 6

//#  define DEBUG(txt) do { std::cout << txt << std::endl; } while (0)
//#  define DEBUG3(a, txt, b) do { std::cout << a << txt << b << std::endl; } while (0)

// **************************************************************
//
// **************************************************************
#define SWAP32(x) (((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | ((x) & 0xff0000) >> 8 | ((x) >> 24) & 0xff)
#define SWAP16(x) (((x) & 0xff) << 8 | (((x) & 0xff00) >> 8))

#endif /* FORTH_HELPER_HPP_ */

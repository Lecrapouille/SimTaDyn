#ifndef FORTHHELPER_HPP_
#  define FORTHHELPER_HPP_

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
#define ADDR8(x)       (reinterpret_cast<Cell8*>(x))
#define CELL8(x)       (reinterpret_cast<Cell8>(x))
#define CELL16(x)      (reinterpret_cast<Cell16>(x))
#define CELL32(x)      (reinterpret_cast<Cell32>(x))
#define CADDR(x)       (reinterpret_cast<Cell16*>(x))

// Memory for data stack, return stack
#define STACK_SIZE     (1024U) // of Cells

// Memory for the dictionnary
// 64 Ko is the maximal reachable address with a 16-bits word
// FIXME: Maybe we can divide / 2 addresses because they are aligned.
#define DATASPACE_SIZE (64U * 1024U) // of bytes

//
#define CELL16_MAX_VALUE (65535U)
// FIXME: COMPIL_ASSERT_TIME((DATASPACE_SIZE - 1U) <= CELL16_MAX_VALUE)

// Used for aligning 32-bits addresses
#define NEXT_MULTIPLE_OF_4(x) (((x) + 3) & ~0x03)

// Used for aligning 16-bits addresses
#define NEXT_MULTIPLE_OF_2(x) (((x) + 1) & ~0x01)

// Spare byte
#define SPARE_VALUE   (0xFF)

// **************************************************************
//
// Stack manipulation
//
// **************************************************************

// Data stack (function parameters manipulation)
#define DPUSH(n) (*(dsp++) = CELL32(n))  // Store the number n on the top of stack
#define DDROP()  (*(--dsp))              // Discard the top of the stack
#define DPOP(r)  (r = DDROP())           // Discard the top of the stack, save its value in the register r
#define DPICK(n) (*(dsp - n - 1))        // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)
#define BINARY_OP(op) { tos = DDROP() op tos; } // Pop a value, apply it the operation op with the content of the register tos (Top Of Stack)

// Return stack (store calling functions)
#define RPUSH(a) (*(rsp++) = CADDR(a))   // Store an address a on the top of stack
#define RPOP(r)  (r = *(--rsp))          // Discard the top of the stack

// **************************************************************
//
// Forth word flags
//
// **************************************************************

#define FLAG_PRECEDENCE (0x80) // A word has always this bit set (historical)
#define FLAG_IMMEDIATE  (0x40) // A word immediate is interpreted during the compilation
#define FLAG_SMUDGE     (0x20) // When set this word is forgotten by the dictionnary

#define MASK_FORTH_NAME_SIZE  (0x1F) // 5 first bits are used to store the word name length
#define MASK_FORTH_FLAGS      (0x7F)

// **************************************************************
//
// Forth mode
//
// **************************************************************
#define EXECUTION_STATE    (0U)
#define COMPILATION_STATE  (1U)

// **************************************************************
//
// Other
//
// **************************************************************

//#define DEBUG(txt) do { std::cout << txt << std::endl; } while (0)
//#define DEBUG3(a, txt, b) do { std::cout << a << txt << b << std::endl; } while (0)

#endif /* FORTHHELPER_HPP_ */

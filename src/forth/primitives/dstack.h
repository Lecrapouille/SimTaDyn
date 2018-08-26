// Define a Forth binary operation for integers.
// This will reduce code size.
#define BINARY_I_FUN(op)                         \
  DPUSH(op(DPOP(), DPOP()))

// Define a Forth unary operation for integers.
// This will reduce code size.
#define UNARY_I_FUN(op)                          \
  DPUSH(op(DPOP()))

// Define a Forth binary operation for integers.
// This will reduce code size.
#define BINARY_I_OP(op)                         \
  m_tos = DPOP(); DPUSH(DPOP() op m_tos)

// Define a Forth unary operation for integers.
// This will reduce code size.
#define L_UNARY_I_OP(op)                        \
  DPUSH(op DPOP())

// Define a Forth unary operation for integers.
// This will reduce code size.
#define R_UNARY_I_OP(op)                        \
  DPUSH(DPOP() op)

#define LOGICAL_I_OP(op)                        \
  m_tos = DPOP(); DPUSH((DPOP() op m_tos) ? -1 : 0)

#define ALL_DSTACK_TOKENS                       \
  TOK_BINARY,                                   \
  TOK_OCTAL,                                    \
  TOK_HEXADECIMAL,                              \
  TOK_DECIMAL,                                  \
  TOK_GET_BASE,                                 \
  TOK_SET_BASE,                                 \
  TOK_DEPTH,                                    \
  TOK_PICK,                                     \
  TOK_ROLL,                                     \
  TOK_NIP,                                      \
  TOK_SWAP,                                     \
  TOK_OVER,                                     \
  TOK_QDUP,                                     \
  TOK_DUP,                                      \
  TOK_DROP,                                     \
  TOK_2DROP,                                    \
  TOK_AND,                                      \
  TOK_OR,                                       \
  TOK_XOR,                                      \
  TOK_PLUS,                                     \
  TOK_MINUS,                                    \
  TOK_DIV,                                      \
  TOK_TIMES,                                    \
  TOK_RSHIFT,                                   \
  TOK_LSHIFT,                                   \
  TOK_ABS,                                      \
  TOK_MIN,                                      \
  TOK_MAX,                                      \
  TOK_TRUE,                                     \
  TOK_FALSE,                                    \
  TOK_GREATER_EQUAL,                            \
  TOK_GREATER,                                  \
  TOK_LOWER_EQUAL,                              \
  TOK_LOWER,                                    \
  TOK_EQUAL,                                    \
  TOK_ZEROEQUAL,                                \
  TOK_NOT_EQUAL,                                \
  TOK_NEGATE,                                   \
  TOK_1MINUS,                                   \
  TOK_1PLUS,                                    \
  TOK_DISP,                                     \
  TOK_UDISP,                                    \
  TOK_DISP_STACK

//FIXME Peut on mettre ca dans PRIMITIVE ?
#define ALL_DSTACK_LABELS                             \
  LABELIZE(BINARY),                                   \
  LABELIZE(OCTAL),                                    \
  LABELIZE(HEXADECIMAL),                              \
  LABELIZE(DECIMAL),                                  \
  LABELIZE(GET_BASE),                                 \
  LABELIZE(SET_BASE),                                 \
  LABELIZE(DEPTH),                                    \
  LABELIZE(PICK),                                     \
  LABELIZE(ROLL),                                     \
  LABELIZE(NIP),                                      \
  LABELIZE(SWAP),                                     \
  LABELIZE(OVER),                                     \
  LABELIZE(QDUP),                                     \
  LABELIZE(DUP),                                      \
  LABELIZE(DROP),                                     \
  LABELIZE(2DROP),                                    \
  LABELIZE(AND),                                      \
  LABELIZE(OR),                                       \
  LABELIZE(XOR),                                      \
  LABELIZE(PLUS),                                     \
  LABELIZE(MINUS),                                    \
  LABELIZE(DIV),                                      \
  LABELIZE(TIMES),                                    \
  LABELIZE(RSHIFT),                                   \
  LABELIZE(LSHIFT),                                   \
  LABELIZE(ABS),                                      \
  LABELIZE(MIN),                                      \
  LABELIZE(MAX),                                      \
  LABELIZE(TRUE),                                     \
  LABELIZE(FALSE),                                    \
  LABELIZE(GREATER_EQUAL),                            \
  LABELIZE(GREATER),                                  \
  LABELIZE(LOWER_EQUAL),                              \
  LABELIZE(LOWER),                                    \
  LABELIZE(EQUAL),                                    \
  LABELIZE(ZEROEQUAL),                                \
  LABELIZE(NOT_EQUAL),                                \
  LABELIZE(NEGATE),                                   \
  LABELIZE(1MINUS),                                   \
  LABELIZE(1PLUS),                                    \
  LABELIZE(DISP),                                     \
  LABELIZE(UDISP),                                    \
  LABELIZE(DISP_STACK)

#define ALL_DSTACK_PRIMITIVES                                    \
  PRIMITIVE(BINARY,           "BINARY");                         \
  PRIMITIVE(OCTAL,            "OCTAL");                          \
  PRIMITIVE(HEXADECIMAL,      "HEXADECIMAL");                    \
  PRIMITIVE(DECIMAL,          "DECIMAL");                        \
  PRIMITIVE(GET_BASE,         "GET_BASE");                       \
  PRIMITIVE(SET_BASE,         "SET_BASE");                       \
  PRIMITIVE(DEPTH,            "DEPTH");                          \
  PRIMITIVE(PICK,             "PICK");                           \
  PRIMITIVE(ROLL,             "ROLL");                           \
  PRIMITIVE(NIP,              "NIP");                            \
  PRIMITIVE(SWAP,             "SWAP");                           \
  PRIMITIVE(OVER,             "OVER");                           \
  PRIMITIVE(QDUP,             "QDUP");                           \
  PRIMITIVE(DUP,              "DUP");                            \
  PRIMITIVE(DROP,             "DROP");                           \
  PRIMITIVE(2DROP,            "2DROP");                          \
  PRIMITIVE(AND,              "AND");                            \
  PRIMITIVE(OR,               "OR");                             \
  PRIMITIVE(XOR,              "XOR");                            \
  PRIMITIVE(PLUS,             "+");                              \
  PRIMITIVE(MINUS,            "-");                              \
  PRIMITIVE(DIV,              "/");                              \
  PRIMITIVE(TIMES,            "*");                              \
  PRIMITIVE(RSHIFT,           ">>");                             \
  PRIMITIVE(LSHIFT,           "<<");                             \
  PRIMITIVE(ABS,              "ABS");                            \
  PRIMITIVE(MIN,              "MIN");                            \
  PRIMITIVE(MAX,              "MAX");                            \
  PRIMITIVE(TRUE,             "TRUE");                           \
  PRIMITIVE(FALSE,            "FALSE");                          \
  PRIMITIVE(GREATER_EQUAL,    ">=");                             \
  PRIMITIVE(GREATER,          ">");                              \
  PRIMITIVE(LOWER_EQUAL,      "<=");                             \
  PRIMITIVE(LOWER,            "<");                              \
  PRIMITIVE(EQUAL,            "=");                              \
  PRIMITIVE(ZEROEQUAL,        "0=");                             \
  PRIMITIVE(NOT_EQUAL,        "<>");                             \
  PRIMITIVE(NEGATE,           "NEGATE");                         \
  PRIMITIVE(1MINUS,           "1-");                             \
  PRIMITIVE(1PLUS,            "1+");                             \
  PRIMITIVE(DISP,             ".");                              \
  PRIMITIVE(UDISP,            "U.");                             \
  PRIMITIVE(DISP_STACK,       ".S");

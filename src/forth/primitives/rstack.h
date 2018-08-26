#define ALL_RSTACK_TOKENS                       \
  TOK_TO_RSTACK,                                \
  TOK_FROM_RSTACK,                              \
  TOK_TO_2RSTACK,                               \
  TOK_FROM_2RSTACK

#define ALL_RSTACK_LABELS                       \
  LABELIZE(TO_RSTACK),                          \
  LABELIZE(FROM_RSTACK),                        \
  LABELIZE(TO_2RSTACK),                         \
  LABELIZE(FROM_2RSTACK)

#define ALL_RSTACK_PRIMITIVES                   \
  PRIMITIVE(TO_RSTACK,            ">R");        \
  PRIMITIVE(FROM_RSTACK,          "R>");        \
  PRIMITIVE(TO_2RSTACK,           "2>R");       \
  PRIMITIVE(FROM_2RSTACK,         "2R>");

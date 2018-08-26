// Move x to the return stack.
// ( x -- ) ( R: -- x )
CODE(TO_RSTACK)
RPUSH(DPOP());
NEXT;

CODE(FROM_RSTACK)
DPUSH(RPOP());
NEXT;

// Transfer cell pair x1 x2 to the return stack
// ( x1 x2 -- ) ( R: -- x1 x2 )
CODE(TO_2RSTACK)
m_tos = DPOP();
RPUSH(DPOP());
RPUSH(m_tos);
NEXT;

CODE(FROM_2RSTACK)
m_tos = RPOP();
DPUSH(RPOP());
DPUSH(m_tos);
NEXT;

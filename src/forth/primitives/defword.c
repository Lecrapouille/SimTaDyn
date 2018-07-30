//=====================================================================
// Set Forth in interpretation mode
CODE(LBRACKET)
{
  m_state = forth::state::interprete;
}
NEXT;

//=====================================================================
// Set Forth in compilation mode
CODE(RBRACKET)
{
  m_state = forth::state::compile;
}
NEXT;

//=====================================================================
// Interprete the token stored in the datastack
CODE(EXECUTE)
{
  forth::token xt = DPOP();
  if (xt <= maxPrimitives())
    execToken(xt);
  throw ForthException(MSG_EXCEPTION_UNKNOWN_FORTH_PRIMITIVE(xt, ""));
}
NEXT;

//=====================================================================
// Compile a token
CODE(COMPILE)
{
  m_ip += 2;
  m_dictionary.appendToken(m_dictionary.readToken(m_ip));
  NEXT;
}

//=====================================================================
// Append to the dictionary the next word
CODE(ICOMPILE)
{
  Cell16 token;
  bool immediate;
  std::string const& word = nextWord();

  //m_ip += 2;
  if (m_dictionary.find(word, token, immediate))
    {
      m_dictionary.appendToken(token);
    }
  else
    {
      UnknownForthWord e(word); throw e;
    }
}
NEXT;

//=====================================================================
// ANSI word to replace [COMPILE] and COMPILE
CODE(POSTPONE)
{
  Cell16 token;
  bool immediate;
  std::string const& word = nextWord();

  if (m_dictionary.find(word, token, immediate))
    {
      if (immediate)
        {
          m_dictionary.appendToken(token);
        }
      else
        {
          m_ip += 2;
          m_dictionary.appendToken(m_dictionary.readToken(m_ip));
        }
    }
  else
    {
      UnknownForthWord e(word); throw e;
    }
}
NEXT;

//=====================================================================
// Start the definition of a new Forth word
CODE(COLON)
{
  m_state = forth::state::compile;
  startCompilingWord(nextWord());
}
NEXT;

//=====================================================================
// First codeworde in
CODE(DOCOL)
{
  RPUSH(m_ip);
  m_ip = readToken();
}
NEXT;

//=====================================================================
// Terminate the definition of the courrently compiling Forth word
CODE(SEMICOLON)
{
  if (m_state == forth::state::interprete)
    abort("Tried to interpret a compile-only word");

  m_dictionary.saveContext(0,0);
  m_dictionary.appendToken(TOK_EXIT);
  m_state = forth::state::interprete;
  if (m_depth_at_colon != stackDepth(forth::DataStack))
    {
      m_dictionary.restoreContext();
      throw ForthException(MSG_EXCEPTION_FORTH_STACK_CHANGED_DURING_DEF(m_creating_word));
    }
}
NEXT;

//=====================================================================
// Restore the IP when interpreting the definition of a non primitive word
CODE(DOSEMI)
{
  m_ip = RPOP();
}
NEXT;

//=====================================================================
// Interprete a int16 inside a Forth word definition.
CODE(LITERAL16)
{
  DPUSH(m_tos);
  m_ip += forth::token_size; // Skip primitive LITERAL
  m_tos = m_dictionary.read16at(m_ip);
}
NEXT;

//=====================================================================
// Interprete a int32 inside a Forth word definition.
CODE(LITERAL32)
{
  DPUSH(m_tos);
  m_ip += forth::token_size; // Skip primitive LITERAL
  m_tos = m_dictionary.read32at(m_ip);
  m_ip += forth::token_size; // Skip the number - 2 because of next ip
  NEXT;
}

//=====================================================================
// Set immediate the last compiled word
CODE(IMMEDIATE)
{
  m_dictionary.immediate();
}
NEXT;

//=====================================================================
// Fiand and toggle the smudge bit of a word
CODE(SMUDGE)
{
  m_dictionary.smudge(nextWord());
}
NEXT;

//=====================================================================
// Store the CFA of the latest compiled word in the data stack
CODE(LAST)
{
  DPUSH(m_dictionary.last());
}
NEXT;

//=====================================================================
// Give a name to the next free slot in the dictionary.
// (note: HERE is not moved)
// https://fr.wikiversity.org/wiki/Forth/Conserver_des_donn%C3%A9es
CODE(CREATE)
{
  startCompilingWord(nextWord());
  m_dictionary.appendToken(TOK_PCREATE);
  m_dictionary.appendToken(TOK_EXIT);
}
NEXT;

//=====================================================================
//
CODE(BUILDS)
{
  startCompilingWord(nextWord());
}
NEXT;

//=====================================================================
//
CODE(DOES)
{
  std::cerr << "NOT IMPLEMENTED" << std::endl;
}
NEXT;

//=====================================================================
//
CODE(TICK)
{
  std::string const& word = nextWord();
  if (false == m_dictionary.exist(word))
    throw ForthException("Tried to CFA of an unknown word '"
                         + word + "'");
  DPUSH(token);
}
NEXT;

      // Restore the IP when interpreting the definition
      // of a non primitive word
    case FORTH_PRIMITIVE_EXIT:
      RPOP(m_ip);
      if (m_trace) {
        std::cout << "POPed: " << std::hex << (int) m_ip << std::endl;
      }
      break;
 
     // Set immediate the last word
    case FORTH_PRIMITIVE_IMMEDIATE:
      m_dictionary.m_dictionary[m_dictionary.m_last] |= FLAG_IMMEDIATE;
      break;

      // Begin the definition of a new word
    case FORTH_PRIMITIVE_COLON:
      create(nextWord());
      m_state = forth::Compile;
      break;

      // End the definition of a new word
    case FORTH_PRIMITIVE_SEMICOLON:
      if (m_state == forth::Interprete) abort("Tried to interpret a compile-only word");
      m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
      m_state = forth::Interprete;
      if (m_depth_at_colon != stackDepth(forth::DataStack))
        {
          m_dictionary.m_last = m_last_at_colon;
          m_dictionary.m_here = m_here_at_colon;
          ModifiedStackDepth e(m_creating_word);
          throw e;
        }
      break;

    case FORTH_PRIMITIVE_LAST:
      DPUSH(m_tos);
      m_tos = m_dictionary.last();
      break;

      // Give a name to the next free slot in the dictionary.
      // (note: HERE is not moved)
      // https://fr.wikiversity.org/wiki/Forth/Conserver_des_donn%C3%A9es
    case FORTH_PRIMITIVE_CREATE:
      create(nextWord());
      m_dictionary.appendCell16(FORTH_PRIMITIVE_PCREATE);
      m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
      break;

    case FORTH_PRIMITIVE_BUILDS:
      create(nextWord());
      break;

    case FORTH_PRIMITIVE_DOES:
      break;

     case FORTH_PRIMITIVE_EXECUTE:
       execToken(m_tos);
       break;

     case FORTH_PRIMITIVE_LBRACKET:
       m_state = forth::Interprete;
       break;

    case FORTH_PRIMITIVE_RBRACKET:
       m_state = forth::Compile;
       break;

    case FORTH_PRIMITIVE_SMUDGE:
      {
        std::string const& word = nextWord();
        if (false == m_dictionary.smudge(word))
          {
            m_color.yellow();
            std::cout << m_color << "[WARNING] Unknown word '"
                      << word << "'. Word SMUDGE Ignored !";
            m_color.normal();
            std::cout << m_color << std::endl;
          }
      }
      break;

    case FORTH_PRIMITIVE_TICK:
      {
        std::string const& word = nextWord();
        Cell16 token;
        bool immediate;
        if (false == m_dictionary.find(word, token, immediate))
          {
            token = 0;
            m_color.yellow();
            std::cout << m_color << "[WARNING] Unknown word '"
                      << word << "'. Word TICK Ignored !";
            m_color.normal();
            std::cout << m_color << std::endl;
          }
        DPUSH(m_tos);
        m_tos = token;
      }
      break;

     case FORTH_PRIMITIVE_COMPILE:
       m_ip += 2;
       m_dictionary.appendCell16(m_dictionary.read16at(m_ip));
       break;

       // Append to the dictionary the next word
     case FORTH_PRIMITIVE_ICOMPILE:
       {
         Cell16 token;
         bool immediate;
         std::string const& word = nextWord();

         //m_ip += 2;
         if (m_dictionary.find(word, token, immediate))
           {
             m_dictionary.appendCell16(token);
           }
         else
           {
             UnknownForthWord e(word); throw e;
           }
       }
       break;

       // ANSI word to replace [COMPILE] and COMPILE
    case FORTH_PRIMITIVE_POSTPONE:
      {
        Cell16 token;
        bool immediate;
        std::string const& word = nextWord();

        if (m_dictionary.find(word, token, immediate))
          {
            if (immediate)
              {
                m_dictionary.appendCell16(token);
              }
            else
              {
                m_ip += 2;
                m_dictionary.appendCell16(m_dictionary.read16at(m_ip));
              }
          }
        else
          {
            UnknownForthWord e(word); throw e;
          }
      }
      break;

    case FORTH_PRIMITIVE_LITERAL_16:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITERAL
      m_tos = m_dictionary.read16at(m_ip);
      break;

    case FORTH_PRIMITIVE_LITERAL_32:
      DPUSH(m_tos);
      m_ip += 2U; // Skip primitive LITERAL
      m_tos = m_dictionary.read32at(m_ip);
      m_ip += 2U; // Skip the number - 2 because of next ip
      break;

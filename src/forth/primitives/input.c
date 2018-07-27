      // Begin of commentary
    case FORTH_PRIMITIVE_LPARENTHESIS:
      //++m_parenthesis;
      m_saved_state = m_state;
      m_state = forth::Comment;
      break;

      // End of commentary
    case FORTH_PRIMITIVE_RPARENTHESIS:
std::cout << "R]" << std::endl;
      if ((forth::Comment != m_state) /*|| (0 == m_parenthesis)*/)
        {
           abort("Unbalanced comment");
        }
     // --m_parenthesis;
      m_state = m_saved_state;
      break;

      // Line of commentary
    case FORTH_PRIMITIVE_COMMENTARY:
      STREAM.skipLine();
      break;

    case FORTH_PRIMITIVE_CARRIAGE_RETURN:
      std::cout << std::endl;
      break;

    case FORTH_PRIMITIVE_INCLUDE:
      // Restore TOS register (because execToken() poped TOS)
      DPUSH(m_tos);

      includeFile(nextWord());

      DPOP(m_tos);
      break;

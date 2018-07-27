      //
    case FORTH_PRIMITIVE_BINARY:
      changeDisplayBase(2U);
      break;
    case FORTH_PRIMITIVE_OCTAL:
      changeDisplayBase(8U);
      break;
    case FORTH_PRIMITIVE_HEXADECIMAL:
      changeDisplayBase(16U);
      break;
    case FORTH_PRIMITIVE_DECIMAL:
      changeDisplayBase(10U);
      break;
    case FORTH_PRIMITIVE_GET_BASE:
      DPUSH(m_tos);
      m_tos = m_base;
      break;
    case FORTH_PRIMITIVE_SET_BASE:
      if (false == changeDisplayBase(m_tos))
        {
          m_color.yellow();
          std::cerr << m_color << "[WARNING] '"
                    << m_tos << "' is an invalid base and shall be [2..36]. Ignored !";
          m_color.normal();
          std::cerr << m_color << std::endl;
        }
      break;

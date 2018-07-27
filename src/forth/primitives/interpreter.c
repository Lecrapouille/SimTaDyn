    case FORTH_PRIMITIVE_TRACE_ON:
      m_trace = true;
      break;

    case FORTH_PRIMITIVE_TRACE_OFF:
      m_trace = false;
      break;

     case FORTH_PRIMITIVE_ABORT:
       abort("COUCOU");
       break;

      /*  case FORTH_PRIMITIVE_THROW:
      if (m_tos)
        {
          M_THROW(m_tos);
        }
      else DPOP(m_tos);
      break;*/

      // Change IP
    case FORTH_PRIMITIVE_BRANCH:
      m_ip += m_dictionary.read16at(m_ip + 2U);
      // Do not forget that m_ip will be += 2 next iteration
      break;

      // Change IP if top of stack is 0
    case FORTH_PRIMITIVE_0BRANCH:
      if (0 == m_tos)
        {
          m_ip += m_dictionary.read16at(m_ip + 2U);
        }
      else
        {
          m_ip += 2U;
        }
      // Do not forget that m_ip will be += 2 next iteration
      DPOP(m_tos);
      break;

#if 0
    case FORTH_PRIMITIVE_DO:
      std::cout << "(DO)" << std::endl;
      RPUSH(m_tos);
      DPOP(m_tos1); // FIXME: optim: RPUSH(DPOP);
      RPUSH(m_tos1);
      m_tos = DDROP();
      break;

    case FORTH_PRIMITIVE_LOOP:
      std::cout << "(LOOP)" << std::endl;
      RPOP(m_tos1); /* limit */
      RPOP(m_tos2); /* index */
      ++m_tos2;
      if (m_tos2 == m_tos1)
        {
          ++m_ip;   /* skip branch offset, exit loop */
        }
      else
        {
/* Push index and limit back to R */
          RPUSH(m_tos2);
          RPUSH(m_tos1);
/* Branch back to just after (DO) */
          m_ip = m_dictionary.read16at(m_ip);
        }
      break;
#endif

    case FORTH_PRIMITIVE_I:
      DPUSH(m_tos);
      m_tos = RPICK(0);
      break;

    case FORTH_PRIMITIVE_J:
      DPUSH(m_tos);
      m_tos = RPICK(2);
      break;

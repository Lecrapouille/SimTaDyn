    case FORTH_PRIMITIVE_HERE:
      DPUSH(m_tos);
      m_tos = m_dictionary.here();
      break;

      // ( a-addr -- x )
      // x is the value stored at a-addr.
    case FORTH_PRIMITIVE_FETCH:
      m_tos = m_dictionary.read32at(m_tos);
      break;

    case FORTH_PRIMITIVE_CELL:
      DPUSH(m_tos);
      m_tos = sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_CELLS:
      m_tos = m_tos * sizeof (Cell32);
      break;

    case FORTH_PRIMITIVE_ALLOT:
      m_dictionary.allot((int32_t) m_tos);
      DPOP(m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA32:
      m_dictionary.appendCell32(m_tos);
      DPOP(m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA16:
      m_dictionary.appendCell16(m_tos);
      DPOP(m_tos);
      break;

      // Reserve one cell of data space and store x in the cell.
    case FORTH_PRIMITIVE_COMMA8:
      m_dictionary.appendCell8(m_tos);
      DPOP(m_tos);
      break;

      // Store x at a-addr.
      // ( x a-addr -- )
    case FORTH_PRIMITIVE_STORE32:
      DPOP(m_tos1);
      m_dictionary.write32at(m_tos, m_tos1);
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_STORE16:
      DPOP(m_tos1);
      m_dictionary.write16at(m_tos, m_tos1);
      DPOP(m_tos);
      break;

    case FORTH_PRIMITIVE_STORE8:
      DPOP(m_tos1);
      m_dictionary.write8at(m_tos, m_tos1);
      DPOP(m_tos);
      break;

      /* ( src dst n -- ) */
    case FORTH_PRIMITIVE_CMOVE:
      DPOP(m_tos2); // dst
      DPOP(m_tos1); // src
      m_dictionary.move(m_tos2, m_tos1, m_tos);
      DPOP(m_tos);
      break;

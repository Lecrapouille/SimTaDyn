    case FORTH_PRIMITIVE_BEGIN_C_LIB:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.begin(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_END_C_LIB:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.end(/*nth*/); // FIXME
        if (res.first)
          {
            uint16_t i = 0;
            for (auto it: m_dynamic_libs/*[nth]*/.m_functions)
              {
                create(it.func_forth_name);
                m_dictionary.appendCell16(FORTH_PRIMITIVE_EXEC_C_FUNC);
                m_dictionary.appendCell16(i);
                m_dictionary.appendCell16(FORTH_PRIMITIVE_EXIT);
                ++i;
              }
          }
        else
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_EXEC_C_FUNC:
      DPUSH(m_tos);
      m_ip += 2U; // Skip the index of pointer function
      m_tos = m_dictionary.read16at(m_ip);
      //std::cout << "Pile avant: ";
      //displayStack(std::cout, forth::DataStack);

      m_dynamic_libs/*[nth]*/.m_functions[m_tos].fun_ptr(&m_dsp);
      //std::cout << "Pile apres: ";
      //displayStack(std::cout, forth::DataStack);
      DPOP(m_tos);
      break;
    case FORTH_PRIMITIVE_C_FUNCTION:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.function(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;
    case FORTH_PRIMITIVE_C_CODE:
      {
        std::pair<bool, std::string> res = m_dynamic_libs.code(STREAM);
        if (!res.first)
          {
            abort(res.second);
          }
      }
      break;

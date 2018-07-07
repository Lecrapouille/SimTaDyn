#include "ForthClibrary.hpp"
#include "PathManager.hpp"

#define ERR_UNBALANCED std::make_pair(false, "Unbalanced C-LIB and END-C-LIB words")

ForthCLib::ForthCLib()
{
}

ForthCLib::~ForthCLib()
{
  if (m_file)
    m_file.close();
  if (nullptr != m_module)
    delete m_module;
}

// **************************************************************
//! \param
// **************************************************************
std::pair<bool, std::string>
ForthCLib::code(ForthStream &stream)
{
   if (!m_file)
    return ERR_UNBALANCED;

   m_file << stream.getLine();
   m_file << "\n";
   return std::make_pair(true, "");
}

// **************************************************************
//! \param
// **************************************************************
std::pair<bool, std::string>
ForthCLib::function(ForthStream &stream)
{
#warning "FIXME: si on passe aucun param et aucun code de retour alors ca bug"
  if (!m_file)
    return ERR_UNBALANCED;

  CFuncHolder holder;

  // Extract Forth word
  if (!stream.hasMoreWords())
    return std::make_pair(false, "Missing Forth definition");
  holder.func_forth_name = stream.nextWord();

  // Extract C function name
  if (!stream.hasMoreWords())
    return std::make_pair(false, "Missing C function name");
  std::string func_c_shortname(stream.nextWord());

  // Extract C function params
  int32_t nb_params = 0;
  bool has_return_code = false;
  std::string param;
  std::string stack;

  while (stream.hasMoreWords())
    {
      std::string token(stream.nextWord());

      // Parse read tokens
      if (token == "n")
        {
          param += 'n';
          if (!has_return_code)
            {
              if (nb_params)
                stack += ", ";
              ++nb_params;
              stack += "dsp[" + std::to_string(-nb_params) + "]";
            }
        }
      else if (token == "--")
        {
          param += '_';
          if (has_return_code)
            {
              return std::make_pair(false, "C function can only return a single error value");
            }
          has_return_code = true;
        }
      else
        {
          return std::make_pair(false, "'" + token + "' is not a valid parameter type");
        }
      if (stream.eol())
        break;
    }

  // Generate the C function with its code
  std::string code(func_c_shortname);
  code += '('; code += stack; code += ')';
  if (has_return_code)
    {
      code = "dsp[" + std::to_string(-nb_params) + "] = " + code;
      code += ";\n  *pdsp = dsp - " + std::to_string(nb_params-1);
    }
  else
    {
      code += ";\n  *pdsp = dsp - " + std::to_string(nb_params);
    }
  holder.func_c_name = "simforth_c_" + func_c_shortname + '_' + param;
  holder.code = "void " + holder.func_c_name + "(Cell32** pdsp)\n{\n  Cell32* dsp = *pdsp;\n  " + code + ";\n}\n";

  // Save it in the file
  m_file << holder.code;

  // Save it for later
  m_functions.push_back(holder);
  return std::make_pair(true, "");
}

// **************************************************************
//! \param
// **************************************************************
std::pair<bool, std::string>
ForthCLib::extlib(ForthStream &stream)
{
  if (!m_file)
    return ERR_UNBALANCED;

  if (!stream.hasMoreWords())
    return std::make_pair(false, "Missing library name");
  m_extlibs += " -l";
  m_extlibs += stream.nextWord();
  return std::make_pair(true, "");
}

// **************************************************************
//! \param
// **************************************************************
std::pair<bool, std::string>
ForthCLib::begin(ForthStream &stream)
{
  if (!m_file)
    return ERR_UNBALANCED;

  if (!stream.hasMoreWords())
    return std::make_pair(false, "Missing library name");
  m_libname = stream.nextWord();

  // Create a temporary C file which will contain all generated C code
  // wrapping the function parameters.
  m_sourcepath = config::tmp_path + m_libname + ".c";
  m_file.open(m_sourcepath);
  if (!m_file)
    {
      return std::make_pair(false, "Failed creating '" + m_sourcepath + "'");
    }
  m_file << "#include <stdint.h>\n";
  m_file << "typedef uint32_t Cell32;\n"; // FIXME: Adapt automaticaly the size Cell64 ..
  return std::make_pair(true, "");
}

// **************************************************************
//! \param
// **************************************************************
std::pair<bool, std::string>
ForthCLib::end()
{
  if (!m_file)
    return ERR_UNBALANCED;
  m_file.close();

  // Compile generated code into a dynamic library.
  std::string makefile = PathManager::instance().expand("forth/LibC/Makefile");
  std::string command = "make -f " + makefile
    + " BUILD=" + config::tmp_path
    + " SRCS=" + m_libname + ".c"
    + " EXTLIBS=\"" + m_extlibs + "\"";
  system(command.c_str()); // FIXME: tester code de retour

  // Find symbols in the dynamic lib and create Forth entries in the dictionary.
  // FIXME: ok extension .so/.dylib pas necessaire avec gtkmm::Module mais utile pour les log
  std::string libpath(config::tmp_path + m_libname);
  m_module = new Glib::Module(libpath);
  if (!*m_module)
    return std::make_pair(false, "Failed loading shared libray '" + libpath + "'");

  bool res = true;
  std::string msg;
  for (auto &it: m_functions)
    {
      void* func = nullptr;
      if (m_module->get_symbol(it.func_c_name, func))
        {
          LOGD("Found symbol '%s' in '%s'", it.func_c_name.c_str(), libpath.c_str());
          it.fun_ptr = reinterpret_cast<forth_c_function>(reinterpret_cast<long>(func)) ;
        }
      else
        {
          msg += ("Failed finding symbol '" + it.func_c_name + "' in '" + libpath + ".so\n"); //FIXME .so
          res = false;
        }
    }

  return std::make_pair(res, msg);
}

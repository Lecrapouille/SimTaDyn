#ifndef FORTHCLIBRARY_HPP
#  define FORTHCLIBRARY_HPP

#  include "ForthStream.hpp"
#  include <glibmm/module.h>
#  include <vector>
#  include <utility>

typedef void (*forth_c_function)(Cell32**);

// A siplifier std::string
struct CFuncHolder
{
  ~CFuncHolder() {}
  std::string func_forth_name; // inutile
  std::string func_c_name;
  std::string code;// inutile
  forth_c_function fun_ptr;
};

// **************************************************************
//! \brief
// **************************************************************
class ForthCLib
{
public:

  ForthCLib();
  ~ForthCLib();

  //------------------------------------------------------------------
  //! \brief Start a C library interface with name.
  //! Create a temporary C file which will contain all generated wrapper
  //! functions. This file will be compiled as dynamic library and its
  //! functions loaded as Forth words.
  //! Forth word is C-LIB and here is an example:
  //! C-LIB library-name
  //! \return true if the temporary file has been created, else false.
  //------------------------------------------------------------------
  std::pair<bool, std::string> begin(ForthStream &stream);
  std::pair<bool, std::string> extlib(ForthStream &stream);

  //------------------------------------------------------------------
  //! \brief Append a line of C code in the temporary c file created by
  //! the Forth word C-LIB.
  //! \return true if the temporary file exists, else retunr false.
  //! Forth code example:
  //! \c #include <stdio.h>
  //------------------------------------------------------------------
  std::pair<bool, std::string> code(ForthStream &stream);

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  std::pair<bool, std::string> function(ForthStream &stream);

  //------------------------------------------------------------------
  //! \brief Finish and (if necessary) build the latest C library interface.
  //! Forth code example:
  //! END-C-LIB
  //------------------------------------------------------------------
  std::pair<bool, std::string> end();

  //std::map<std::string, CFuncHolder> m_functions;
  std::vector<CFuncHolder> m_functions;

private:

  std::ofstream m_file;
  std::string m_libname;
  std::string m_sourcepath;
  std::string m_extlibs;
  //bool m_closed;
  Glib::Module *m_module = nullptr; // TODO: use unique_ptr Yeah pointer else local will close file and lost func pointers
};

#endif

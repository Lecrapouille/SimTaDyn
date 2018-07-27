//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTHCLIBRARY_HPP
#  define FORTHCLIBRARY_HPP

#  include "ForthTypes.hpp"
#  include "ForthStream.hpp"
#  include <glibmm/module.h>
#  include <vector>
#  include <utility>

namespace forth
{

  typedef void (*forth_c_function)(forth::cell**);

// A siplifier std::string
struct CFuncHolder
{
  std::string func_forth_name; // inutile
  std::string func_c_name;
  std::string code;// inutile
  forth_c_function fun_ptr;
};

// **************************************************************
//! \brief
// **************************************************************
class CLib
{
public:

  CLib();
  ~CLib();

  //------------------------------------------------------------------
  //! \brief Start a C library interface with name.
  //! Create a temporary C file which will contain all generated wrapper
  //! functions. This file will be compiled as dynamic library and its
  //! functions loaded as Forth words.
  //! Forth word is C-LIB and here is an example:
  //! C-LIB library-name
  //! \return true if the temporary file has been created, else false.
  //------------------------------------------------------------------
  std::pair<bool, std::string> begin(forth::Stream &stream);
  std::pair<bool, std::string> extlib(forth::Stream &stream);

  //------------------------------------------------------------------
  //! \brief Append a line of C code in the temporary c file created by
  //! the Forth word C-LIB.
  //! \return true if the temporary file exists, else retunr false.
  //! Forth code example:
  //! \c #include <stdio.h>
  //------------------------------------------------------------------
  std::pair<bool, std::string> code(forth::Stream &stream);

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  std::pair<bool, std::string> function(forth::Stream &stream);

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
  bool m_closed;
  Glib::Module *m_module; // Yeah pointer else local will close file and lost func pointers
};

} // namespace

#endif

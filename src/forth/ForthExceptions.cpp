//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "ForthExceptions.hpp"

const char *c_stack_names[forth::MaxStackID + 1] =
  {
    [forth::DataStack] = "Data Stack",
    [forth::ReturnStack] = "Return Stack",
    [forth::AuxStack] = "Auxilary Stack",
    [forth::FloatStack] = "Floating point Stack",
  };

//! This macro (from the library POCO) will generate code for members.
IMPLEMENT_EXCEPTION(ForthException, Exception, "Forth Exception")

OutOfBoundStack::OutOfBoundStack(const forth::StackID stack_id, const int32_t depth)
  : ForthException(37)
{
  m_stack_id = stack_id;
  m_msg = c_stack_names[stack_id];

  if (depth < 0)
    {
      m_msg += " underflow";
    }
  else
    {
      m_msg += " overflow";
    }
}

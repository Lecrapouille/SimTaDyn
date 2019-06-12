//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef OPENGL_HPP
#  define OPENGL_HPP

// *****************************************************************************
//! \file OpenGL.hpp OpenGL routines.
// *****************************************************************************

#  include "Logger.hpp"
#  include "GLEnum.hpp"
#  include "NonCopyable.hpp"
#  include "NonCppStd.hpp"
#  include "Exception.hpp"

// ***********************************************************************************************
//! \brief This macro will declare a class OpenGLException derived from Exception.
// ***********************************************************************************************
DECLARE_EXCEPTION(OpenGLException, Exception)

namespace opengl
{
//----------------------------------------------------------------------------
//! \brief Return if the OpenGL has been created or has not been
//! created or has failed creating.
//!
//! \return true if the OpenGL context has been created
//! else return false (not yet created or failed during
//! its creation).
//----------------------------------------------------------------------------
inline static bool& hasCreatedContext()
{
  static bool s_context_started = false;
  return s_context_started;
}
} // namespace opengl

//----------------------------------------------------------------------------
//! \brief Allow to detect if the last OpenGL command succeeded or failed.
//! In the case of failure an error is displayed on console and/or logged.
//!
//! \note do not use this function directly but use the macro glCheck.
//!
//! \param filename the file path where the OpenGL routine was called.
//! \param line the line where the OpenGL routine was called.
//! \param expression the line content where the OpenGL routine was called.
//----------------------------------------------------------------------------
void checkGLError(const char* filename, const uint32_t line, const char* expression);

//----------------------------------------------------------------------------
//! Macro encapsuling the OpenGL command and the fault checker.
//----------------------------------------------------------------------------
#  ifdef CHECK_OPENGL
#    define glCheck(expr) expr; checkGLError(__FILE__, __LINE__, #expr);
#  else
#    define glCheck(expr) expr;
#  endif

#endif // OPENGL_HPP

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

#include "OpenGL.hpp"

//! This macro will generate code for members.
IMPLEMENT_EXCEPTION(OpenGLException, Exception, "OpenGL Exception")

void checkError(const char* filename, const uint32_t line, const char* expression)
{
  GLenum id;
  const char* error;

  while ((id = glGetError()) != GL_NO_ERROR)
    {
      switch (id)
        {
        case GL_INVALID_OPERATION:
          error = "GL_INVALID_OPERATION";
          break;
        case GL_INVALID_ENUM:
          error = "GL_INVALID_ENUM";
          break;
        case GL_INVALID_VALUE:
          error = "GL_INVALID_VALUE";
          break;
        case GL_OUT_OF_MEMORY:
          error = "GL_OUT_OF_MEMORY";
          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
          error = "GL_INVALID_FRAMEBUFFER_OPERATION";
          break;
        default:
          error = "UNKNOWN";
          break;
        }

      // Do not use directly LOG macros because it will catch this
      // filename and its line instead of the faulty file/line which
      // produced the OpenGL error.
      Logger::instance().log(&std::cerr, logger::Error,
                             "[%s::%d] Failed executing '%s'. Reason is '%s'\n",
                             filename, line, expression, error);
    }
}

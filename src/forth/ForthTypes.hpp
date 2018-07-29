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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTH_TYPES_HPP_
#  define FORTH_TYPES_HPP_

#  include <cstdint>

// FIXME temporaire
#  define FORTH_PRIMITIVE_LITERAL_16 0
#  define FORTH_PRIMITIVE_LITERAL_32 1

#  ifndef likely
#    define likely(x)       __builtin_expect(!!(x),1)
#  endif
#  ifndef unlikely
#    define unlikely(x)     __builtin_expect(!!(x),0)
# endif

namespace forth
{
  typedef int32_t        cell;
  typedef uint32_t       ucell;
  typedef uint16_t       token;

#  define appendToken appendCell16
#  define readToken readCell16
#  define appendCell appendCell32
#  define readCell readCell32

  static constexpr int cell_size = sizeof (forth::cell);
  static constexpr int token_size = sizeof (forth::token);
}

#endif // FORTH_TYPES_HPP_

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

#include "Forth.hpp"
#include "ForthPrimitives.hpp"
#include "primitives/defword.h"
#include "primitives/rstack.h"
#include "primitives/fstack.h"
#include "primitives/dstack.h"
#include "Logger.hpp"

enum ForthPrimitives
  {
    // Dummy word and comments
    TOK_NOOP = 0,
    ALL_BRANCHING_TOKENS,
    ALL_IO_TOKENS,
    ALL_CLIB_TOKENS,
    ALL_DICO_TOKENS,
    ALL_DEFWORD_TOKENS,
    ALL_RSTACK_TOKENS,
    ALL_FSTACK_TOKENS,
    ALL_DSTACK_TOKENS,
    FORTH_MAX_TOKENS
  };

uint32_t Forth::maxPrimitives() const
{
  return FORTH_MAX_TOKENS;
}

// **************************************************************
//! This method should be called after the contructor Forth()
//! and has been separated from the constructor to load the
//! dictionary at the desired moment.
// **************************************************************
void Forth::boot()
{
  LOGI("Forth booting ...");
  PRIMITIVE(NOOP, "NOOP");
  ALL_DEFWORD_PRIMITIVES;
  ALL_RSTACK_PRIMITIVES;
  ALL_FSTACK_PRIMITIVES;
  ALL_DSTACK_PRIMITIVES;
}

//------------------------------------------------------------------
//! \param token.
//------------------------------------------------------------------
void Forth::executeToken(forth::token const xt)
{
  // Only if macro USE_COMPUTED_GOTO is defines
#ifdef USE_COMPUTED_GOTO
  static void *c_primitives[FORTH_MAX_TOKENS] =
    {
      LABELIZE(NOOP),
      ALL_DEFWORD_LABELS,
      ALL_RSTACK_LABELS,
      ALL_FSTACK_LABELS,
      ALL_DSTACK_LABELS,
    };
#endif

  DISPATCH(xt)
  {
    CODE(NOOP) NEXT;
#include "primitives/defword.c"
#include "primitives/rstack.c"
#include "primitives/fstack.c"
#include "primitives/dstack.c"
    UNKNOWN: NEXT;
  }
}

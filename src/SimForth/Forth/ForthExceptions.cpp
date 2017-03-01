#include "ForthExceptions.hpp"

//! This macro (from the library POCO) will generate code for members.
POCO_IMPLEMENT_EXCEPTION(ForthException, simtadyn::Exception, "Forth Exception");

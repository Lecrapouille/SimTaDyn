#include "ForthExceptions.hpp"

const char *c_stack_names[forth::MaxStackID + 1] =
  {
    [forth::DataStack] = "Data Stack",
    [forth::ReturnStack] = "Return Stack",
    [forth::AuxStack] = "Auxilary Stack",
    [forth::FloatStack] = "Floating point Stack",
  };

//! This macro (from the library POCO) will generate code for members.
POCO_IMPLEMENT_EXCEPTION(ForthException, simtadyn::Exception, "Forth Exception")

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

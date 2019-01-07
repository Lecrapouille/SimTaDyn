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

#include "Forth.hpp"
#include <limits.h>

// **************************************************************
//! Initialize the Forth interpretor context. The
//! dictionary is given as reference to allow the inheritance.
//! \param dictionary the reference of (an empty) Forth dictionary.
//! \param color is a class for rendering words with ansi color.
// **************************************************************
Forth::Forth(ForthDictionary& dictionary)
  : m_state(forth::Interprete),
    m_dictionary(dictionary)
{
  LOGI("Creating Forth");
  m_err_stream = 0;
  m_base = 10U;
  m_last_completion = m_dictionary.last();

  // Not necessary but used for hiding coverity scan errors
  m_tos = 0;
  m_tos1 = 0;
  m_tos2 = 0;
  m_tos3 = 0;
  m_tos4 = 0;
  m_depth_at_colon = 0;
  m_last_at_colon = 0;
  m_here_at_colon = 0;

  //
  abort();
}

// **************************************************************
//! Reset the context like the Forth word ABORT does.
// **************************************************************
void Forth::abort()
{
  if ((forth::Compile == m_state) ||
      ((forth::Comment == m_state) &&
       (forth::Compile == m_saved_state)))
    {
      // Drop the Forth word not totaly compiled
      m_dictionary.m_last = m_last_at_colon;
      m_dictionary.m_here = m_here_at_colon;
    }

  m_ip = 0;
  m_state = forth::Interprete;
  m_saved_state = m_state;
  m_data_stack = m_data_stack_ + STACK_UNDERFLOW_MARGIN;
  m_alternative_stack = m_alternative_stack_ + STACK_UNDERFLOW_MARGIN;
  m_return_stack = m_return_stack_ + STACK_UNDERFLOW_MARGIN;
  m_dsp = m_data_stack;
  m_asp = m_alternative_stack;
  m_rsp = m_return_stack;
  m_opened_streams = 0;
  m_trace = false;
}

// **************************************************************
//! \throw AbortForth with a message inside.
//! \param msg the message to pass in the exception.
// **************************************************************
void Forth::abort(std::string const& msg)
{
  throw AbortForth(msg);
}

// **************************************************************
//! \throw OutOfBoundStack is data stack underflows or overflows
// **************************************************************
int32_t Forth::isStackUnderOverFlow(const forth::StackID id) const
{
  int32_t depth = stackDepth(id);
  if (depth < -1)
    {
      // Underflow
      throw OutOfBoundStack(id, depth);
    }
  else if (depth >= (int32_t) (STACK_SIZE - STACK_UNDERFLOW_MARGIN))
    {
      // Overflow
      throw OutOfBoundStack(id, depth);
    }
  return depth;
}

// **************************************************************
//! When displaying numbers on screen, display them on this
//! selected base. For example 10 in base 10 => A in base 16.
//! \param base the new displaying base.
//! \return if the new base can be used. If this is not the case
//! the older base is not changed.
// **************************************************************
bool Forth::changeDisplayBase(const uint8_t base)
{
  if ((base >= 2) && (base <= 36))
    {
      m_base = base;
      return true;
    }
  return false;
}

// **************************************************************
//! Used for trace and debug.
// **************************************************************
void Forth::displayStack(std::ostream& os, const forth::StackID id) const
{
  int32_t depth;
  Cell32 *ptr;

  switch (id)
    {
    case forth::DataStack:
      depth = m_dsp - m_data_stack;
      ptr = m_data_stack;
      break;
    case forth::ReturnStack:
      depth = m_rsp - m_return_stack;
      ptr = m_return_stack;
      break;
    case forth::AuxStack:
      depth = m_asp - m_alternative_stack;
      ptr = m_alternative_stack;
      break;
    default:
      LOGES("Pretty print this stack %u is not yet implemented", id);
      return ;
    }

  // Temporary change formatting
  std::ostream stream(os.rdbuf());

  // Display the stack
  stream << std::setbase(m_base);
  for (int32_t s = 0; s < depth; ++s)
    {
      stream << ptr[s] << ' ';
    }
  stream << std::endl;
}

// **************************************************************
//! \param word the Forth name to store in the dictionary.
// **************************************************************
void Forth::create(std::string const& word)
{
  if (m_dictionary.exists(word))
    {
      std::cout << FORTH_WARNING_COLOR << "[WARNING] Redefining '" << word << "'"
                << FORTH_NORMAL_COLOR << std::endl;
    }
  else
    {
      if (m_trace) {
        LOGD("Append new word definition '%s' in dictionary", word.c_str());
      }
    }

  // Save informations which will be checked
  // when executing the SEMI_COLON primitive.
  m_creating_word = word;
  m_last_at_colon = m_dictionary.m_last;
  m_here_at_colon = m_dictionary.m_here;
  m_depth_at_colon = stackDepth(forth::DataStack);

  // Add it in the dictionary
  Cell16 token = m_dictionary.here() + word.size() + 3U; // 3U: flags + NFA
  m_dictionary.add(token, word, 0);
}

// **************************************************************
//! \param tx token to execute (either a Forth primitive or a user
//! word definition).
//! \throw OutOfBoundStack if the data or the return stack overflowed.
// **************************************************************
void Forth::execToken(const Cell16 tx)
{
  int32_t depth;
  Cell16 token = tx;
  m_ip = 65535U;

  if (m_trace) {
    LOGI("Execute Forth token %u", tx);
    CPP_LOG(logger::Debug)
      << "================ Initial Stack ==================\n"
      << "DStack: " << ForthStackDiplayer(this, forth::DataStack)
      << "RStack: " << ForthStackDiplayer(this, forth::ReturnStack)
      << "Execute token " << std::hex << (uint32_t) token << "\n\n";// << std::endl << std::endl;
  }

  // Always eat the top of the stack and store the value in a working register.
  DPOP(m_tos);

  // Traverse the word definition like a tree
  do
    {
      // A non primitive word is a consecutive set of primitive identifier
      while (!isPrimitive(token))
        {
          /* std::pair<bool, int32_t> res = m_dictionary.find(token);
          if (!res.first)
            {
              // FIXME: changer le message
              throw ForthUnknownPrimitive(token, __PRETTY_FUNCTION__);
            } */

          if (m_trace) {
            CPP_LOG(logger::Debug)
              << "Token " << m_dictionary.displayToken(token)
              << " is not a primitive " << "\n\n";//<< std::endl;
          }

          // Save the next token to exec after the end of the definition
          Cell32 c = m_ip;
          RPUSH(c);
          if (m_trace) {
            CPP_LOG(logger::Debug)
              << "PUSH "
              << m_dictionary.displayToken(m_ip)
              << "\n";//<< std::endl;
          }

          m_ip = m_dictionary.read16at(token);
          m_ip += 2U;
          token = m_dictionary.read16at(m_ip);
          if (m_trace) {
            CPP_LOG(logger::Debug)
              << "Next token is "
              << m_dictionary.displayToken(token)
              << "\n\n";//<< std::endl << std::endl;
          }
        }

      //
      if (m_trace) {
        CPP_LOG(logger::Debug)
          << "Token " << m_dictionary.displayToken(token)
          <<" is a primitive. Consum it" << "\n";//<< std::endl;
      }
      execPrimitive(token);

      if (m_trace) {
        DPUSH(m_tos);
        CPP_LOG(logger::Debug)
          << "DStack: " << ForthStackDiplayer(this, forth::DataStack)
          << "RStack: " << ForthStackDiplayer(this, forth::ReturnStack)
          << "\n";//<< std::endl;
        DPOP(m_tos);
      }

      // Data stack under/overflow ?
      isStackUnderOverFlow(forth::DataStack);

      // Do not forget than non-primitive words have the
      // token EXIT to pop the return stack to get back
      // IP value.
      if (65535U != m_ip)
        {
          m_ip += 2U;
          token = m_dictionary.read16at(m_ip);
          if (m_trace) {
            CPP_LOG(logger::Debug)
              << "Next token is "
              << m_dictionary.displayToken(token)
              << "\n";//<< std::endl;
          }
        }

      // Return stack under/overflow ?
      depth = isStackUnderOverFlow(forth::ReturnStack);
      if (m_trace) {
        CPP_LOG(logger::Debug) << "RStack: " << ForthStackDiplayer(this, forth::ReturnStack);
      }
    } while (depth > 0);

  // Store the working register
  DPUSH(m_tos);

  if (m_trace) {
    CPP_LOG(logger::Debug)
      << "================ Etat final ====================\n\n";
  }
}

// **************************************************************
//!
// **************************************************************
const char* Forth::completion(std::string const& partial_name)
{
  const char* complete_name;

  complete_name = m_dictionary.completion(m_last_completion, partial_name);
  if (nullptr == complete_name)
    {
      // FIXME eviter de retester des mots deja vus en pasant en param
      // l'ancienne valeur m_last_completion est en la testant pendant
      // l'iteration des mots
      m_last_completion = dictionary().last();
      complete_name = m_dictionary.completion(m_last_completion, partial_name);
    }
  return complete_name;
}

// **************************************************************
//! \param word (in) the number to convert
//! \param number (out) the result of the conversion if the function
//! returned true (else the number is undefined).
//! \return false if the number is malformed (not a number in the
//! current base or too huge to store in a cell.
//! \note: prefixes 'b', 'h' and '0x' are not Forth standard.
// **************************************************************
bool Forth::toNumber(std::string const& word, Cell32& number) const
{
  int base = m_base;
  uint32_t negative = 0;
  uint32_t i = 0;

  // sign
  if ('-' == word[i])
    {
      ++i;
      negative = 1;
    }
  else if ('+' == word[i])
    {
      ++i;
    }

  // decimal
  if (('&' == word[i]) || ('#' == word[i]))
    {
      ++i;
      base = 10;
    }
  // binary ('b' is non standard)
  else if (('b' == word[i]) || ('%' == word[i]))
    {
      ++i;
      base = 2;
    }
  // hexadecimal ('h' is non standard)
  else if (('h' == word[i]) || ('$' == word[i]))
    {
      ++i;
      base = 16;
      if (m_base >= 33)
        return false;
    }
  // hexadecimal, if base < 33.
  else if (('0' == word[i]) && ('x' == word[i + 1]))
    {
      if (m_base < 33)
        {
          i += 2U;
          base = 16;
        }
      else
        {
          return false;
        }
    }
  // numeric value (e.g., ASCII code) an optional ' may be present
  // after the character
  else if ('\'' == word[i])
    {
      int s = word.size();
      if ((2 == s) || ((3 == s) && ('\'' == word[i + 2])))
        {
          number = (negative ? -word[i + 1] : word[i + 1]);
          return true;
        }
      return false;
    }

  // Try to convert the string into number
  try
    {
      std::size_t sz;
      unsigned long val = std::stoul(word.substr(i, word.length() - i), &sz, base);
      number = (negative ? -val : val);
      return (sz + i) == word.length();
    }
  catch (const std::invalid_argument& ia)
    {
      return false;
    }
  catch (const std::out_of_range& oor)
    {
      // Two strategies:
#if (FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE == FORTH_TRUNCATE_OUT_OF_RANGE_NUMBERS)
      {
        // 1st strategy: runcate the number with a warning message to
        // avoid abort the program.
        number = (negative ? LONG_MIN : LONG_MAX);

        std::pair<size_t, size_t> p = STREAM.position();
        std::cerr << FORTH_WARNING_COLOR << "[WARNING] " << STREAM.name() << ":"
                  << p.first << ":" << p.second
                  << " Out of range number '" + word + "' will be truncated"
                  << FORTH_NORMAL_COLOR << std::endl;
        return true;
      }
#else // FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE == FORTH_OUT_OF_RANGE_NUMBERS_ARE_WORDS
      {
        // 2nd strategy: do not consider the word as a number which
        // probably result an "unknown word" error message.
        return false;
      }
#endif // FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE
    }
}

// **************************************************************
//!
// **************************************************************
void Forth::interpreteWordCaseInterprete(std::string const& word)
{
  Cell32 number;
  Cell16 token;
  bool immediate;

  if (m_dictionary.find(word, token, immediate))
    {
      if (m_trace) {
        CPP_LOG(logger::Debug)
          <<"\nExecute word '"
          << word << "'" << "\n"; //<< std::endl;
      }
      execToken(token);
    }
  else if (toNumber(word, number))
    {
      if (m_trace) {
        CPP_LOG(logger::Debug)
          << "PUSH number " << word
          << "\n"; // << std::endl;
      }
      DPUSH(number);
      isStackUnderOverFlow(forth::DataStack);
    }
  else
    {
      throw UnknownForthWord(word);
    }
}

// **************************************************************
//!
// **************************************************************
void Forth::interpreteWordCaseCompile(std::string const& word)
{
  Cell32 number;
  Cell16 token;
  bool immediate;

  if (m_dictionary.find(word, token, immediate))
    {
      if (immediate)
        {
          if (m_trace) {
            CPP_LOG(logger::Debug)
              << "\n" //<< std::endl
              << "Execute immediate word '"
              << word << "'" << "\n"; //<< std::endl;
          }
          execToken(token);
        }
      else
        {
          if (m_trace) {
            CPP_LOG(logger::Debug)
              << "Append new word '" << word
              << "' in dictionary" << "\n"; //<< std::endl;
          }
          m_dictionary.appendCell16(token);
        }
    }
  else if (toNumber(word, number))
    {
      if (m_trace) {
        CPP_LOG(logger::Debug)
          << "Append literal " << number
          << "' in dictionary" << "\n"; //<< std::endl;
      }

      if (number <= 65535U)
        {
          m_dictionary.appendCell16(FORTH_PRIMITIVE_LITERAL_16);
          m_dictionary.appendCell16(number);
        }
      else
        {
          m_dictionary.appendCell16(FORTH_PRIMITIVE_LITERAL_32);
          m_dictionary.appendCell32(number);
        }
    }
  else
    {
      throw UnknownForthWord(word);
    }
}

// **************************************************************
//! \param word the Forth word extracted from the stream.
//! \throw UnknownForthWord if the word does not exist in the
//! dictionary and is not a number.
// **************************************************************
void Forth::interpreteWord(std::string const& word)
{
  if (m_trace) {
    LOGI("Forth interprete word '%s'", word.c_str());
  }
  if (forth::Interprete == m_state)
    {
      interpreteWordCaseInterprete(word);
    }
  else if (forth::Compile == m_state)
    {
      interpreteWordCaseCompile(word);
    }
  else // forth::Comment == m_state
    {
      // Nothing to do ignore the word except end of comment
      if (0 == word.compare(")"))
        {
          execToken(FORTH_PRIMITIVE_RPARENTHESIS);
        }
    }
}

// **************************************************************
//! \param filename the file name of the Forth script (ascii file)
//! to interprete.
//! \return a pair: a boolean indicating if the file has been
//! correctly interpreted (no error) and a string containing the
//! result of the interpreter.
// **************************************************************
std::pair<bool, std::string> Forth::interpreteFile(std::string const& filename)
{
  std::cout << "Forth interpreting the file '" << filename << "'\n";
  LOGI("Forth interpreting the file '%s'", filename.c_str());
  if (STREAM.loadFile(filename))
    {
      return parseStream();
    }
  else
    {
      LOGE("Failed interpreting the file '%s'", filename.c_str());
      std::string msg("failed opening this file. Reason is '");
      msg += strerror(errno);
      msg += "'";
      return std::make_pair(false, msg);
    }
}

// **************************************************************
//! \param code_forth the forth script stored as char*.
//! \param name give a name to the Foth script code_forth. For
//! example when an error occures during the code interpretation
//! this name is used in error message.
//! \return See Forth::interpreteFile
// FIXME: convert char* --> string can be consumns lot of memory ?!
// **************************************************************
std::pair<bool, std::string> Forth::interpreteString(const char* const code_forth,
                                                     std::string const& name)
{
  return Forth::interpreteString(std::string(code_forth), name);
}

// **************************************************************
//! \param code_forth the forth script stored as string.
//! \param name give a name to the Foth script code_forth. For
//! example when an error occures during the code interpretation
//! this name is used in error message.
//! \return See Forth::interpreteFile
// **************************************************************
std::pair<bool, std::string> Forth::interpreteString(std::string const& code_forth,
                                                     std::string const& name)
{
  m_err_stream = 0;
  STREAM.loadString(code_forth, name);
  return parseStream();
}

// **************************************************************
//! Note: all Forth exceptions are caught by this routine and
//! converted into a string to be displayed as an error message.
//! \return See Forth::interpreteFile
// **************************************************************
std::pair<bool, std::string> Forth::parseStream()
{
  std::string word;

  m_err_stream = 0;
  try
    {
      // FIXME: le stream peut ne pas etre termine: attendre
      // FIXME: retourne ok si on lui donne une ligne vide dans un string
      while (STREAM.hasMoreWords())
        {
          interpreteWord(STREAM.nextWord());
        }

      // TODO: checker les piles
      #warning "TODO check if stack depth changed during include!"

      // Check Forth state shall be in execute mode at the end of the
      // stream.  Else this means the end of the stream is truncated
      // with a non finished function definition or non finished
      // commentary.
      if (forth::Interprete != m_state)
        {
          // FIXME: the stream colum information is erroneous because
          // a ForthReader::refill() has been called before. But I like
          // this !
          throw UnfinishedStream(m_state);
        }
      return std::make_pair(true, "ok");
    }

  catch (ForthException const& e)
    {
      LOGC("Caught Forth Exception '%s'", e.message().c_str());
      if (0 == m_err_stream)
        {
          m_err_stream = m_opened_streams;
        }
      return std::make_pair(false, e.message());
    }
}

// **************************************************************
//! \param res See Forth::interpreteFile
// **************************************************************
void Forth::ok(std::pair<bool, std::string> const& res)
{
  if (res.first)
    {
      std::cout << "    " << res.second << std::endl;
    }
  else
    {
      std::pair<size_t, size_t> p = STREAM.position();
      std::cerr << FORTH_ERROR_COLOR << "[ERROR] from "
                << STREAM.name() << ':'
                << p.first << ':'
                << p.second << ' '
                << res.second
                << FORTH_NORMAL_COLOR << std::endl;
      abort(); // FIXME bad location
    }
}

// **************************************************************
//! Forth code: INCLUDE filename.fs
//! Note: this is an indirect recursive function trhough functions
//! eatFile() which can call this routine again through the function
//! execPrimitive() if the Forth word INCLUDE is found.
//! \param filename the new stream to parse.
//! \throw TooManyOpenedStreams if too many streams are opened
//! or if a included file contains an error detected by the interpretor.
// **************************************************************
void Forth::includeFile(std::string const& filename)
{
  // Reached maximum depth of include file including file including ... etc
  if (m_opened_streams >= MAX_OPENED_STREAMS - 1U)
    {
      throw TooManyOpenedStreams();
    }

  // Save the current base in the stream before 'stack'ing it
  STREAM.m_base = m_base;
  m_base = 10;

  // Save the current stream in a stack
  ++m_opened_streams;

  // Parse the included file (recursive)
  std::pair<bool, std::string> res = interpreteFile(filename);
  if (res.first)
    {
      // Succes
      res.second += " parsed ";
      res.second += filename;
      ok(res);

      // Pop the previous stream
      assert(m_opened_streams > 0);
      STREAM.close();
      --m_opened_streams;
      m_base = STREAM.m_base;
    }
  else
    {
      // Compilation failure.
      // Concat the message error from the previous stream
      // given
      std::pair<size_t, size_t> p = STREAM.position();
      std::string msg("\n        including " + STREAM.name() + ':'
                      + std::to_string(p.first) + ':'
                      + std::to_string(p.second) + ' '
                      + res.second);

      // Pop the previous stream
      STREAM.close();
      --m_opened_streams;

      // Call exception that will be caugh by the parseStream()
      throw ForthException(msg);
    }
}

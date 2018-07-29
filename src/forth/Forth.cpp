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
#include "ForthTerminalColor.hpp"
#include "ForthUtils.hpp"
#include "Logger.hpp"
#include <iomanip> // setbase

enum S { CURRENT, SAVED };

#define currentStream() m_streams.stack[m_streams.opened]
#define faultyStream() m_streams.stack[m_streams.faulty]

// **************************************************************
//! Initialize the Forth interpretor context. The dictionary is given
//! as reference to allow inheritance.
//! \param dictionary the reference of (an empty) Forth dictionary.
// **************************************************************
Forth::Forth(forth::Dictionary& dictionary)
  : m_dictionary(dictionary),
    m_return_stack("ReturnStack"),
    m_data_stack("DataStack")
    // m_auxiliary_stack("AuxiliaryStack")
{
  LOGI("Creating Forth interpreter");
  reset();
}

Forth::~Forth()
{
  LOGI("Destroying Forth interpreter");
}

//------------------------------------------------------------------
void Forth::reset()
{
  // If an error occured during the compilation of a Forth word it's
  // partially stored inside the dictionary. Instead of using the
  // SMUDGE bit we restore the dictionary states.
  if ((forth::state::Compile == m_state[CURRENT]) ||
      ((forth::state::Comment == m_state[CURRENT]) &&
       (forth::state::Compile == m_state[SAVED])))
    {
      m_dictionary.restoreContext();
    }

  // Reset Forth states, stacks and streams
  m_ip = 0;
  m_base = 10U;
  m_state[CURRENT] = forth::state::Interprete;
  m_state[SAVED] = forth::state::Interprete;
  m_return_stack.reset();
  m_data_stack.reset();
  //m_auxialiary_stack.reset();
  m_streams.faulty = m_streams.opened;
  m_streams.opened = 0u;
}

// **************************************************************
//! \param base the new displaying base.
//! \return true if the new base can be used. Else return false
//! and the older base is not changed.
// **************************************************************
bool Forth::setBase(const uint8_t base)
{
  if ((base >= 2) && (base <= 36))
    {
      m_base = base;
      return true;
    }
  return false;
}

bool Forth::toNumber(std::string const& word, forth::cell& number) const
{
  const char* msg = nullptr;
  bool res = forth::toNumber(word, m_base, number, msg);
  if (res)
    {
      if (nullptr != msg)
        {
          std::pair<size_t, size_t> p = currentStream().position();
          std::cerr << FORTH_WARNING_COLOR << "[WARNING] " << currentStream().name() << ":"
                    << p.first << ":" << p.second
                    << word << " " << msg
                    << FORTH_NORMAL_COLOR << std::endl;
        }
    }
  // Malformed number
  else if (nullptr != msg)
    throw ForthException(msg);

  return res;
}

// FIXME: A placer dans le mot ':'
// **************************************************************
//! \param word the Forth name to store in the dictionary.
// **************************************************************
void Forth::startCompilingWord(std::string const& word)
{
  if (m_dictionary.find(word))
    {
      std::cout << FORTH_WARNING_COLOR << "[WARNING] Redefining '" << word << "'"
                << FORTH_NORMAL_COLOR << std::endl;
    }

  // Save informations which will be checked when executing the SEMI_COLON primitive.
  auto const& p = currentStream().position();
  m_dictionary.saveContext(p.first, p.second);

  // Append an entry of the word in the dictionary
  m_dictionary.compileWord(word);
}

//------------------------------------------------------------------
//! \param token.
//------------------------------------------------------------------
void Forth::parseStream()
{
  forth::cell number;
  forth::token token;
  bool immediate;

  while (currentStream().hasMoreWords())
    {
      std::string const& word = currentStream().nextWord();
      if (m_dictionary.find(word, token, immediate))
        {
          if ((forth::state::Interprete == m_state[CURRENT]) || (immediate))
            {
              executeToken(token);
            }
          else if (forth::state::Compile == m_state[CURRENT])
            {
              m_dictionary.compileToken(token);
            }
        }
      else if (toNumber(word, number))
        {
          if (forth::state::Interprete == m_state[CURRENT])
            {
              m_data_stack.push(number);
              //FIXME m_data_stack.checkOverflow();
            }
          else
            {
              m_dictionary.compileLiteral(number);
            }
        }
      else
        {
          throw ForthException(MSG_EXCEPTION_UNKNOWN_FORTH_WORD(word));
        }
    }

  // Unfinished script detection
  if (forth::state::Interprete != m_state[CURRENT])
    {
      if (forth::state::Compile == m_state[CURRENT])
        throw ForthException(MSG_EXCEPTION_UNFINISHED_FORTH_DEFINITION(0,0)); // FIXME
    }
}

//------------------------------------------------------------------
//! \param token.
//------------------------------------------------------------------
std::pair<bool, std::string>
Forth::interpreteStream()
{
  try
    {
      parseStream();
      return std::make_pair(true, "ok");
    }

  catch (ForthException const& fe)
    {
      reset();
      return std::make_pair(false, fe.message());
    }
  catch (std::exception const& e)
    {
      reset();
      return std::make_pair(false, e.what());
    }
}

//------------------------------------------------------------------
//! \param
//------------------------------------------------------------------
std::pair<bool, std::string>
Forth::interpreteString(std::string const& forth_script,
                        std::string const& script_name)
{
  LOGI("Forth is interpreting the string '%s'", forth_script.c_str());
  currentStream().loadString(forth_script, script_name);
  return interpreteStream();
}

//------------------------------------------------------------------
//! \param
//------------------------------------------------------------------
std::pair<bool, std::string>
Forth::interpreteFile(std::string const& filename)
{
  LOGI("Forth is interpreting the file '%s'", filename.c_str());
  if (currentStream().loadFile(filename))
    {
      return interpreteStream();
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

//------------------------------------------------------------------
//! \param token.
//------------------------------------------------------------------
void Forth::ok(std::pair<bool, std::string> const& res)
{
  if (res.first)
    {
      std::cout << "    " << res.second << std::endl;
    }
  else
    {
      std::cerr << FORTH_ERROR_COLOR << "[ERROR] from "
                << faultyStream().name() << ':'
                << faultyStream().position().first << ':'
                << faultyStream().position().second << ' '
                << res.second
                << FORTH_NORMAL_COLOR << std::endl;
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
  if (m_streams.opened >= MAX_OPENED_STREAMS - 1U)
    throw ForthException(MSG_EXCEPTION_TOO_MANY_OPENED_FORTH_STREAMS);

  // Save the current base in the stream before 'stack'ing it
  currentStream().m_base = m_base;
  m_base = 10;

  // Save the current stream in a stack
  ++m_streams.opened;

  // Parse the included file (recursive)
  std::pair<bool, std::string> res = interpreteFile(filename);
  if (res.first)
    {
      // Succes
      res.second += " parsed ";
      res.second += filename;
      ok(res);

      // Pop the previous stream
      assert(m_streams.opened > 0);
      currentStream().close();
      --m_streams.opened;
      m_base = currentStream().m_base;
    }
  else
    {
      // Compilation failure.
      // Concat the message error from the previous stream
      // given
      std::pair<size_t, size_t> p = currentStream().position();
      std::string msg("\n        including " + currentStream().name() + ':'
                      + std::to_string(p.first) + ':'
                      + std::to_string(p.second) + ' '
                      + res.second);

      // Pop the previous stream
      currentStream().close();
      assert(m_streams.opened > 0);
      --m_streams.opened;

      // Call exception that will be caugh by the parseStream()
      throw ForthException(msg);
    }
}

// FIXME: a appeler par le mot (
void Forth::skipCommentary()
{
  size_t line = currentStream().position().first;
  size_t column = currentStream().position().second;
  size_t parenthesis = 1;

  while (currentStream().hasMoreWords())
    {
      std::string const& word = currentStream().nextWord();
      if (0 == word.compare(")"))
        {
          --parenthesis;

          // Reach the end of Forth comment
          if (0 == parenthesis)
            return ;
        }
      else if (0 == word.compare("("))
        {
          ++parenthesis;
        }
      else
        {
          // Ignore words
        }
    }

  // Still in a Forth comment when reaching the end of the stream
  if (parenthesis)
    throw ForthException(MSG_EXCEPTION_UNFINISHED_FORTH_COMMENT(line, column));
}

#include "Forth.hpp"
#include <limits.h>

// **************************************************************
//! Initialize the Forth interpretor context. The
//! dictionary is given as reference to allow the inheritance.
//! \param dictionary the reference of (an empty) Forth dictionary.
// **************************************************************
Forth::Forth(ForthDictionary& dictionary, TextColor &color)
  : m_color(color),
    m_state(INTERPRETER_STATE),
    m_dictionary(dictionary)
{
  m_err_stream = 0;
  m_base = 10U;
  abort();
  m_last_completion = m_dictionary.last();
}

// **************************************************************
//! Reset the context like the Forth word ABORT does.
// **************************************************************
void Forth::abort()
{
  if ((COMPILATION_STATE == m_state) ||
      ((COMMENT_STATE == m_state) &&
       (COMPILATION_STATE == m_saved_state)))
    {
      // Drop the Forth word not totaly compiled
      m_dictionary.m_last = m_last_at_colon;
      m_dictionary.m_here = m_here_at_colon;
    }

  m_state = INTERPRETER_STATE;
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
  Forth::abort();
  AbortForth e(msg);
  throw e;
}

// **************************************************************
//! \throw OutOfBoundStack is data stack underflows or overflows
// **************************************************************
void Forth::isDStackUnderOverFlow() const
{
  int32_t depth = DStackDepth();
  if (depth < -1)
    {
      // Underflow
      OutOfBoundStack e(DATA_STACK, depth); throw e;
    }
  else if (depth >= (int32_t) (STACK_SIZE - STACK_UNDERFLOW_MARGIN))
    {
      // Overflow
      OutOfBoundStack e(DATA_STACK, depth); throw e;
    }
}

// **************************************************************
//! \throw OutOfBoundStack is data stack underflows or overflows
// **************************************************************
int32_t Forth::isRStackUnderOverFlow() const
{
  int32_t depth = RStackDepth();
  if (depth < -1)
    {
      // Underflow
      OutOfBoundStack e(RETURN_STACK, depth); throw e;
    }
  else if (depth >= (int32_t) (STACK_SIZE - STACK_UNDERFLOW_MARGIN))
    {
      // Overflow
      OutOfBoundStack e(RETURN_STACK, depth); throw e;
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
void Forth::displayDStack() const
{
  int32_t depth = m_dsp - m_data_stack;

  for (int32_t s = 0; s < depth; ++s)
    {
      std::cout << std::setbase(m_base) << m_data_stack[s] << ' ';
    }
  std::cout << std::endl;
}

// **************************************************************
//! Used for trace and debug.
// **************************************************************
void Forth::displayRStack() const
{
  int32_t depth = m_rsp - m_return_stack;

  for (int32_t s = 0; s < depth; ++s)
    {
      std::cout << std::hex << m_return_stack[s] << ' ';
    }
  std::cout << std::endl;
}

// **************************************************************
//! \param word the Forth name to store in the dictionary.
// **************************************************************
void Forth::create(std::string const& word)
{
  if (m_dictionary.exists(word))
    {
      m_color.yellow();
      std::cout << m_color << "[WARNING] Redefining '" << word << "'";
      m_color.normal();
      std::cout << m_color << std::endl;
    }
  else
    {
      if (m_trace) {
        std::cout << "Append new word definition '" << word << "' in dictionary" << std::endl;
      }
    }

  // Save informations which will be checked
  // when executing the SEMI_COLON primitive.
  m_creating_word = word;
  m_last_at_colon = m_dictionary.m_last;
  m_here_at_colon = m_dictionary.m_here;
  m_depth_at_colon = DStackDepth();

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
    std::cout << "================ Etat initial ==================" << std::endl;
    std::cout << "DStack: "; displayDStack();
    std::cout << "RStack: "; displayRStack();
    std::cout << "Execute token " << std::hex << token << std::endl << std::endl;
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
              ForthUnknownPrimitive e(token, __PRETTY_FUNCTION__); throw e;
            } */

          if (m_trace) {
            std::cout << "Token "; m_dictionary.displayToken(token);
            std::cout << " is not a primitive " << std::endl;
          }

          // Save the next token to exec after the end of the definition
          Cell32 c = m_ip; RPUSH(c);
          if (m_trace) {
            std::cout << "PUSH "; m_dictionary.displayToken(m_ip);
            std::cout << std::endl;
          }

          m_ip = m_dictionary.read16at(token);
          m_ip += 2U;
          token = m_dictionary.read16at(m_ip);
          if (m_trace) {
            std::cout << "Next token is "; m_dictionary.displayToken(token);
            std::cout << std::endl << std::endl;
          }
        }

      //
      if (m_trace) {
        std::cout << "Token "; m_dictionary.displayToken(token);
        std::cout <<" is a primitive. Consum it"
                  << std::endl;
      }
      execPrimitive(token);

      if (m_trace) {
        DPUSH(m_tos); std::cout << "DStack: ";
        displayDStack(); DPOP(m_tos);
        std::cout << "RStack: "; displayRStack();
        std::cout << std::endl;
      }

      // Data stack under/overflow ?
      isDStackUnderOverFlow();

      // Do not forget than non-primitive words have the
      // token EXIT to pop the return stack to get back
      // IP value.
      if (65535U != m_ip)
        {
          m_ip += 2U;
          token = m_dictionary.read16at(m_ip);
          if (m_trace) {
            std::cout << "Next token is "; m_dictionary.displayToken(token);
            std::cout << std::endl;
          }
        }

      // Return stack under/overflow ?
      depth = isRStackUnderOverFlow();
      if (m_trace) {
        std::cout << "RStack: "; displayRStack();
      }
    } while (depth > 0);

  // Store the working register
  DPUSH(m_tos);

  if (m_trace) {
    std::cout << "================ Etat final ===================="
              << std::endl << std::endl;
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
  // numeric value (e.g., ASCII code) an optional ' may be present after the character
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

        m_color.yellow();
        std::cerr << m_color << "[WARNING] " << STREAM.name() << ":"
                  << p.first << ":" << p.second
                  << " Out of range number '" + word + "' will be truncated";
        m_color.normal();
        std::cout << m_color << std::endl;
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
//! \param word the Forth word extracted from the stream.
//! \throw UnknownForthWord if the word does not exist in the
//! dictionary and is not a number.
// **************************************************************
void Forth::interpreteWord(std::string const& word)
{
  Cell32 number;
  Cell16 token;
  bool immediate;

  if (INTERPRETER_STATE == m_state)
    {
      if (m_dictionary.find(word, token, immediate))
        {
          if (m_trace) std::cout << std::endl << "Execute word '" << word << "'" << std::endl;
          execToken(token);
        }
      else if (toNumber(word, number))
        {
          if (m_trace) std::cout << "PUSH number " << word << std::endl;
          DPUSH(number);
          isDStackUnderOverFlow();
        }
      else
        {
          UnknownForthWord e(word); throw e;
        }
    }
  else if (COMPILATION_STATE == m_state)
    {
      if (m_dictionary.find(word, token, immediate))
        {
          if (immediate)
            {
              if (m_trace) std::cout << std::endl << "Execute immediate word '" << word << "'" << std::endl;
              execToken(token);
            }
          else
            {
              if (m_trace) std::cout << "Append new word '" << word << "' in dictionary" << std::endl;
              m_dictionary.appendCell16(token);
            }
        }
      else if (toNumber(word, number))
        {
          if (m_trace) std::cout << "Append literal " << number << "' in dictionary" << std::endl;
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
          UnknownForthWord e(word); throw e;
        }
    }
  else // COMMENT_STATE == m_state
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
  if (m_trace) std::cout << "eating File " << filename << std::endl;
  if (STREAM.loadFile(filename))
    {
      return parseStream();
    }
  else
    {
      std::string msg("failed opening this file. Reason is '");
      msg += strerror(errno);
      msg += "'";
      return std::make_pair(false, msg);
    }
}

// **************************************************************
//! \param code_forth the forth script stored as char*.
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
      #warning " Warning: stack depth changed during include!"

      // Check Forth state shall be in execute mode at the end of the
      // stream.  Else this means the end of the stream is truncated
      // with a non finished function definition or non finished
      // commentary.
      if (INTERPRETER_STATE != m_state)
        {
          // FIXME: the stream colum information is erroneous because
          // a ForthReader::refill() has been called before. But I like
          // this !
          UnfinishedStream e(m_state); throw e;
        }
      return std::make_pair(true, "ok");
    }

  catch (ForthException const& e)
    {
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
      m_color.red();
      std::cerr << m_color << "[ERROR] from "
                << STREAM.name() << ':'
                << p.first << ':'
                << p.second << ' '
                << res.second;
      m_color.normal();
      std::cerr << m_color << std::endl;
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
  // FIXME memoriser m_base

  // Reached maximum depth of include file including file including ... etc
  if (m_opened_streams >= MAX_OPENED_STREAMS - 1U)
    {
      TooManyOpenedStreams e; throw e;
    }

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
      ForthException e(msg);
      throw e;
    }
}

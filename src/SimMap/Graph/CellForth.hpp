#ifndef CELLFORTH_HPP_
#  define CELLFORTH_HPP_

#  include "SimForth.hpp"

// **************************************************************
//
// **************************************************************
class ModifiedStackDepth2: public ForthException
{
public:
  ModifiedStackDepth2(std::string const& cell, const bool b)
    : ForthException(41)
  {
    m_msg = "Stack depth changed during the interpretation of the cell '" + cell;
    if (b)
      {
        m_msg += "' probably you forget to store the result in the cell field";
      }
    else
      {
        m_msg += "' probably you poped the data stack too much";
      }
  }
};

// **************************************************************
//
// **************************************************************
class CellForth
{
public:
  CellForth()
  {
    clearForthWord();
  }

  //! \brief Change the Forth interpreter for all CellForth
  static void forthInterpreter(Forth *forth)
  {
    m_forth = forth;
  }

  //! \brief Accessor. Update the Forth word
  inline void forthWord(std::string const& word)
  {
    m_word = word;
  }

  //! \brief Accessor. Return the current Forth word.
  inline const std::string& forthWord() const
  {
    return m_word;
  }

  //! \brief Clear the current Forth word
  inline void clearForthWord()
  {
    m_word.clear();
  }

  //! \brief Update the Forth word by the one given as param and
  //! interprete it.
  bool interpreteWord(std::string const& word)
  {
    m_word = word;
    return interpreteWord();
  }

  //! Interprete the current Forth word.
  bool interpreteWord()
  {
    LOGI("CellForth: interpreting the word '%s'", m_word.c_str());

    if (m_word.empty())
      return true;

    if (nullptr == m_forth)
      {
        LOGES("Forth Interpreter for CellForth has not been given");
        return false;
      }

    // FIXME verifier que la pile n'a pas change de profondeur
    // int32_t depth = m_forth->DStackDepth();

    // Disable compilation mode
    m_forth->dictionary().smudge(":");

    //
    auto res = m_forth->interpreteString(m_word, name());

    // Enable compilation mode
    m_forth->dictionary().smudge(":");

    //
    //if ((!res.first) && (m_forth->DStackDepth() != depth))
    //  {
    //    ModifiedStackDepth2 e(m_creating_word, depth > m_forth->DStackDepth());
    //    throw e;
    //  }

    // FIXME ne pas laisser faire un std::cout
    m_forth->ok(res);
    LOGI("Result (%u): %s", res.first, res.second);
    return res.first;
  }

  //! \brief Accessor. Virtual. Return the name of the cell
  virtual const std::string& name() const = 0;

  static Forth *m_forth; // FIXME share_ptr

protected:

  //! \brief the Forth word to interprete
  std::string m_word;
};

#endif /* CELLFORTH_HPP_ */

#ifndef CELLFORTH_HPP_
#  define CELLFORTH_HPP_

#  include "SimForth.hpp"

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

  //! \brief Accessor. Update the Forth word
  inline void forthWord(std::string const& word)
  {
    m_word = word;
  }

  //! \brief Accessor. Update the Forth word
  inline void forthWord(const char *word)
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

  //! Interprete the current Forth word.
  virtual bool interprete(SimForth& forth)
  {
    LOGI("CellForth: interpreting the word '%s'. Cost function = %d",
         m_word.c_str(), m_cost);

    if (m_word.empty())
      return true;

    auto res = forth.interpreteCell(m_word, name(), m_cost);
    forth.ok(res);

    LOGI("Result %s. Cost function = %d", res.second.c_str(), m_cost);
    return res.first;
  }

  /*inline bool operator()()
  {
    return interpreteWord();
    }*/

  //! \brief Accessor. Virtual. Return the name of the cell
  virtual const std::string& name() const = 0;

  Cell32 m_cost = 0;

protected:

  //! \brief the Forth word to interprete
  std::string m_word;
};

#endif /* CELLFORTH_HPP_ */

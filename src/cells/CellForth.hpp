#ifndef CELLFORTH_HPP_
#  define CELLFORTH_HPP_

class CellForthExecOnly: public std::exception
{
public:
  ForthException() throw () {}
  ~ForthException() throw () {}
  virtual const char* what () const throw ()
  {
    return "Exception from SimTaDynCellForth: Forth compilation mode is forbidden."
      " inside a SimTaDynCell. Please define your word inside the global Forth";
  }

class CellForth: public Forth
{
public:
  uint32_t result; // result = A1.forthEvalString("A2 + A3 :=");

  // Quand on faire A1.forthEvalString("A2 + A3 :="); et que l'on a 1M de cells
  // les rechercher a chaque fois c'est peut etre un peu long. peut etre du coup
  // compiler dans un dico temporaire (ce qui autorise du coup les : TOTO ... ;)
  // ce qui sera plus rapide.
  //
  // Ou alors faire un mode singleton: compiler en fin de dic puis comparer les
  // x tokens avec l'ancienne definition: si identique on restore 'here' sinon ok.
  //
  // Attention au multitask: 2 cell qui manipulent le dico en meme temps. Mutex ?
  // Ou alors faire un forth multitask ou plutot multifork

  // EvalCell: abort si on tente une defi
  // Comment recuperer simplement le resul d'un mot forth sans faire appel aux pointeurs
  // Comment traduire "nom cellule en address ?"

void eat(std::string const& word) override
{
  Cell32 number;
  Cell16 token;
  Cell16 immediate;

  if (EXECUTION_STATE == state)
    {
      if (toToken(word, token, immediate))
        {
          execToken(token);
        }
      else if (toNumber(word, number))
        {
          DPUSH(number);
        }
      else
        {
          ForthUnknownWord e(word); throw e;
        }
    }
  else // COMPILATION_STATE
    {
      CellForthExecOnly e(word); throw e;
    }

  // **************************************************************
  // FIXME: Unit test: tester eatString("")
  // **************************************************************
  bool eatString(std::string const& code_forth) override
  {
    std::string word;

    try
      {
        reader.useForthString(code_forth);
        while (reader.getNextWord(word))
          {
            eat(word);
          }
        if (DStackSize() > 0)
          {
            DPOP(result);
            return true;
          }
        std::cerr << "[WARN]: empty stack. The last stack element shall be teh result" << std::endl;
        return false;
      }
    catch(std::exception const& e)
      {
        std::cerr << "[ERROR]: " << e.what() << std::endl;
        return false;
      }
  }
}

#endif /* CELLFORTH_HPP_ */

#ifndef CONTEXT_HPP_
#  define CONTEXT_HPP_

#  include "SimTaDynWindow.hpp"
#  include "SimForth.hpp"

#  define SIMTADYN() SimTaDynContext::getInstance()

// Singleton pattern (not thread safe)
class SimTaDynContext
{
public:

  const std::string m_data_path;
  SimTaDynWindow* m_window;
  SimForth m_forth;

  static SimTaDynContext& getInstance()
  {
    return s_SimTaDyn;
  }

  const std::string data_path(const std::string& path) const
  {
     return m_data_path + path;
  }

private:

  SimTaDynContext& operator=(const SimTaDynContext&) = delete;

  SimTaDynContext(const SimTaDynContext&)
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/")),
      m_forth(m_dico)
  {
  }

  SimTaDynContext()
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/")),
      m_forth(m_dico)
  {
    // Start the Forth core
    s_SimTaDyn.m_forth.boot();
    s_SimTaDyn.m_forth.eatFile("forth/core/system.fs");
  }

  ~SimTaDynContext()
  {
  }

  // Singleton
  static SimTaDynContext s_SimTaDyn;

protected:

  SimForthDico m_dico;
};

#endif /* CONTEXT_HPP_ */

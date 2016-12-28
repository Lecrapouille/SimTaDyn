#ifndef CONTEXT_HPP_
#  define CONTEXT_HPP_

#  include "SimTaDynWindow.hpp"
#  include "SimForth.hpp"

#  define SIMTADYN() SimTaDynContext::getInstance()

// Singleton pattern (not thread safe)
class SimTaDynContext // FIXME: peut on faire public SimTaDynWindow: je ne pense pas
{
public:

  const std::string m_data_path;
  SimTaDynWindow* m_window;
  NoColor m_color;
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
      m_forth(m_dictionary, m_color)
  {
  }

  SimTaDynContext()
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/")),
      m_forth(m_dictionary, m_color)
  {
    // Start the Forth core
    s_SimTaDyn.m_forth.boot();
    s_SimTaDyn.m_forth.interpreteFile("forth/core/system.fs");
  }

  ~SimTaDynContext()
  {
  }

  // Singleton
  static SimTaDynContext s_SimTaDyn;

protected:

  SimForthDictionary m_dictionary;
};

#endif /* CONTEXT_HPP_ */

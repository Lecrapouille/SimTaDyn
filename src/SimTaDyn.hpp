#ifndef SIMTADYN_HPP_
#  define SIMTADYN_HPP_

#  include "ForthEditor.hpp"
#  include "MapEditor.hpp"

//! \brief This class defines the context of the SimTaDyn project.  It
//! uses the singleton pattern without a thread safe init but here we
//! do not care because we initialize it for the main() and we do not
//! use threads.
class SimTaDyn
{
public:

  //! \brief Forbid the public usage of the empty constructor.
  SimTaDyn()
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/")),
      m_title("SimTaDyn"),
      m_forth(m_dictionary, m_color)
  {
  }

  //! \brief
  inline std::string data_path(const std::string& path) const
  {
    return m_data_path + path;
  }

  //! \brief
  inline std::string data_path(const char *path) const
  {
    return m_data_path + path;
  }

  //! \brief
  inline std::string data_path() const
  {
    return m_data_path;
  }

protected:

  std::string m_data_path;
  std::string m_title;
  NoColor m_color;

public:

  SimForthDictionary m_dictionary;
  SimForth           m_forth;
  MapEditor          m_mapeditor;
  ForthEditor        m_fortheditor;
};

#endif /* SIMTADYN_HPP_ */

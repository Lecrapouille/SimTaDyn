#ifndef CONFIG_HPP_
#  define CONFIG_HPP_

#  include "Singleton.hpp"
#  include <string>
#  include <glibmm.h>

class Config: public Singleton<Config>
{
  friend class Singleton<Config>;

public:

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

  inline const char *char_data_path() const
  {
    return m_data_path.c_str();
  }

  std::string m_data_path;
  std::string m_title = "SimTaDyn";
  std::string m_log_path = data_path("SimTaDyn.log");
  const uint32_t m_major_version = 0;
  const uint32_t m_minor_version = 2;

private:

  Config()
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/"))
  {
  }
};

#endif /* CONFIG_HPP_ */

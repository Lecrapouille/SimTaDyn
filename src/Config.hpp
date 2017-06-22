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

private:

  Config()
    : m_data_path(Glib::get_home_dir() + std::string("/.SimTaDyn/data/"))
  {
  }
};

namespace config
{
  //! \brief 
  const char project_name[] = "SimTaDyn";
  //! \brief Major version of SimTaDyn
  const uint32_t major_version = 0U;
  //! \brief Minor version of SimTaDyn
  const uint32_t minor_version = 2U;
  //! \brief 
  const char log_path[] = "SimTaDyn.log";
  //! \brief 
  const char tmp_log_path[] = "/tmp/SimTaDyn.log";
  //! \brief Number of elements by pool in containers
  //! used for storing nodes and arcs in a graph
  const uint32_t graph_container_nb_elements = 8U;
}

#endif /* CONFIG_HPP_ */

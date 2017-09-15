#ifndef SIMTADYNFILE_HPP
#define SIMTADYNFILE_HPP

#include "SimTaDynMap.hpp"

class SimTaDynFile
{
public:
  SimTaDynFile()
  {
    LOGI("New SimTaDynFile");
  }

  ~SimTaDynFile()
  {
    LOGI("Deleting SimTaDynFile");
    //TODO Path::instance().remove(m_base_dir);
  }

  //FIXME SceneGraph<SimTaDynMap> m_maps;
  std::vector<SimTaDynMap*> m_maps;
  std::vector<std::string>  m_scripts_forth;
  std::string               m_zip_path;
  std::string               m_base_dir;
  std::string               m_name;
  std::string               m_full_path;
};

#endif

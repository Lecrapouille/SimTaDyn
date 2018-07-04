#ifndef PATHMANAGER_HPP
#define PATHMANAGER_HPP

#  include "Path.hpp"

// **************************************************************
//! \brief
// **************************************************************
class PathManager:
  public Path,
  public LazySingleton<PathManager>
{
private:

  //------------------------------------------------------------------
  //! \brief Mandatory by design.
  //------------------------------------------------------------------
  friend class LazySingleton<PathManager>;

  //------------------------------------------------------------------
  //! \brief Private because of Singleton.
  //------------------------------------------------------------------
  PathManager()
  {
    add(PROJECT_DATA_PATH);
  }

  //------------------------------------------------------------------
  //! \brief Private because of Singleton. Check if resources is still
  //! acquired which show a bug in the management of resources.
  //------------------------------------------------------------------
  ~PathManager() { };
};

#endif

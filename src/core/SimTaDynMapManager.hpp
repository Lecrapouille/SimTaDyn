#ifndef SIMTADYNMAPMANAGER_HPP
#  define SIMTADYNMAPMANAGER_HPP

#  include "SimTaDynFileLoader.hpp"

class SimTaDynMapManager
{
public:

  SimTaDynMapManager() {}
  ~SimTaDynMapManager()
  {
    for (auto& it: m_manager.resources())
      {
        it.save();
      }
  }

  SimTaDynMapPtr create(std::string const& id)
  {
    SimTaDynMapPtr map = m_manager.create(id, );
    if (nullptr != map)
      {
        map_created_success.emit(map);
      }
    else
      {
        map_created_failure.emit(filename, e.message());
      }
  }

  void remove(std::string const& id)
  {
    if (m_manager.remove(id))
      {
        map_removed.emit(map);
      }
    else
      {
        // Do nothing
      }
  }

  SimTaDynMapPtr acquire(std::string const& id)
  {
    return m_manager.acquire(id);
  }

  SimTaDynMapPtr rename(std::string const& oldKey,
                        std::string const& newKey)
  {
    return m_manager.rename(oldKey, newKey, );
  }

  inline size_t size()
  {
    return m_manager.size();
  }

private:

  ResourceManager<std::string, SimTaDynMap> m_manager;
};

#endif

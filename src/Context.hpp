#ifndef CONTEXT_HPP_
#  define CONTEXT_HPP_

#  include "SimTaDynWindow.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class SimTaDynContext
{
public:

  static SimTaDynWindow* mainWindow()
  {
    if (nullptr == s_itself)
      {
        s_itself = new SimTaDynWindow();
      }
    return s_itself;
  }

  static SimForth& forth()
  {
    assert(nullptr != s_itself);
    return s_itself->m_forth;
  }

  static MapEditor& mapEditor()
  {
    assert(nullptr != s_itself);
    return s_itself->m_mapeditor;
  }

  static ForthEditor& forthEditor()
  {
    assert(nullptr != s_itself);
    return s_itself->m_fortheditor;
  }

  inline static std::string data_path(const std::string& path)
  {
    assert(nullptr != s_itself);
    return s_itself->data_path(path);
  }

  inline static std::string data_path(const char *const path)
  {
    assert(nullptr != s_itself);
    return s_itself->data_path(path);
  }

  //! \brief
  inline static std::string data_path()
  {
    assert(nullptr != s_itself);
    return s_itself->data_path();
  }

private:

  SimTaDynWindow& operator=(const SimTaDynWindow&) = delete;

  SimTaDynContext(const SimTaDynWindow&)
  {
  }

  SimTaDynContext()
  {
  }

  ~SimTaDynContext()
  {
    delete s_itself;
  }

  static SimTaDynWindow *s_itself;
};

#endif /* CONTEXT_HPP_ */

#ifndef PATH_HPP_
#  define PATH_HPP_

#  include "File.hpp"
#  include <list>
#  include <sstream>

class Path
{
public:

  Path()
  {
  }

  Path(std::string const& path)
  {
    split(path);
  }

  //! \brief add a directory in the path
  void add(std::string const& path)
  {
    split(path);
  }

  //! \brief add a directory in the path
  void replace(std::string const& path)
  {
    m_paths.clear();
    split(path);
  }

  void remove(std::string const& path)
  {
    m_paths.remove(path);
  }

  std::pair<std::string, bool> find(std::string const& filename) const
  {
    for (auto it: m_paths)
    {
      std::string file = it + filename;
      if (File::exist(file))
        return std::make_pair(file, true);
    }
    if (File::exist(filename))
      return std::make_pair(filename, true);

    // Not found
    return std::make_pair(std::string(), false);
  }

  std::string const &toString() const
  {
    return m_path;
  }

protected:

  void update()
  {
    m_path = "";
    for (auto it: m_paths)
      {
        m_path += it;
        m_path.pop_back(); // Remove the '/' char
        m_path += m_delimiter;
      }
  }

  void split(std::string const& path)
  {
    std::stringstream ss(path);
    std::string directory;

    while (std::getline(ss, directory, m_delimiter))
      {
        if (directory.empty())
          continue ;

        if ((*directory.rbegin() == '\\') || (*directory.rbegin() == '/'))
          m_paths.push_back(directory);
        else
          m_paths.push_back(directory + "/");
      }
    update();
  }

  char m_delimiter = ':';
  std::list<std::string> m_paths;
  std::string m_path;
};

#endif

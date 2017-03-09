#ifndef FILE_HPP_
#  define FILE_HPP_

#  include <string>
#  include <sys/stat.h>

class File
{
public:

  //! \brief give the file name with its extension from a given path
  inline static std::string shortNameWithExtension(std::string const& path)
  {
    return path.substr(path.find_last_of("/") + 1);
  }

  inline static std::string shortName(std::string const& path)
  {
    return path.substr(path.find_last_of("/") + 1,
                       path.find_last_of("."));
  }

  inline static std::string extension(std::string const& filename)
  {
    return filename.substr(filename.find_last_of(".") + 1);
  }

  inline static bool exist(std::string const& path)
  {
    struct stat buffer;
    return 0 == stat(path.c_str(), &buffer);
  }
};

#endif /* FILE_HPP_ */

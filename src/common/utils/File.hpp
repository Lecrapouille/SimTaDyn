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
    std::string::size_type pos = path.find_last_of("\\/");
    if (pos != std::string::npos)
      return path.substr(pos + 1, std::string::npos);
    return path;
  }

  //! \brief give the file name without its extension from a given path
  inline static std::string shortName(std::string const& path)
  {
    std::string filename = shortNameWithExtension(path);
    return filename.substr(0, filename.find_last_of("."));
  }

  //! \brief give the file extension
  inline static std::string extension(std::string const& path)
  {
    std::string::size_type pos = path.find_last_of(".");
    if (pos != std::string::npos)
      return path.substr(pos + 1, std::string::npos);
    return "";
  }

  //! \brief Check if a file exits.
  //!
  //! Beware of race condition ! Do not use this function for opening
  //! a file but use instead the open() function and check the error
  //! code. Indeed, the file may be deleted between this function and
  //! the open() function because theses two functions will not be
  //! atomic.
  inline static bool exist(std::string const& path)
  {
    struct stat buffer;
    return 0 == stat(path.c_str(), &buffer);
  }
};

#endif /* FILE_HPP_ */

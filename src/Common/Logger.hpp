#ifndef LOGGER_HPP_
#  define LOGGER_HPP_

#  include <mutex>
#  include <stdarg.h>
#  include <fstream>
#  include <cassert>
#  include <sstream>
#  include <string>
#  include <iostream>

namespace logger
{
  enum LoggerSeverity { None, Info, Debug, Warning, Failed, Error, Fatal, MaxLoggerSeverity = Fatal };
}

// **************************************************************
//
// **************************************************************
class ILogger
{
public:

  void log(const char* format, ...);
  void log(std::ostream *stream, enum logger::LoggerSeverity severity, const char* format, ...);
  template <class T> ILogger& operator<<(const T& tolog);

protected:

  void currentDate();
  void currentTime();

private:

  virtual void write(std::string const& message) = 0;
  virtual void write(const char *message, const int length = -1) = 0;
  virtual void beginLine() = 0;

protected:

  std::mutex m_mutex;
  constexpr static const uint32_t c_buffer_size = 1024;
  char m_buffer[c_buffer_size];
  enum logger::LoggerSeverity m_severity = logger::None;
  char m_buffer_time[32];
  std::ostream *m_stream = nullptr;
};

// **************************************************************
//
// **************************************************************
class FileLogger: public ILogger
{
private:

  virtual void open(std::string const& filename) = 0;
  virtual void close() = 0;
  virtual void header() = 0;
  virtual void footer() = 0;
};

#endif /* LOGGER_HPP_ */

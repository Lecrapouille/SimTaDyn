#include "Logger.hpp"
#include <cstring>

void ILogger::currentDate()
{
  time_t current_time = time(nullptr);

  strftime(m_buffer_time, sizeof (m_buffer_time), "[%d/%m/%Y]", localtime(&current_time));
}

void ILogger::currentTime()
{
  time_t current_time = time(nullptr);

  strftime(m_buffer_time, sizeof (m_buffer_time), "[%H:%M:%S]", localtime(&current_time));
}

void ILogger::log(enum logger::LoggerSeverity severity, const char* format, ...)
{
  int n;
  va_list params;

  m_mutex.lock();

  m_severity = severity;
  beginLine();
  va_start(params, format);
  n = vsnprintf(m_buffer, c_buffer_size, format, params);
  va_end(params);
  write(m_buffer);

  if ('\n' != m_buffer[n - 1])
    write("\n");

  m_mutex.unlock();
}

void ILogger::log(const char* format, ...)
{
  va_list params;

  m_mutex.lock();

  va_start(params, format);
  vsnprintf(m_buffer, c_buffer_size, format, params);
  va_end(params);
  write(m_buffer);

  m_mutex.unlock();
}

template <class T> ILogger& ILogger::operator <<(const T& to_log)
{
  std::ostringstream stream;
  stream << to_log;
  write(stream.str());

  return *this;
}

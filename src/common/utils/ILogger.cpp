#include "ILogger.hpp"
#include <cstring>

const char *ILogger::strtime()
{
  currentTime();
  return m_buffer_time;
}

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

void ILogger::log(std::ostream *stream, enum logger::LoggerSeverity severity, const char* format, ...)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  int n;
  va_list params;

  m_severity = severity;
  m_stream = stream;
  beginLine();
  va_start(params, format);
  n = vsnprintf(m_buffer, c_buffer_size - 2, format, params);
  va_end(params);

  // Add a '\n' if missing
  if ('\n' != m_buffer[n - 1])
    {
      m_buffer[n] = '\n';
      m_buffer[n + 1] = '\0';
    }

  write(m_buffer);

  m_stream = nullptr;
}

void ILogger::log(const char* format, ...)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  va_list params;

  va_start(params, format);
  vsnprintf(m_buffer, c_buffer_size, format, params);
  va_end(params);
  write(m_buffer);
}

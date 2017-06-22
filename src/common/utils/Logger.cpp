#include "Logger.hpp"

static const char *c_str_severity[logger::MaxLoggerSeverity + 1] =
  {
    [logger::None] = "",
    [logger::Info] = "[INFO]",
    [logger::Debug] = "[DEBUG]",
    [logger::Warning] = "[WARNI]",
    [logger::Failed] = "[FAILU]",
    [logger::Error] = "[ERROR]",
    [logger::Fatal] = "[FATAL]"
  };

Logger::Logger(std::string const& filename)
{
  open(filename);
}

Logger::~Logger()
{
  close();
}

void Logger::open(std::string const& filename)
{
  m_file.open(filename.c_str());
  if (!m_file.is_open())
    {
      m_file.open(config::tmp_log_path);
    }
  if (m_file.is_open())
    {
      header();
    }
}

void Logger::close()
{
  if (!m_file.is_open())
    return ;

  footer();
  m_file.close();
}

void Logger::write(const char *message, const int /*length*/)
{
  if (nullptr != m_stream)
    {
      (*m_stream) << message;
      m_stream->flush();
    }

  if (!m_file.is_open())
    return ;

  m_file << message;
  m_file.flush();
}

void Logger::beginLine()
{
  currentTime();
  write(m_buffer_time);
  write(c_str_severity[m_severity]);
}

void Logger::header()
{
  currentDate();
  log("===============================================\n"
      "  %s %u.%u - Event log - %s\n"
      "===============================================\n\n",
      config::project_name,
      config::major_version,
      config::minor_version,
      m_buffer_time);
}

void Logger::footer()
{
  currentTime();
  log("\n===============================================\n"
      "  %s log closed at %s\n"
      "===============================================\n\n",
      config::project_name, m_buffer_time);
}

ILogger& Logger::operator<<(const logger::LoggerSeverity& severity)
{
  write(c_str_severity[severity]);
  return *this;
}

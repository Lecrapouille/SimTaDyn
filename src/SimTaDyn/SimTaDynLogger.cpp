#include "SimTaDynLogger.hpp"

static const char *c_str_severity[logger::MaxLoggerSeverity + 1] =
  {
    [logger::None] = "",
    [logger::Info] = "[INFO]",
    [logger::Debug] = "[DBUG]",
    [logger::Warning] = "[WARN]",
    [logger::Failed] = "[FAIL]",
    [logger::Error] = "[ERRO]",
    [logger::Fatal] = "[DEAD]"
  };

SimTaDynLogger::SimTaDynLogger(std::string const& filename)
{
  open(filename);
}

SimTaDynLogger::~SimTaDynLogger()
{
  close();
}

void SimTaDynLogger::open(std::string const& filename)
{
  m_file.open(filename.c_str());
  if (!m_file.is_open())
    {
      m_file.open("/tmp/SimTaDyn.log");
    }
  if (m_file.is_open())
    {
      header();
    }
}

void SimTaDynLogger::close()
{
  if (!m_file.is_open())
    return ;

  footer();
  m_file.close();
}

void SimTaDynLogger::write(const char *message)
{
  if (!m_file.is_open())
    return ;

  m_file << message;
  m_file.flush();
}

void SimTaDynLogger::beginLine()
{
  currentDate();
  write(m_buffer_time);
  write(c_str_severity[m_severity]);
}

void SimTaDynLogger::header()
{
  currentDate();
  log("===============================================\n"
      "  SimTaDyn %u.%u - Event log - %s\n"
      "===============================================\n",
      Config::instance().m_major_version,
      Config::instance().m_minor_version,
      m_buffer_time);
}

void SimTaDynLogger::footer()
{
  currentTime();
  log("===============================================\n"
      "  SimTaDyn closed at %s\n"
      "===============================================\n\n",
      m_buffer_time);
}

ILogger& SimTaDynLogger::operator<<(const logger::LoggerSeverity& severity)
{
  write(c_str_severity[severity]);
  return *this;
}

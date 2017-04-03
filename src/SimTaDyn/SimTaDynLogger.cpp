#include "SimTaDynLogger.hpp"

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

void SimTaDynLogger::write(const char *message, const int /*length*/)
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

void SimTaDynLogger::beginLine()
{
  currentTime();
  write(m_buffer_time);
  write(c_str_severity[m_severity]);
}

void SimTaDynLogger::header()
{
  currentDate();
  log("===============================================\n"
      "  SimTaDyn %u.%u - Event log - %s\n"
      "===============================================\n\n",
      simtadyn::m_major_version,
      simtadyn::m_minor_version,
      m_buffer_time);
}

void SimTaDynLogger::footer()
{
  currentTime();
  log("\n===============================================\n"
      "  SimTaDyn log closed at %s\n"
      "===============================================\n\n",
      m_buffer_time);
}

ILogger& SimTaDynLogger::operator<<(const logger::LoggerSeverity& severity)
{
  write(c_str_severity[severity]);
  return *this;
}

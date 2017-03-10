#ifndef SIMTADYN_LOGGER_HPP_
#  define SIMTADYN_LOGGER_HPP_

#  include "Logger.hpp"
#  include "Config.hpp"
#  include "Singleton.hpp"
#  include "File.hpp"
//#  include "TextColor.hpp"

class SimTaDynLogger: public FileLogger, public Singleton<SimTaDynLogger>
{
  friend class Singleton<SimTaDynLogger>;

public:

  SimTaDynLogger(std::string const& filename = Config::instance().m_log_path);
  virtual ~SimTaDynLogger();
  ILogger& operator<<(const logger::LoggerSeverity& severity);

protected:

  virtual void open(std::string const& filename) override;
  virtual void close() override;

private:

  inline virtual void write(std::string const& message) override
  {
    write(message.c_str());
  }

  virtual void write(const char *message, const int length = -1) override;
  virtual void header() override;
  virtual void footer() override;
  virtual void beginLine() override;

private:

  std::ofstream m_file;
};

#define SHORT_FILENAME File::shortNameWithExtension(__FILE__).c_str()

//! \brief Log C++ like. Example:  CPP_LOG(logger::Fatal) << "test\n";
#define CPP_LOG(severity, ...) SimTaDynLogger::instance() << severity << '[' << __FILE__ << "::" << __LINE__ << ']'


//! \brief Basic log without severity or file and line information. 'B' for Basic.
#define LOGB(...)         SimTaDynLogger::instance().log(nullptr, logger::None, __VA_ARGS__)
//! \brief Information Log.
#define LOGI(format, ...) SimTaDynLogger::instance().log(nullptr, logger::Info, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Debug Log.
#define LOGD(format, ...) SimTaDynLogger::instance().log(nullptr, logger::Debug, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Warning Log.
#define LOGW(format, ...) SimTaDynLogger::instance().log(nullptr, logger::Warning, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Failure Log.
#define LOGF(format, ...) SimTaDynLogger::instance().log(nullptr, logger::Failed, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Error Log.
#define LOGE(format, ...) SimTaDynLogger::instance().log(nullptr, logger::Error, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Fatal Log.
#define LOGX(format, ...) SimTaDynLogger::instance().log(nullptr, logger::Fatal, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)

#define LOGIS(format, ...) SimTaDynLogger::instance().log(&std::cout, logger::Info, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGDS(format, ...) SimTaDynLogger::instance().log(&std::cout, logger::Debug, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGWS(format, ...) SimTaDynLogger::instance().log(&std::cerr, logger::Warning, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGFS(format, ...) SimTaDynLogger::instance().log(&std::cerr, logger::Failed, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGES(format, ...) SimTaDynLogger::instance().log(&std::cerr, logger::Error, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGXS(format, ...) SimTaDynLogger::instance().log(&std::cerr, logger::Fatal, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)

#endif /* SIMTADYN_LOGGER_HPP_ */

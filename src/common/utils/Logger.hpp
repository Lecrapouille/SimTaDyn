#ifndef LOGGER_HPP_
#  define LOGGER_HPP_

#  include "Singleton.hpp"
#  include "ILogger.hpp"
#  include "Config.hpp"
#  include "File.hpp"
//#  include "TextColor.hpp"

class Logger: public FileLogger, public Singleton<Logger>
{
  friend class Singleton<Logger>;

public:

  Logger(std::string const& filename = config::log_path);
  virtual ~Logger();
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
#define CPP_LOG(severity, ...) Logger::instance() << severity << '[' << __FILE__ << "::" << __LINE__ << ']'


//! \brief Basic log without severity or file and line information. 'B' for Basic.
#define LOGB(...)         Logger::instance().log(nullptr, logger::None, __VA_ARGS__)
//! \brief Information Log.
#define LOGI(format, ...) Logger::instance().log(nullptr, logger::Info, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Debug Log.
#define LOGD(format, ...) Logger::instance().log(nullptr, logger::Debug, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Warning Log.
#define LOGW(format, ...) Logger::instance().log(nullptr, logger::Warning, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Failure Log.
#define LOGF(format, ...) Logger::instance().log(nullptr, logger::Failed, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Error Log.
#define LOGE(format, ...) Logger::instance().log(nullptr, logger::Error, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
//! \brief Fatal Log.
#define LOGX(format, ...) Logger::instance().log(nullptr, logger::Fatal, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)

#define LOGIS(format, ...) Logger::instance().log(&std::cout, logger::Info, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGDS(format, ...) Logger::instance().log(&std::cout, logger::Debug, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGWS(format, ...) Logger::instance().log(&std::cerr, logger::Warning, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGFS(format, ...) Logger::instance().log(&std::cerr, logger::Failed, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGES(format, ...) Logger::instance().log(&std::cerr, logger::Error, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)
#define LOGXS(format, ...) Logger::instance().log(&std::cerr, logger::Fatal, "[%s::%d] " format, SHORT_FILENAME, __LINE__, ##__VA_ARGS__)

#endif /* LOGGER_HPP_ */

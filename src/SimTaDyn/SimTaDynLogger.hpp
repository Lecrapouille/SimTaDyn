#ifndef SIMTADYN_LOGGER_HPP_
#  define SIMTADYN_LOGGER_HPP_

#  include "Logger.hpp"
#  include "Config.hpp"
#  include "Singleton.hpp"
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

  virtual void write(const char *message) override;
  virtual void header() override;
  virtual void footer() override;
  virtual void beginLine() override;

private:

  std::ofstream m_file;
};

#define LOGN(...) SimTaDynLogger::instance().log(logger::None, __VA_ARGS__)
#define LOGI(format, ...) SimTaDynLogger::instance().log(logger::Info, "[%s::%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGD(format, ...) SimTaDynLogger::instance().log(logger::Debug, "[%s::%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGW(format, ...) SimTaDynLogger::instance().log(logger::Warning, "[%s::%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGF(format, ...) SimTaDynLogger::instance().log(logger::Failed, "[%s::%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGE(format, ...) SimTaDynLogger::instance().log(logger::Error, "[%s::%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGX(format, ...) SimTaDynLogger::instance().log(logger::Fatal, "[%s::%d] " format, __FILE__, __LINE__, ##__VA_ARGS__)

#endif /* SIMTADYN_LOGGER_HPP_ */

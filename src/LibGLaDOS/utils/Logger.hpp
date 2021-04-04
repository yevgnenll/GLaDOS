#ifndef GLADOS_LOGGER_HPP
#define GLADOS_LOGGER_HPP

#include <iostream>
#include <string>

#include "Enumeration.h"
#include "StringFormatter.h"

namespace GLaDOS {
  struct SourceLocation {
    SourceLocation(const char* filename, int line, const char* functionName);

    std::string getFileName() const;

    const char* mFilename{nullptr};
    int mLine{0};
    const char* mFunctionName{nullptr};
  };

  struct LogMessage {
    LogMessage(SourceLocation loc, std::string loggerName, LogLevel level, std::string message);

    SourceLocation mLocation;
    TimePoint mTime;
    std::string mLoggerName;
    LogLevel mLevel;
    TimeZone mTimeZone{TimeZone::UTC};
    std::size_t mThreadId{0};
    int mPid{0};
    std::string mMessage;
  };

  class Logger {
  public:
    Logger(const std::string& loggerName);
    ~Logger() = default;

    std::string getName() const;
    LogLevel getLevel() const;
    void setLevel(LogLevel level);

    template <typename T, typename... Ts>
    void trace(SourceLocation loc, const T& fmt, const Ts&... args);
    template <typename T>
    void trace(SourceLocation loc, const T& fmt);
    template <typename T, typename... Ts>
    void debug(SourceLocation loc, const T& fmt, const Ts&... args);
    template <typename T>
    void debug(SourceLocation loc, const T& fmt);
    template <typename T, typename... Ts>
    void info(SourceLocation loc, const T& fmt, const Ts&... args);
    template <typename T>
    void info(SourceLocation loc, const T& fmt);
    template <typename T, typename... Ts>
    void warn(SourceLocation loc, const T& fmt, const Ts&... args);
    template <typename T>
    void warn(SourceLocation loc, const T& fmt);
    template <typename T, typename... Ts>
    void error(SourceLocation loc, const T& fmt, const Ts&... args);
    template <typename T>
    void error(SourceLocation loc, const T& fmt);

  private:
    template <typename T, typename... Ts>
    void log(SourceLocation loc, LogLevel level, const T& fmt, const Ts&... args);
    template <typename T>
    void log(SourceLocation loc, LogLevel level, const T& fmt);
    bool shouldLog(LogLevel level) const;
    static std::string formatLogMessage(const LogMessage& msg);
    static std::string formatTime(const char* format, const TimePoint& tp, TimeZone timeZone);
    static std::string formatStdTime(const TimePoint& tp, TimeZone timeZone);
    static std::chrono::milliseconds ms(TimePoint tp);
    static std::string getLevelColor(LogLevel level);

    std::string mLoggerName;
    LogLevel mLevel{LogLevel::Trace};
  };

  template <typename T, typename... Ts>
  void Logger::log(SourceLocation loc, LogLevel level, const T& fmt, const Ts&... args) {
    if (!shouldLog(level)) {
      return;
    }

    std::string log = formatLogMessage({loc, mLoggerName, level, StringFormatter::formatTo(fmt, args...)});
    std::cout.write(log.c_str(), static_cast<long>(log.size()));
  }

  template <typename T>
  void Logger::log(SourceLocation loc, LogLevel level, const T& fmt) {
    if (!shouldLog(level)) {
      return;
    }

    std::string log = formatLogMessage({loc, mLoggerName, level, fmt});
    std::cout.write(log.c_str(), static_cast<long>(log.size()));
  }

  template <typename T, typename... Ts>
  void Logger::trace(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Trace, fmt, args...);
  }

  template <typename T>
  void Logger::trace(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Trace, fmt);
  }

  template <typename T, typename... Ts>
  void Logger::debug(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Debug, fmt, args...);
  }

  template <typename T>
  void Logger::debug(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Debug, fmt);
  }

  template <typename T, typename... Ts>
  void Logger::info(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Info, fmt, args...);
  }

  template <typename T>
  void Logger::info(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Info, fmt);
  }

  template <typename T, typename... Ts>
  void Logger::warn(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Warn, fmt, args...);
  }

  template <typename T>
  void Logger::warn(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Warn, fmt);
  }

  template <typename T, typename... Ts>
  void Logger::error(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Error, fmt, args...);
  }

  template <typename T>
  void Logger::error(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Error, fmt);
  }
}  // namespace GLaDOS

#endif

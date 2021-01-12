#ifndef GAMEENGINE_DEBUG_H
#define GAMEENGINE_DEBUG_H

#include <iostream>
#include <string>

#include "platform/Platform.h"
#include "utils/Enumeration.h"
#include "utils/StringFormatter.h"

namespace GameEngine {
  struct SourceLocation {
    SourceLocation(const char* filename, int line, const char* functionName)
        : mFilename{filename}, mLine{line}, mFunctionName{functionName} {}

    std::string getFileName() const;

    const char* mFilename{nullptr};
    int mLine{0};
    const char* mFunctionName{nullptr};
  };

  struct LogMessage {
    LogMessage(SourceLocation loc, std::string loggerName, LogLevel level, std::string message)
        : mLocation{loc}, mTime{SystemClock::now()}, mLoggerName{std::move(loggerName)}, mLevel{level}, mThreadId{Platform::getThreadId()}, mPid{Platform::getPid()}, mMessage{std::move(message)} {}

    SourceLocation mLocation;
    TimePoint mTime;
    std::string mLoggerName;
    LogLevel mLevel;
    TimeZone mTimeZone{TimeZone::UTC};
    std::size_t mThreadId{0};
    int mPid{0};
    std::string mMessage;
  };

  class Debug {
  public:
    Debug(std::string name);
    ~Debug() = default;

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

    static Debug* getInstance();

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

    std::string mName;
    LogLevel mLevel{LogLevel::Info};

    static Debug* instance;
  };

  template <typename T, typename... Ts>
  void Debug::log(SourceLocation loc, LogLevel level, const T& fmt, const Ts&... args) {
    if (!shouldLog(level)) {
      return;
    }

    std::string log = formatLogMessage({loc, mName, level, StringFormatter::formatTo(fmt, args...)});
    std::cout.write(log.c_str(), static_cast<long>(log.size()));
  }

  template <typename T>
  void Debug::log(SourceLocation loc, LogLevel level, const T& fmt) {
    if (!shouldLog(level)) {
      return;
    }

    std::string log = formatLogMessage({loc, mName, level, fmt});
    std::cout.write(log.c_str(), static_cast<long>(log.size()));
  }

  template <typename T, typename... Ts>
  void Debug::trace(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Trace, fmt, args...);
  }

  template <typename T>
  void Debug::trace(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Trace, fmt);
  }

  template <typename T, typename... Ts>
  void Debug::debug(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Debug, fmt, args...);
  }

  template <typename T>
  void Debug::debug(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Debug, fmt);
  }

  template <typename T, typename... Ts>
  void Debug::info(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Info, fmt, args...);
  }

  template <typename T>
  void Debug::info(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Info, fmt);
  }

  template <typename T, typename... Ts>
  void Debug::warn(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Warn, fmt, args...);
  }

  template <typename T>
  void Debug::warn(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Warn, fmt);
  }

  template <typename T, typename... Ts>
  void Debug::error(SourceLocation loc, const T& fmt, const Ts&... args) {
    log(loc, LogLevel::Error, fmt, args...);
  }

  template <typename T>
  void Debug::error(SourceLocation loc, const T& fmt) {
    log(loc, LogLevel::Error, fmt);
  }
}  // namespace GameEngine

#endif

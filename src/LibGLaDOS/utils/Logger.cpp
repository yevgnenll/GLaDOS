#include "Logger.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

#include "StringUtils.h"
#include "platform/OSTypes.h"
#include "platform/Platform.h"
#include "utils/Utility.h"

namespace GLaDOS {
  SourceLocation::SourceLocation(const char* filename, int line, const char* functionName)
      : mFilename{filename}, mLine{line}, mFunctionName{functionName} {
  }

  LogMessage::LogMessage(SourceLocation loc, std::string loggerName, LogLevel level, std::string message)
      : mLocation{loc}, mTime{SystemClock::now()}, mLoggerName{std::move(loggerName)}, mLevel{level}, mThreadId{Platform::getThreadId()}, mPid{Platform::getPid()}, mMessage{std::move(message)} {
  }

  std::string SourceLocation::getFileName() const {
    auto path = std::string(mFilename);
    return path.substr(path.find_last_of(PATH_SEPARATOR) + 1);
  }

  Logger::Logger(const std::string& loggerName) : mLoggerName{loggerName} {
  }

  std::string Logger::getName() const {
    return mLoggerName;
  }

  LogLevel Logger::getLevel() const {
    return mLevel;
  }

  void Logger::setLevel(LogLevel level) {
    mLevel = level;
  }

  bool Logger::shouldLog(LogLevel level) const {
    return mLevel <= level;
  }

  std::string Logger::formatLogMessage(const LogMessage& msg) {
    std::string result;
    result
        .append(ANSI_COLOR_GREY)
        .append(formatStdTime(msg.mTime, msg.mTimeZone))
        .append(Logger::getLevelColor(msg.mLevel))
        .append(StringUtils::padLeft(msg.mLevel.toString(), 7))
        .append(ANSI_COLOR_RESET)
        .append(" (")
        .append(ANSI_COLOR_YELLOW)
        .append(StringUtils::padLeft(msg.mLocation.getFileName().append(":").append(std::to_string(msg.mLocation.mLine)), 30))
        .append(ANSI_COLOR_RESET)
        .append(") ")
        .append(StringUtils::normalize(msg.mPid))
        .append("-")
        .append(StringUtils::normalize(msg.mThreadId))
        .append(" ")
        .append(ANSI_COLOR_GREEN)
        .append(StringUtils::padLeft(msg.mLoggerName, 7))
        .append(ANSI_COLOR_RESET)
        .append(" : ")
        .append(msg.mMessage)
        .append("\n");
    return result;
  }

  std::string Logger::formatTime(const char* format, const TimePoint& tp, TimeZone timeZone) {
    std::time_t inTime = SystemClock::to_time_t(tp);
    std::tm* tm;
    switch (timeZone) {
      case TimeZone::UTC:
        tm = std::gmtime(&inTime);
        break;
      case TimeZone::Local:
        tm = std::localtime(&inTime);
        break;
      default:
        throw std::runtime_error{"unsupported time zone"};
    }

    std::stringstream ss;
    ss << std::put_time(tm, format) << "." << StringUtils::padLeft(StringUtils::normalize(ms(tp).count()), 3, '0');
    return ss.str();
  }

  std::string Logger::formatStdTime(const TimePoint& tp, TimeZone timeZone) {
    return formatTime("%Y-%m-%d %X", tp, timeZone);
  }

  std::chrono::milliseconds Logger::ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()) % 1000;
  }

  std::string Logger::getLevelColor(LogLevel level) {
    switch (level) {
      case LogLevel::Trace:
        return ANSI_COLOR_BLUE;
      case LogLevel::Debug:
        return ANSI_COLOR_GREY;
      case LogLevel::Info:
        return ANSI_COLOR_GREEN;
      case LogLevel::Warn:
        return ANSI_COLOR_YELLOW;
      case LogLevel::Error:
        return ANSI_COLOR_RED;
      default:
        break;
    }

    return "";
  }
}  // namespace GLaDOS
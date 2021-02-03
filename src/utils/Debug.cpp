#include "Debug.h"

#include <ctime>
#include <iomanip>
#include <sstream>

#include "StringUtils.h"
#include "platform/OSTypes.h"

namespace GLaDOS {
  std::string SourceLocation::getFileName() const {
    auto path = std::string(mFilename);
    return path.substr(path.find_last_of(PATH_SEPARATOR) + 1);
  }

  std::string Debug::getName() const {
    return mName;
  }

  LogLevel Debug::getLevel() const {
    return mLevel;
  }

  void Debug::setLevel(LogLevel level) {
    mLevel = level;
  }

  bool Debug::shouldLog(LogLevel level) const {
    return mLevel >= level;
  }

  std::string Debug::formatLogMessage(const LogMessage& msg) {
    std::string result;
    result.append(formatStdTime(msg.mTime, msg.mTimeZone))
        .append(StringUtils::padLeft(logLevelName[static_cast<std::size_t>(msg.mLevel)], 7))
        .append(" (")
        .append(msg.mLocation.getFileName())
        .append(":")
        .append(std::to_string(msg.mLocation.mLine))
        .append(") ")
        .append(StringUtils::normalize(msg.mPid))
        .append("-")
        .append(StringUtils::normalize(msg.mThreadId))
        .append(" ")
        .append(StringUtils::padLeft(msg.mLoggerName, 10))
        .append(" : ")
        .append(msg.mMessage)
        .append("\n");
    return result;
  }

  std::string Debug::formatTime(const char* format, const TimePoint& tp, TimeZone timeZone) {
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

  std::string Debug::formatStdTime(const TimePoint& tp, TimeZone timeZone) {
    return formatTime("%Y-%m-%d %X", tp, timeZone);
  }

  std::chrono::milliseconds Debug::ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()) % 1000;
  }
}  // namespace GLaDOS
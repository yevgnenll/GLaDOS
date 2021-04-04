#include "LoggerRegistry.h"

#include "Logger.hpp"
#include "Utility.h"

namespace GLaDOS {
  LoggerRegistry::LoggerRegistry() {
    mDefaultLogger = NEW_T(Logger("default"));
    mLoggers.try_emplace("default", mDefaultLogger);
  }

  LoggerRegistry::~LoggerRegistry() {
    deallocValueInMap(mLoggers);
  }

  Logger* LoggerRegistry::getLogger(const std::string& name) {
    UnorderedMap<std::string, Logger*>::const_iterator find = mLoggers.find(name);
    return find == mLoggers.end() ? mDefaultLogger : find->second;
  }

  Logger* LoggerRegistry::getDefaultLogger() {
    return mDefaultLogger;
  }

  void LoggerRegistry::registerNewLogger(Logger* logger) {
    std::lock_guard<std::mutex> lock(mRegistryMutex);
    std::string name = logger->getName();
    if (mLoggers.find(name) != mLoggers.end()) {
      LOG_ERROR("default", "Logger name {0} already exists!", name);
      return;
    }
    mLoggers.try_emplace(name, logger);
  }
}  // namespace GLaDOS
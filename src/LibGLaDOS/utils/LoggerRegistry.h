#ifndef GLADOS_LOGGERREGISTRY_H
#define GLADOS_LOGGERREGISTRY_H

#include <mutex>
#include <string>

#include "Singleton.hpp"
#include "Stl.h"

namespace GLaDOS {
  class Logger;
  class LoggerRegistry : public Singleton<LoggerRegistry> {
  public:
    LoggerRegistry();
    ~LoggerRegistry() override;

    Logger* getLogger(const std::string& name);
    Logger* getDefaultLogger();
    void registerNewLogger(Logger* logger);

  private:
    std::mutex mRegistryMutex;
    UnorderedMap<std::string, Logger*> mLoggers;
    Logger* mDefaultLogger;
  };
}  // namespace GLaDOS

#endif  //GLADOS_LOGGERREGISTRY_H

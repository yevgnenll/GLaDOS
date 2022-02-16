#include "LoggerRegistry.h"
#include "Config.h"
#include "Logger.hpp"
#include "Utility.h"
#include "utils/DestructionManager.h"

namespace GLaDOS {
    LoggerRegistry::LoggerRegistry() {
        setDestructionPhase(1);
        mDefaultLogger = NEW_T(Logger("default"));
        mLoggers.try_emplace("default", mDefaultLogger);
    }

    LoggerRegistry::~LoggerRegistry() {
        deallocValueInMap(mLoggers);
    }

    Logger* LoggerRegistry::getLogger(const std::string& name) {
        // TODO: RWLock, iterator is not atomic
        UnorderedMap<std::string, Logger*>::const_iterator find = mLoggers.find(name);
        return find == mLoggers.end() ? mDefaultLogger : find->second;
    }

    Logger* LoggerRegistry::getDefaultLogger() {
        return mDefaultLogger;
    }

    Logger* LoggerRegistry::makeAndGetLogger(const std::string& name) {
        Logger* logger = NEW_T(Logger(name));
#if DEBUG_BUILD == 1
        logger->setLevel(LogLevel::Trace);
#elif DEBUG_BUILD == 2
        logger->setLevel(LogLevel::Debug);
#else
        logger->setLevel(LogLevel::Error);
#endif

        if (!registerNewLogger(logger)) {
            DELETE_T(logger, Logger);
            return mDefaultLogger;
        }

        return logger;
    }

    bool LoggerRegistry::registerNewLogger(Logger* logger) {
        std::lock_guard<std::mutex> lock(mRegistryMutex);
        std::string name = logger->getName();
        if (mLoggers.find(name) != mLoggers.end()) {
            LOG_ERROR(getDefaultLogger(), "Logger name {0} already exists!", name);
            return false;
        }
        mLoggers.try_emplace(name, logger);
        return true;
    }
}  // namespace GLaDOS
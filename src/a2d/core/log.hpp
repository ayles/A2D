//
// Created by selya on 23.01.2019.
//

#ifndef A2D_LOG_H
#define A2D_LOG_H

#if NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>
#if TARGET_ANDROID
#include <spdlog/sinks/android_sink.h>
#else
#include <spdlog/sinks/stdout_sinks.h>
#endif

#include <memory>

#define LOG_TRACE(...)      SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...)      SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)       SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)       SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)      SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)   SPDLOG_CRITICAL(__VA_ARGS__)

namespace a2d {

class Logger {
private:
    static std::shared_ptr<spdlog::logger> CreateLogger() {
        std::shared_ptr<spdlog::logger> logger;
#if TARGET_ANDROID
        logger = spdlog::android_logger_mt("logger", "a2d_log");
#else
        logger = spdlog::stdout_logger_mt("logger");
#endif
        logger->set_level(spdlog::level::info);
        logger->set_pattern("%+");
#if NDEBUG
        logger->set_level(spdlog::level::level_enum::info);
#else
        logger->set_level(spdlog::level::level_enum::trace);
#endif
        spdlog::set_default_logger(logger);
        return logger;
    }

    static std::shared_ptr<spdlog::logger> GetLogger() {
        static std::shared_ptr<spdlog::logger> logger = CreateLogger();
        return logger;
    }

public:
    template<class... Args>
    static void Trace(const char *fmt, const Args &... args) {
        GetLogger()->trace(fmt, args...);
    }

    template<class... Args>
    static void Debug(const char *fmt, const Args &... args) {
        GetLogger()->debug(fmt, args...);
    }

    template<class... Args>
    static void Info(const char *fmt, const Args &... args) {
        GetLogger()->info(fmt, args...);
    }

    template<class... Args>
    static void Warn(const char *fmt, const Args &... args) {
        GetLogger()->warn(fmt, args...);
    }

    template<class... Args>
    static void Error(const char *fmt, const Args &... args) {
        GetLogger()->error(fmt, args...);
    }

    template<class... Args>
    static void Critical(const char *fmt, const Args &... args) {
        GetLogger()->critical(fmt, args...);
    }
};

}

#endif //A2D_LOG_H

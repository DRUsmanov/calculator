#include "logger/logger.h"

#include <spdlog/sinks/rotating_file_sink.h>

namespace logger
{

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::logInfo(std::string_view msg)
{
    logger_->info(msg);
}

void Logger::logError(std::string_view msg)
{
    logger_->error(msg);
}

Logger::Logger()
{
    size_t max_file_size = 5 * 1024 * 1024;
    size_t max_files = 10;
    logger_ = spdlog::rotating_logger_mt("logger", "logs/log.txt",
                                         max_file_size, max_files);
    logger_->set_level(spdlog::level::debug);
}

} // namespace logger

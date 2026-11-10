#include <spdlog/spdlog.h>
#include <string_view>
#include <memory>

namespace logger {

class Logger {
public:
    static Logger& instance();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    void logInfo(std::string_view msg);
    void logError(std::string_view msg);
private:
    Logger();
    ~Logger() = default;

private:
    std::shared_ptr<spdlog::logger> logger_ = nullptr;

};

} // namespace logger

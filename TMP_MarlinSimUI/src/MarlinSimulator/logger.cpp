#include "logger.h"

#include <functional>
#include <mutex>

namespace logger {

std::function<void(const std::string_view)> logger_callback {};
std::mutex log_write_mutex;

}

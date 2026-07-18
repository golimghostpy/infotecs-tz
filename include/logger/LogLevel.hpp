#pragma once

#include <string>

// уровни важности сообщений (debug < info < error)
enum class LogLevel {
    UNKNOWN = -1,
    DEBUG = 0,
    INFO = 1,
    ERROR = 2
};

// преобразование уровня в строку
std::string log_level_to_string(LogLevel level);
// преобразование строки в уровень
LogLevel log_level_from_string(const std::string& s);

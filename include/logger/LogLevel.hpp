#pragma once

#include <string>

using namespace std;

// уровни важности сообщений (debug < info < error)
enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    ERROR = 2
};

// преобразование уровня в строку
string log_level_to_string(LogLevel level);
// преобразование строки в уровень
LogLevel log_level_from_string(const string& s);

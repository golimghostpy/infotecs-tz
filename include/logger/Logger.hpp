#pragma once

#include "LogLevel.hpp"
#include <mutex>
#include <string>

// основной класс для заипси сообщений в журнал
class Logger {
public:
    Logger();

    // инициализация логгера с заданным файлом и пороговым уровнем
    void init(const std::string& file_path, LogLevel default_level = LogLevel::DEBUG);
    // освобождение ресурсов
    void shutdown();

    // изменение порогового уровня
    void set_level(LogLevel level);
    // получение текущего порогового уровня
    LogLevel get_level() const;

    // запись сообщения
    void log(const std::string& message, LogLevel level);

private:
    bool initialized_ = false;
    std::string file_path_;
    LogLevel level_ = LogLevel::DEBUG;
    mutable std::mutex mutex_;
};

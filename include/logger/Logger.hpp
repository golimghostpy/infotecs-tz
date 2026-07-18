#pragma once

#include "LogLevel.hpp"
#include <mutex>
#include <string>

using namespace std;

// основной класс для записи сообщений в журнал
class Logger {
public:
    Logger();

    // инициализация логгера с заданным файлом и пороговым уровнем
    void init(const string& file_path, LogLevel default_level = LogLevel::DEBUG);
    // освобождение ресурсов
    void shutdown();

    // изменение порогового уровеня
    void set_level(LogLevel level);
    // получение текущего порогового уровеня
    LogLevel get_level() const;

    // запись сообщения
    void log(const string& message, LogLevel level);

private:
    bool initialized_ = false;
    string file_path_;
    LogLevel level_ = LogLevel::DEBUG;
    mutable mutex mutex_;
};

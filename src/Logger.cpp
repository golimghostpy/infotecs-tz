#include "logger/Logger.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

string log_level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

LogLevel log_level_from_string(const string& s) {
    if (s == "DEBUG") return LogLevel::DEBUG;
    if (s == "INFO") return LogLevel::INFO;
    if (s == "ERROR") return LogLevel::ERROR;
    return LogLevel::INFO;
}

Logger::Logger() = default;

void Logger::init(const string& file_path, LogLevel default_level) {
    lock_guard<mutex> lock(mutex_);
    file_path_ = file_path;
    level_ = default_level;
    initialized_ = true;
}

void Logger::shutdown() {
    lock_guard<mutex> lock(mutex_);
    initialized_ = false;
}

void Logger::set_level(LogLevel level) {
    lock_guard<mutex> lock(mutex_);
    level_ = level;
}

LogLevel Logger::get_level() const {
    lock_guard<mutex> lock(mutex_);
    return level_;
}

void Logger::log(const string& message, LogLevel level) {
    lock_guard<mutex> lock(mutex_);

    // проверка инициализации логгера
    if (!initialized_) {
        cerr << "[Logger] Not initialized" << endl;
        return;
    }

    // проверка: уровень сообщения не ниже порога
    if (level < level_) {
        return;
    }

    // получение текущего времени в формате yyyy-mm-dd hh:mm:ss
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    auto tm = *localtime(&time_t);

    // формирование строки: [время] [уровень] сообщение
    ostringstream oss;
    oss << "[" << put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << log_level_to_string(level) << "] "
        << message;

    // открытие файла в режиме дозаписи
    ofstream out(file_path_, ios::app);
    if (!out) {
        cerr << "[Logger] Failed to open log file: " << file_path_ << endl;
        return;
    }

    out << oss.str() << endl;

    // проверка на ошибку записывания
    if (!out) {
        cerr << "[Logger] Failed to write to log file" << endl;
    }
}

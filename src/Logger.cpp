#include "logger/Logger.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

using std::string;
using std::lock_guard;
using std::mutex;
using std::cerr;
using std::endl;
using std::ofstream;
using std::ios;

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
    return LogLevel::UNKNOWN;
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

    // проверка инициалзиации логгера
    if (!initialized_) {
        cerr << "[Логгер] Не инициализирован" << endl;
        return;
    }

    // проверка: уровень сообщения не ниже порога
    if (level < level_) {
        return;
    }

    // получение текущего времени в формате yyyy-mm-dd hh:mm:ss
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);

    // формирование строки: [время] [уровень] сообщение
    ofstream out(file_path_, ios::app);
    if (!out) {
        cerr << "[Логгер] Не удалось открыть файл: " << file_path_ << endl;
        return;
    }

    out << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
        << "[" << log_level_to_string(level) << "] "
        << message << endl;

    // проверка на ошибку записывания
    if (!out) {
        cerr << "[Логгер] Зпись в файл не удалась" << endl;
    }
}

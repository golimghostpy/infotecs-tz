#include "logger.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::stringstream;
using std::streambuf;
using std::cerr;

int passed = 0;
int failed = 0;

// утилита для проверки
void check(bool condition, const string& name) {
    if (condition) {
        passed++;
        cout << "  [OK] " << name << endl;
    } else {
        failed++;
        cout << "  [FAIL] " << name << endl;
    }
}

// проверка фильтрации по уровню
void test_level_filtering() {
    cout << "Тест: фильтрация по уровню" << endl;

    Logger logger;
    logger.init("test_logger.log", LogLevel::ERROR);

    logger.log("debug message", LogLevel::DEBUG);
    logger.log("info message", LogLevel::INFO);
    logger.log("error message", LogLevel::ERROR);
    logger.shutdown();

    ifstream in("test_logger.log");
    string last_line;
    int lines = 0;
    string tmp;
    while (getline(in, tmp)) {
        last_line = tmp;
        lines++;
    }
    in.close();

    check(lines == 1, "записан только ERROR при пороге ERROR");
    check(last_line.find("error message") != string::npos, "записано сообщение error message");
}

// проверка смены уровня лога
void test_set_level() {
    remove("test_logger2.log");

    cout << "Тест: смена уровня лога" << endl;

    Logger logger;
    logger.init("test_logger2.log", LogLevel::ERROR);

    logger.log("info msg", LogLevel::INFO);

    logger.set_level(LogLevel::INFO);
    logger.log("info msg 2", LogLevel::INFO);

    check(logger.get_level() == LogLevel::INFO, "get_level вернул INFO");

    logger.shutdown();

    ifstream in("test_logger2.log");
    int lines = 0;
    string last_line;
    string tmp;
    while (getline(in, tmp)) {
        last_line = tmp;
        lines++;
    }

    check(lines == 1, "записано 1 сообщение после смены уровня логирования");
    check(last_line.find("info msg 2") != string::npos, "записано сообщение info msg 2");
}

// проверка: после set_level сообщения без явного уровня записываются с новым порогом
void test_set_level_default() {
    remove("test_logger2.log");

    cout << "Тест: set_level изменяет уровень по умолчанию" << endl;

    Logger logger;
    logger.init("test_logger2.log", LogLevel::ERROR);

    // уровень DEBUG < ERROR - не запишется
    logger.log("debug before set_level", LogLevel::DEBUG);

    // изменение порога на DEBUG
    logger.set_level(LogLevel::DEBUG);
    check(logger.get_level() == LogLevel::DEBUG, "get_level вернул DEBUG после set_level");

    // запись сообщения с уровнем DEBUG
    logger.log("debug after set_level", LogLevel::DEBUG);

    logger.shutdown();

    ifstream in("test_logger2.log");
    string last_line;
    int lines = 0;
    string tmp;
    while (getline(in, tmp)) {
        last_line = tmp;
        lines++;
    }
    in.close();

    check(lines == 1, "записано 1 сообщение после set_level на DEBUG");
    check(last_line.find("debug after set_level") != string::npos, "записано сообщение debug after set_level");

    remove("test_logger2.log");
}

// проверка записи до инициализации
void test_uninitialized_logger() {
    cout << "Тест: запись до инициализации" << endl;

    Logger logger;

    // перенаправление stderr для проверки ошибки
    stringstream ss;
    streambuf* old = cerr.rdbuf(ss.rdbuf());
    logger.log("test", LogLevel::INFO);
    cerr.rdbuf(old);

    string err = ss.str();
    bool has_error = err.find("Не инициализирован") != string::npos;
    check(has_error, "выведено сообщение об ошибке");
}

// проверка записи в недоступный файл
void test_bad_file_path() {
    cout << "Тест: запись в недоступный файл" << endl;

    Logger logger;
    logger.init("/nonexistent/path/log.txt", LogLevel::DEBUG);

    // перенаправление stderr
    stringstream ss;
    streambuf* old = cerr.rdbuf(ss.rdbuf());
    logger.log("test", LogLevel::INFO);
    cerr.rdbuf(old);

    logger.shutdown();

    string err = ss.str();
    bool has_error = err.find("Не удалось открыть файл") != string::npos;
    check(has_error, "выведено сообщение об ошибке открытия файла");
}

int main() {
    remove("test_logger.log");
    remove("test_logger2.log");

    cout << "=== Запуск тестов ===" << endl;

    test_level_filtering();
    test_set_level();
    test_set_level_default();
    test_uninitialized_logger();
    test_bad_file_path();

    cout << endl;
    cout << "Результат: " << passed << " из " << (passed + failed) << " пройдено" << endl;

    remove("test_logger.log");
    remove("test_logger2.log");

    return failed > 0 ? 1 : 0;
}

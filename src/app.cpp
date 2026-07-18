#include "logger.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <sstream>

using std::string;
using std::thread;
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::getline;
using std::istringstream;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Использование: " << argv[0] << " <файл> <DEBUG|INFO|ERROR>" << endl;
        return 1;
    }

    string file_path = argv[1];
    LogLevel level = log_level_from_string(argv[2]);

    if (level == LogLevel::UNKNOWN) {
        cerr << "Ошибка: неизвестный уровень \"" << argv[2] << "\"" << endl;
        return 1;
    }

    Logger logger;
    logger.init(file_path, level);

    cout << "Логгер запущен. Введите сообщение или 'УРОВЕНЬ сообщение'" << endl;
    cout << "Введите уровень без сообщения, чтобы изменить уровень по умолчанию и порог." << endl;
    cout << "Уровни: DEBUG, INFO, ERROR. Пустая строка для выхода." << endl;

    while (true) {
        cout << "> ";
        string input;
        getline(cin, input);

        if (input.empty()) {
            break;
        }

        istringstream iss(input);
        string first_token;
        iss >> first_token;

        LogLevel msg_level;
        string message;

        // проверка: является ли первая часть сообщения новым уровнем логирования
        LogLevel test_level = log_level_from_string(first_token);
        if (test_level != LogLevel::UNKNOWN) {
            msg_level = test_level;
            getline(iss, message);
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);
            }

            if (message.empty()) {
                // изменение уровня по умолчанию и порога логирования
                logger.set_level(msg_level);
                level = msg_level;
                cout << "Уровень по умолчанию и пороговый уровень изменены на: " << log_level_to_string(msg_level) << endl;
                continue;
            }
        } else {
            msg_level = level;
            message = input;
        }

        thread([&logger, message, msg_level]() {
            logger.log(message, msg_level);
        }).detach();
    }

    logger.shutdown();
    cout << "Логгер остановлен." << endl;

    return 0;
}

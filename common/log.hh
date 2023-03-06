#include <iostream>
#include <string>

using namespace std;

enum LogLevel {
    ERROR,
    WARNING,
    SUCCESS,
    INFO
};

class Logger {
public:
    Logger(LogLevel level)
        : m_level(level)
    {
    }

    template <typename T>
    Logger& operator<<(const T& value)
    {
        if (m_level == ERROR) {
            cout << "\033[31m"; // set color to red
        } else if (m_level == WARNING) {
            cout << "\033[33m"; // set color to yellow
        } else if (m_level == SUCCESS) {
            cout << "\033[32m"; // set color to green
        } else {
            cout << "\033[0m"; // reset color to default
        }

        cout << value;
        return *this;
    }

private:
    LogLevel m_level;
};
#pragma once

#include <fstream>

class Logger {
public:
    static Logger & getInstance() {
        static Logger instance;
        return instance;
    }

private:
    Logger();

public:
    Logger(Logger const &) = delete;
    void operator =(Logger const &) = delete;
    std::ofstream mainout;
    std::ofstream inputout;
    std::ofstream outputout;
};

#pragma once

#include <string>
#include <fstream>

class Logger {
public:
    Logger(const std::string & path);
    std::ofstream out;
};

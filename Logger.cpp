#include <ctime>
#include "Logger.h"

Logger::Logger(const std::string & fileName) {
    const std::string logDir = "C:/Users/eliha/Documents/dev/sascha/" ;
    const auto now = std::time(nullptr) ;
    char cstr[256] {};
    const std::string timeStr = std::strftime( cstr, sizeof(cstr), "%Y%m%d_%H%M%S", std::localtime(&now) ) ? cstr : "" ;
    const std::string logPath = logDir + timeStr + '_' + fileName;
    out = std::ofstream(logPath);
}

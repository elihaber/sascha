#include <ctime>
#include <string>
#include <fstream>
#include "Logger.h"

namespace Sascha {

const std::string LOG_FOLDER = "C:/Users/eliha/Documents/dev/sascha/logs/";
const std::string MAIN_LOG_FILE_NAME = "SaschaMainLog.txt";
const std::string INPUT_LOG_FILE_NAME = "SaschaInputLog.txt";
const std::string OUTPUT_LOG_FILE_NAME = "SaschaOutputLog.txt";
const std::string RECURSION_LOG_FILE_NAME = "SaschaRecursionLog.txt";

Logger::Logger() {
    const std::string logDir = LOG_FOLDER;
    const auto now = std::time(nullptr) ;
    char cstr[256] {};
    const std::string timeStr = std::strftime( cstr, sizeof(cstr), "%Y%m%d_%H%M%S", std::localtime(&now) ) ? cstr : "" ;
    const std::string mainlogPath = logDir + timeStr + '_' + MAIN_LOG_FILE_NAME;
    const std::string inputlogPath = logDir + timeStr + '_' + INPUT_LOG_FILE_NAME;
    const std::string outputlogPath = logDir + timeStr + '_' + OUTPUT_LOG_FILE_NAME;
    const std::string recursionlogPath = logDir + timeStr + '_' + RECURSION_LOG_FILE_NAME;
    mainout = std::ofstream(mainlogPath);
    inputout = std::ofstream(inputlogPath);
    outputout = std::ofstream(outputlogPath);
    recursionout = std::ofstream(recursionlogPath);
}

}

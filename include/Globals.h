#pragma once

#include "Logger.h"

namespace Sascha {

#define MAINLOG(x) Sascha::Logger::Logger::getInstance().mainout << x << std::endl; Sascha::Logger::Logger::getInstance().mainout.flush();
#define MAINLOG_NNL(x) Sascha::Logger::Logger::getInstance().mainout << x; Sascha::Logger::Logger::getInstance().mainout.flush();
#define INPUTLOG(x) Sascha::Logger::Logger::getInstance().inputout << x << std::endl; Sascha::Logger::Logger::getInstance().inputout.flush();
#define INPUTLOG_NNL(x) Sascha::Logger::Logger::getInstance().inputout << x; Sascha::Logger::Logger::getInstance().inputout.flush();
#define OUTPUTLOG(x) Sascha::Logger::Logger::getInstance().outputout << x << std::endl; Sascha::Logger::Logger::getInstance().outputout.flush();
#define OUTPUTLOG_NNL(x) Sascha::Logger::Logger::getInstance().outputout << x; Sascha::Logger::Logger::getInstance().outputout.flush();
#define RECURSIONLOG(x) Sascha::Logger::Logger::getInstance().recursionout << x << std::endl; Sascha::Logger::Logger::getInstance().recursionout.flush();
#define RECURSIONLOG_NNL(x) Sascha::Logger::Logger::getInstance().recursionout << x; Sascha::Logger::Logger::getInstance().recursionout.flush();

}

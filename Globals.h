#pragma once

#include "Logger.h"

#define MAINLOG(x) Logger::getInstance().mainout << x << std::endl; Logger::getInstance().mainout.flush();
#define MAINLOG_NNL(x) Logger::getInstance().mainout << x; Logger::getInstance().mainout.flush();
#define INPUTLOG(x) Logger::getInstance().inputout << x << std::endl; Logger::getInstance().inputout.flush();
#define INPUTLOG_NNL(x) Logger::getInstance().inputout << x; Logger::getInstance().inputout.flush();
#define OUTPUTLOG(x) Logger::getInstance().outputout << x << std::endl; Logger::getInstance().outputout.flush();
#define OUTPUTLOG_NNL(x) Logger::getInstance().outputout << x; Logger::getInstance().outputout.flush();

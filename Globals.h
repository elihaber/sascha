#pragma once

#include "Logger.h"

#define MAINLOG(x) MainLog.out << x << std::endl; MainLog.out.flush();
#define MAINLOG_NNL(x) MainLog.out << x; MainLog.out.flush();
#define INPUTLOG(x) InputLog.out << x << std::endl; InputLog.out.flush();
#define INPUTLOG_NNL(x) InputLog.out << x; InputLog.out.flush();
#define OUTPUTLOG(x) OutputLog.out << x << std::endl; OutputLog.out.flush();
#define OUTPUTLOG_NNL(x) OutputLog.out << x; OutputLog.out.flush();

static Logger MainLog("SaschaMainLog.txt");
static Logger InputLog("SaschaInputLog.txt");
static Logger OutputLog("SaschaOutputLog.txt");

#include <sstream>
#include "IO/Messages/CopyProtectionMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool CopyProtectionMessage::formatMessage(std::stringstream & line) {
    line << "copyprotection ";

    if (checkingFlag()) {
        line << "checking";
    }
    else if (okFlag()) {
        line << "ok";
    }
    else if (errorFlag()) {
        line << "error";
    }
    else {
        MAINLOG("COPYPROTECTION message without any flag")
        return false;
    }
    return true;
}

}
}
}

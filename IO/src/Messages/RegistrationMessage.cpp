#include <sstream>
#include "IO/Messages/RegistrationMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool RegistrationMessage::formatMessage(std::stringstream & line) {
    line << "registration ";

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
        MAINLOG("REGISTRATION message without any flag")
        return false;
    }
    return true;
}

}
}
}

#include <sstream>
#include "IO/Messages/IdMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool IdMessage::formatMessage(std::stringstream & line) {
    line << "id ";
    if (nameFlag()) {
        line << "name ";
        line << name();
    }
    else if (authorFlag()) {
        line << "author ";
        line << author();
    }
    else {
        MAINLOG("ID message without nameFlag or authorFlag")
        return false;
    }
    return true;
}

}
}
}

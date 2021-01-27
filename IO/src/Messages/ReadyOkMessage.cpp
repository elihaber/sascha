#include <sstream>
#include "IO/Messages/ReadyOkMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool ReadyOkMessage::formatMessage(std::stringstream & line) {
    line << "readyok";
    return true;
}

}
}
}

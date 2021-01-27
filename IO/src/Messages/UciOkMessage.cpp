#include <sstream>
#include "IO/Messages/UciOkMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool UciOkMessage::formatMessage(std::stringstream & line) {
    line << "uciok";
    return true;
}

}
}
}

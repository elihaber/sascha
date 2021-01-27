#include <iterator>
#include <sstream>
#include "IO/Messages/DebugMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool DebugMessage::parseInputLine(const std::string & line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    setDebugFlag((tokens.size() > 1 && tokens[1].compare("on") == 0));
    return true;    
}

}
}
}

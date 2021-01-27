#include <iterator>
#include <sstream>
#include "IO/Messages/SetOptionMessage.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool SetOptionMessage::parseInputLine(const std::string & line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    if (tokens.size() < 5 || tokens[1].compare("name") != 0) {
        return false;
    }

    int valPos = -1;
    for (int i = 2; i < tokens.size(); ++i) {
        if (tokens[i].compare("value") == 0) {
            valPos = i;
            break;
        }
    }

    if (valPos == -1) {
        return false;
    }

    setOptionNameValue(_concatTokens(tokens, 2, valPos), _concatTokens(tokens, valPos + 1, tokens.size()));

    return true;    
}

}
}
}

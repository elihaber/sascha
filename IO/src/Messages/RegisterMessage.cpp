#include <iterator>
#include <sstream>
#include "IO/Messages/RegisterMessage.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool RegisterMessage::parseInputLine(const std::string & line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    if (tokens.size() < 2) {
        return false;
    }

    if (tokens[1].compare("later") == 0) {
        setRegisterLaterFlag(true);
        return true;
    }

    if (tokens.size() < 3) {
        return false;
    }

    if (tokens[1].compare("name") == 0) {
        setRegisterNameFlag(true);
        setRegisterName(_concatTokens(tokens, 2, tokens.size()));
        return true;
    }

    if (tokens[1].compare("code") == 0) {
        setRegisterCodeFlag(true);
        setRegisterCode(_concatTokens(tokens, 2, tokens.size()));
        return true;
    }

    return false;    
}

}
}
}

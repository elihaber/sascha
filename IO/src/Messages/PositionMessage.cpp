#include <iterator>
#include <sstream>
#include "IO/Messages/PositionMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool PositionMessage::parseInputLine(const std::string & line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    if (tokens.size() < 2) {
        return false;
    }

    int lastUsedPos = -1;

    if (tokens[1].compare("startpos") == 0) {
        setStartposFlag(true);
        lastUsedPos = 1;
    }
    else if (tokens[1].compare("fen") == 0) {
        setFenFlag(true);
    }
    else {
        MAINLOG("Returning false because no startpos and no fenflag")
        return false;
    }

    int movesPos = -1;
    for (int i = 2; i < tokens.size(); ++i) {
        if (tokens[i].compare("moves") == 0) {
            movesPos = i;
            break;
        }
    }

    setMovesFlag(movesPos != -1);

    if (fenFlag()) {
        if (movesPos == 2) {
            MAINLOG("Returning false because fenflag and movesPos == 2")
            return false;
        }
        setFen(_concatTokens(tokens, 2, movesPos));
    }
    
    if (movesFlag()) {
        for (int i = movesPos + 1; i < tokens.size(); ++i) {
            addMove(tokens[i]);
        }
    }

    return true;    
}

}
}
}

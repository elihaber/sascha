#include <sstream>
#include "IO/Messages/BestMoveMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool BestMoveMessage::formatMessage(std::stringstream & line) {
    line << "bestmove ";
    if (move1().empty()) {
        MAINLOG("BESTMOVE message with blank move1")
        return false;
    }
    line << move1();
    if (ponderFlag()) {
        line << " ponder ";
        if (move2().empty()) {
            MAINLOG("BESTMOVE message with blank move2")
            return false;
        }
        line << move2();
    }

    return true;
}

}
}
}

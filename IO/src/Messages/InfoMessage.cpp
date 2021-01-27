#include <sstream>
#include "IO/Messages/InfoMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool InfoMessage::formatMessage(std::stringstream & line) {
    line << "info";
    if (depthFlag()) {
        line << " depth";
        line << " " << depth();
    }
    if (selDepthFlag()) {
        if (!depthFlag()) {
            MAINLOG("INFO message with selDepth flag but no depth flag")
            return false;
        }
        line << " seldepth";
        line << " " << selDepth();
    }
    if (timeFlag()) {
        if (!pvFlag()) {
            MAINLOG("INFO message with time flag but no pv flag")
            return false;
        }
        line << " time";
        line << " " << time();
    }
    if (nodesFlag()) {
        line << " nodes";
        line << " " << nodes();
    }
    if (pvFlag()) {
        line << " pv";
        if (moves().empty()) {
            MAINLOG("INFO message with pv flag but no moves")
            return false;
        }
        for (int i = 0; i < moves().size(); ++i) {
            line << " " << moves()[i];
        }
    }
    if (multiPvFlag()) {
        line << " multipv";
        line << " " << multiPv();
    }
    if (scoreFlag()) {
        line << " score";
        if (centipawnFlag()) {
            line << " cp";
            line << " " << centipawn();
        }
        else if (mateFlag()) {
            line << " mate";
            line << " " << mate();
        }
        else if (lowerBoundFlag()) {
            line << " lowerbound";
        }
        else if (upperBoundFlag()) {
            line << " upperbound";
        }
        else {
            MAINLOG("INFO message with score flag but no score type flag")
            return false;
        }
    }
    if (currMoveFlag()) {
        if (!currMove().empty()) {
            MAINLOG("INFO message with currMove flag but no currMove")
            return false;
        }
        line << " currmove " << currMove();
    }
    if (currMoveNumberFlag()) {
        line << " currmovenumber " << currMoveNumber();
    }
    if (hashFullFlag()) {
        line << " hashfull " << hashFull();
    }
    if (nodesPerSecondFlag()) {
        line << " nps " << nodesPerSecond();
    }
    if (tableBaseHitsFlag()) {
        line << " tbhits " << tableBaseHits();
    }
    if (cpuLoadFlag()) {
        line << " cpuload " << cpuLoad();
    }
    if (displayStringFlag()) {
        line << " string " << displayString();
    }
    if (refutationFlag()) {
        line << " refutation";
        if (moves().empty()) {
            MAINLOG("INFO message with refutation flag but no moves")
            return false;
        }
        for (int i = 0; i < moves().size(); ++i) {
            line << " " << moves()[i];
        }
    }
    if (currLineFlag()) {
        line << " currline";
        if (cpuNumberFlag()) {
            line << " " << cpuNumber();
        }
        if (moves().empty()) {
            MAINLOG("INFO message with currLine flag but no moves")
            return false;
        }
        for (int i = 0; i < moves().size(); ++i) {
            line << " " << moves()[i];
        }
    }

    return true;
}

}
}
}

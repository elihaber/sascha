#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <thread>
#include <memory>
#include <future>
#include "IO/OutputHandler.h"
#include "IO/Messages/Message.h"
#include "IO/MessageQueue.h"
#include "Globals.h"

using namespace std::chrono_literals;

namespace Sascha {
namespace IO {

using namespace Messages;

void OutputHandler::start(std::future<void> futureObj) {
    std::string line;

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        auto message = _outgoingMessages.readNextMessage();
        if (message->messageType() != MessageType::NONE) {
            std::stringstream line;
            if (_formatMessage(message, line)) {
                OUTPUTLOG("out << " << line.str())
                std::cout << line.str() << std::endl;
            }
            else {
                MAINLOG("Failed to format message for output")
            }
        }

        std::this_thread::sleep_for(100ms);
    }
}

void OutputHandler::end() {
    _endFlag = true;
}

bool OutputHandler::_formatMessage(std::shared_ptr<Messages::Message> message, std::stringstream & line) {
    if (message->messageType() == MessageType::ID) {
        std::shared_ptr<IdMessage> realMessage = std::static_pointer_cast<IdMessage>(message);
        line << "id ";
        if (realMessage->nameFlag()) {
            line << "name ";
            line << realMessage->name();
        }
        else if (realMessage->authorFlag()) {
            line << "author ";
            line << realMessage->author();
        }
        else {
            MAINLOG("ID message without nameFlag or authorFlag")
            return false;
        }
        return true;
    }

    if (message->messageType() == MessageType::UCIOK) {
        line << "uciok";
        return true;
    }

    if (message->messageType() == MessageType::READYOK) {
        line << "readyok";
        return true;
    }

    if (message->messageType() == MessageType::BESTMOVE) {
        std::shared_ptr<BestMoveMessage> realMessage = std::static_pointer_cast<BestMoveMessage>(message);
        line << "bestmove ";
        if (realMessage->move1().empty()) {
            MAINLOG("BESTMOVE message with blank move1")
            return false;
        }
        line << realMessage->move1();
        if (realMessage->ponderFlag()) {
            line << " ponder ";
            if (realMessage->move2().empty()) {
                MAINLOG("BESTMOVE message with blank move2")
                return false;
            }
            line << realMessage->move2();
        }

        return true;
    }

    if (message->messageType() == MessageType::COPYPROTECTION) {
        std::shared_ptr<CopyProtectionMessage> realMessage = std::static_pointer_cast<CopyProtectionMessage>(message);
        line << "copyprotection ";

        if (realMessage->checkingFlag()) {
            line << "checking";
        }
        else if (realMessage->okFlag()) {
            line << "ok";
        }
        else if (realMessage->errorFlag()) {
            line << "error";
        }
        else {
            MAINLOG("COPYPROTECTION message without any flag")
            return false;
        }
        return true;
    }

    if (message->messageType() == MessageType::REGISTRATION) {
        std::shared_ptr<RegistrationMessage> realMessage = std::static_pointer_cast<RegistrationMessage>(message);
        line << "registration ";

        if (realMessage->checkingFlag()) {
            line << "checking";
        }
        else if (realMessage->okFlag()) {
            line << "ok";
        }
        else if (realMessage->errorFlag()) {
            line << "error";
        }
        else {
            MAINLOG("REGISTRATION message without any flag")
            return false;
        }
        return true;
    }

    if (message->messageType() == MessageType::INFO) {
        std::shared_ptr<InfoMessage> realMessage = std::static_pointer_cast<InfoMessage>(message);
        line << "info";
        if (realMessage->depthFlag()) {
            line << " depth";
            line << " " << realMessage->depth();
        }
        if (realMessage->selDepthFlag()) {
            if (!realMessage->depthFlag()) {
                MAINLOG("INFO message with selDepth flag but no depth flag")
                return false;
            }
            line << " seldepth";
            line << " " << realMessage->selDepth();
        }
        if (realMessage->timeFlag()) {
            if (!realMessage->pvFlag()) {
                MAINLOG("INFO message with time flag but no pv flag")
                return false;
            }
            line << " time";
            line << " " << realMessage->time();
        }
        if (realMessage->nodesFlag()) {
            line << " nodes";
            line << " " << realMessage->nodes();
        }
        if (realMessage->pvFlag()) {
            line << " pv";
            auto moves = realMessage->moves();
            if (moves.empty()) {
                MAINLOG("INFO message with pv flag but no moves")
                return false;
            }
            for (int i = 0; i < moves.size(); ++i) {
                line << " " << moves[i];
            }
        }
        if (realMessage->multiPvFlag()) {
            line << " multipv";
            line << " " << realMessage->multiPv();
        }
        if (realMessage->scoreFlag()) {
            line << " score";
            if (realMessage->centipawnFlag()) {
                line << " cp";
                line << " " << realMessage->centipawn();
            }
            else if (realMessage->mateFlag()) {
                line << " mate";
                line << " " << realMessage->mate();
            }
            else if (realMessage->lowerBoundFlag()) {
                line << " lowerbound";
            }
            else if (realMessage->upperBoundFlag()) {
                line << " upperbound";
            }
            else {
                MAINLOG("INFO message with score flag but no score type flag")
                return false;
            }
        }
        if (realMessage->currMoveFlag()) {
            if (!realMessage->currMove().empty()) {
                MAINLOG("INFO message with currMove flag but no currMove")
                return false;
            }
            line << " currmove " << realMessage->currMove();
        }
        if (realMessage->currMoveNumberFlag()) {
            line << " currmovenumber " << realMessage->currMoveNumber();
        }
        if (realMessage->hashFullFlag()) {
            line << " hashfull " << realMessage->hashFull();
        }
        if (realMessage->nodesPerSecondFlag()) {
            line << " nps " << realMessage->nodesPerSecond();
        }
        if (realMessage->tableBaseHitsFlag()) {
            line << " tbhits " << realMessage->tableBaseHits();
        }
        if (realMessage->cpuLoadFlag()) {
            line << " cpuload " << realMessage->cpuLoad();
        }
        if (realMessage->displayStringFlag()) {
            line << " string " << realMessage->displayString();
        }
        if (realMessage->refutationFlag()) {
            line << " refutation";
            auto moves = realMessage->moves();
            if (moves.empty()) {
                MAINLOG("INFO message with refutation flag but no moves")
                return false;
            }
            for (int i = 0; i < moves.size(); ++i) {
                line << " " << moves[i];
            }
        }
        if (realMessage->currLineFlag()) {
            line << " currline";
            if (realMessage->cpuNumberFlag()) {
                line << " " << realMessage->cpuNumber();
            }
            auto moves = realMessage->moves();
            if (moves.empty()) {
                MAINLOG("INFO message with currLine flag but no moves")
                return false;
            }
            for (int i = 0; i < moves.size(); ++i) {
                line << " " << moves[i];
            }
        }

        return true;
    }
    if (message->messageType() == MessageType::OPTION) {
        std::shared_ptr<OptionMessage> realMessage = std::static_pointer_cast<OptionMessage>(message);
        line << "option";

        if (realMessage->name().empty()) {
            MAINLOG("OPTION message with no name")
            return false;
        }
        line << " name " << realMessage->name();

        if (realMessage->optionType().empty()) {
            MAINLOG("OPTION message with no optioneType")
            return false;
        }
        line << " type " << realMessage->optionType();

        if (realMessage->defaultFlag()) {
            if (realMessage->defaultVal().empty()) {
                MAINLOG("OPTION message with default flag but no default")
                return false;
            }
            line << " default " << realMessage->defaultVal();
        }

        if (realMessage->minFlag()) {
            line << " min " << realMessage->min();
        }

        if (realMessage->maxFlag()) {
            line << " max " << realMessage->max();
        }

        if (realMessage->varFlag()) {
            auto vars = realMessage->vars();
            if (vars.empty()) {
                MAINLOG("OPTION message with var flag but no vars")
                return false;
            }
            line << " var";
            for (int i = 0; i < vars.size(); ++i) {
                line << " " << vars[i];
            }
        }

        return true;
    }


    return true;
}

}
}

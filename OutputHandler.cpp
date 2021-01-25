#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <thread>
#include <memory>
#include <future>
#include "OutputHandler.h"
#include "Message.h"
#include "MessageQueue.h"
#include "log.h"

using namespace std::chrono_literals;

void OutputHandler::start(std::future<void> futureObj) {
    std::string line;

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        log::out << "Output thread loop " << this << std::endl; log::flush();

        auto message = _outgoingMessages.readNextMessage();
        if (message->messageType() != MessageType::NONE) {
            std::stringstream line;
            if (_formatMessage(message, line)) {
                log::out << "out << " << line.str() << std::endl; log::flush();
                std::cout << line.str() << std::endl;
            }
            else {
                log::out << "Failed to format message for output" << std::endl; log::flush();
            }
        }

        std::this_thread::sleep_for(100ms);
    }
}

void OutputHandler::end() {
           log::out << "Output thread end call " << this << std::endl; log::flush();
    _endFlag = true;
}

bool OutputHandler::_formatMessage(std::shared_ptr<Message> message, std::stringstream & line) {
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
            log::out << "ID message without nameFlag or authorFlag" << std::endl; log::flush();
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
            log::out << "BESTMOVE message with blank move1" << std::endl; log::flush();
            return false;
        }
        line << realMessage->move1();
        if (realMessage->ponderFlag()) {
            line << " ponder ";
            if (realMessage->move2().empty()) {
                log::out << "BESTMOVE message with blank move2" << std::endl; log::flush();
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
            log::out << "COPYPROTECTION message without any flag" << std::endl; log::flush();
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
            log::out << "REGISTRATION message without any flag" << std::endl; log::flush();
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
                log::out << "INFO message with selDepth flag but no depth flag" << std::endl; log::flush();
                return false;
            }
            line << " seldepth";
            line << " " << realMessage->selDepth();
        }
        if (realMessage->timeFlag()) {
            if (!realMessage->pvFlag()) {
                log::out << "INFO message with time flag but no pv flag" << std::endl; log::flush();
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
                log::out << "INFO message with pv flag but no moves" << std::endl; log::flush();
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
                log::out << "INFO message with score flag but no score type flag" << std::endl; log::flush();
                return false;
            }
        }
        if (realMessage->currMoveFlag()) {
            if (!realMessage->currMove().empty()) {
                log::out << "INFO message with currMove flag but no currMove" << std::endl; log::flush();
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
                log::out << "INFO message with refutation flag but no moves" << std::endl; log::flush();
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
                log::out << "INFO message with currLine flag but no moves" << std::endl; log::flush();
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
            log::out << "OPTION message with no name" << std::endl; log::flush();
            return false;
        }
        line << " name " << realMessage->name();

        if (realMessage->optionType().empty()) {
            log::out << "OPTION message with no optioneType" << std::endl; log::flush();
            return false;
        }
        line << " type " << realMessage->optionType();

        if (realMessage->defaultFlag()) {
            if (realMessage->defaultVal().empty()) {
                log::out << "OPTION message with default flag but no default" << std::endl; log::flush();
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
                log::out << "OPTION message with var flag but no vars" << std::endl; log::flush();
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

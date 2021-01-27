#include <sstream>
#include "IO/Messages/AllMessages.h"

namespace Sascha {
namespace IO {
namespace Messages {

std::shared_ptr<Message> Message::createMessage(MessageType messageType) {
    if (messageType == MessageType::UCI) {
        return std::make_shared<UciMessage>();
    }
    if (messageType == MessageType::DEBUG) {
        return std::make_shared<DebugMessage>();
    }
    if (messageType == MessageType::ISREADY) {
        return std::make_shared<IsReadyMessage>();
    }
    if (messageType == MessageType::SETOPTION) {
        return std::make_shared<SetOptionMessage>();
    }
    if (messageType == MessageType::REGISTER) {
        return std::make_shared<RegisterMessage>();
    }
    if (messageType == MessageType::UCINEWGAME) {
        return std::make_shared<UciNewGameMessage>();
    }
    if (messageType == MessageType::POSITION) {
        return std::make_shared<PositionMessage>();
    }
    if (messageType == MessageType::GO) {
        return std::make_shared<GoMessage>();
    }
    if (messageType == MessageType::STOP) {
        return std::make_shared<StopMessage>();
    }
    if (messageType == MessageType::PONDERHIT) {
        return std::make_shared<PonderHitMessage>();
    }
    if (messageType == MessageType::QUIT) {
        return std::make_shared<QuitMessage>();
    }
    if (messageType == MessageType::ID) {
        return std::make_shared<IdMessage>();
    }
    if (messageType == MessageType::UCIOK) {
        return std::make_shared<UciOkMessage>();
    }
    if (messageType == MessageType::READYOK) {
        return std::make_shared<ReadyOkMessage>();
    }
    if (messageType == MessageType::BESTMOVE) {
        return std::make_shared<BestMoveMessage>();
    }
    if (messageType == MessageType::COPYPROTECTION) {
        return std::make_shared<CopyProtectionMessage>();
    }
    if (messageType == MessageType::REGISTRATION) {
        return std::make_shared<RegistrationMessage>();
    }
    if (messageType == MessageType::INFO) {
        return std::make_shared<InfoMessage>();
    }
    if (messageType == MessageType::OPTION) {
        return std::make_shared<OptionMessage>();
    }
    if (messageType == MessageType::NONE) {
        return std::make_shared<DummyMessage>();
    }
    throw std::runtime_error("Incorrect type of message to create");

}

std::string Message::_concatTokens(const std::vector<std::string> tokens, int start, int end) {
    std::stringstream sstr;
    for(size_t i = start; i < end; ++i)  {
        if(i != start) {
            sstr << " ";
        }
        sstr << tokens[i];
    }

    return sstr.str();        
}

}
}
}

#pragma once

#include <utility>
#include <string>
#include <vector>
#include <memory>

namespace Sascha {
namespace IO {
namespace Messages {

enum class MessageType {
    NONE,

    // Input
    UCI,
    DEBUG,
    ISREADY,
    SETOPTION,
    REGISTER,
    UCINEWGAME,
    POSITION,
    GO,
    STOP,
    PONDERHIT,
    QUIT,

    // Outupt
    ID,
    UCIOK,
    READYOK,
    BESTMOVE,
    COPYPROTECTION,
    REGISTRATION,
    INFO,
    OPTION
};

class Message {
public:
    Message(MessageType messageType) : _messageType(messageType) { }
    MessageType messageType() const { return _messageType; }
    void setMessageType(MessageType val) { _messageType = val; }
    virtual bool parseInputLine(const std::string & line) = 0;
    virtual bool formatMessage(std::stringstream & line) = 0;

    static std::shared_ptr<Message> createMessage(MessageType messageType);

protected:
    std::string _concatTokens(const std::vector<std::string> tokens, int start, int end);

private:
    MessageType _messageType;
};

}
}
}

#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class UciNewGameMessage : public Message {
public:
    UciNewGameMessage() : Message(MessageType::UCINEWGAME) { }
    virtual bool parseInputLine(const std::string & line) override { return true; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class PonderHitMessage : public Message {
public:
    PonderHitMessage() : Message(MessageType::PONDERHIT) { }
    virtual bool parseInputLine(const std::string & line) override { return true; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

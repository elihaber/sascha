#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class IsReadyMessage : public Message {
public:
    IsReadyMessage() : Message(MessageType::ISREADY) { }
    virtual bool parseInputLine(const std::string & line) override { return true; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

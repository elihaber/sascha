#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class DummyMessage : public Message {
public:
    DummyMessage() : Message(MessageType::NONE) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

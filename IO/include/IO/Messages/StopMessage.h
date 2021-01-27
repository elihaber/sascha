#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class StopMessage : public Message {
public:
    StopMessage() : Message(MessageType::STOP) { }
    virtual bool parseInputLine(const std::string & line) override { return true; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

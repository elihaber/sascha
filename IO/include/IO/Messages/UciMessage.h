#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class UciMessage : public Message {
public:
    UciMessage() : Message(MessageType::UCI) { }
    virtual bool parseInputLine(const std::string & line) override { return true; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

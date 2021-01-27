#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class ReadyOkMessage : public Message {
public:
    ReadyOkMessage() : Message(MessageType::READYOK) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;
};

}
}
}

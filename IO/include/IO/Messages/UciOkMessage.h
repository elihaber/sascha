#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class UciOkMessage : public Message {
public:
    UciOkMessage() : Message(MessageType::UCIOK) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;
};

}
}
}

#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class QuitMessage : public Message {
public:
    QuitMessage() : Message(MessageType::QUIT) { }
    virtual bool parseInputLine(const std::string & line) override { return true; }
    virtual bool formatMessage(std::stringstream & line) override { return false; }
};

}
}
}

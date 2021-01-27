#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class DebugMessage : public Message {
public:
    DebugMessage() : Message(MessageType::DEBUG), _debugFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override;
    virtual bool formatMessage(std::stringstream & line) override { return false; }
    void setDebugFlag(bool val) { _debugFlag = val; }
    bool debugFlag() const { return _debugFlag; }
private:
    bool _debugFlag;
};

}
}
}

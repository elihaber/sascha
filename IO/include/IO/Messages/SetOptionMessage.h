#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class SetOptionMessage : public Message {
public:
    SetOptionMessage() : Message(MessageType::SETOPTION) { }
    virtual bool parseInputLine(const std::string & line) override;
    virtual bool formatMessage(std::stringstream & line) override { return false; }
    void setOptionNameValue(const std::string & name, const std::string & val) { _optionNameValue = std::make_pair(name, val); }
private:
    std::pair<std::string, std::string> _optionNameValue;
};

}
}
}

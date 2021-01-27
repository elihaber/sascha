#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class CopyProtectionMessage : public Message {
public:
    CopyProtectionMessage() : Message(MessageType::COPYPROTECTION), _checkingFlag(false), _okFlag(false), _errorFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;
    bool checkingFlag() const { return _checkingFlag; }
    bool okFlag() const { return _okFlag; }
    bool errorFlag() const { return _errorFlag; }
    void setCheckingFlag(bool val) { _checkingFlag = val; }
    void setOkFlag(bool val) { _okFlag = val; }
    void setErrorFlag(bool val) { _errorFlag = val; }
private:
    bool _checkingFlag;
    bool _okFlag;
    bool _errorFlag;
};

}
}
}

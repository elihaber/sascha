#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class RegisterMessage : public Message {
public:
    RegisterMessage() : Message(MessageType::REGISTER), _registerLaterFlag(false), _registerNameFlag(false), _registerCodeFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override;
    virtual bool formatMessage(std::stringstream & line) override { return false; }
    void setRegisterLaterFlag(bool val) { _registerLaterFlag = val; }
    void setRegisterNameFlag(bool val) { _registerNameFlag = val; }
    void setRegisterCodeFlag(bool val) { _registerCodeFlag = val; }
    void setRegisterName(const std::string & val) { _registerName = val; }
    void setRegisterCode(const std::string & val) { _registerCode = val; }
    bool registerLaterFlag() const { return _registerLaterFlag; }
    bool registerNameFlag() const { return _registerNameFlag; }
    bool registerCodeFlag() const { return _registerCodeFlag; }
    std::string registerName() const { return _registerName; }
    std::string registerCode() const { return _registerCode; }
private:
    bool _registerLaterFlag;
    bool _registerNameFlag;
    bool _registerCodeFlag;
    std::string _registerName;
    std::string _registerCode;
};

}
}
}

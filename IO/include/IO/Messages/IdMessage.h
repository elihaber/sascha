#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class IdMessage : public Message {
public:
    IdMessage() : Message(MessageType::ID), _nameFlag(false), _authorFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;
    bool nameFlag() const { return _nameFlag; }
    bool authorFlag() const { return _authorFlag; }
    std::string name() const { return _name; }
    std::string author() const { return _author; }
    void setNameFlag(bool val) { _nameFlag = val; }
    void setAuthorFlag(bool val) { _authorFlag = val; }
    void setName(const std::string & val) { _name = val; }
    void setAuthor(const std::string & val) { _author = val; }
private:
    bool _nameFlag;
    bool _authorFlag;
    std::string _name;
    std::string _author;
};

}
}
}

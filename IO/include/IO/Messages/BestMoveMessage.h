#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class BestMoveMessage : public Message {
public:
    BestMoveMessage() : Message(MessageType::BESTMOVE), _ponderFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;

    bool ponderFlag() const { return _ponderFlag; }
    std::string move1() const { return _move1; }
    std::string move2() const { return _move2; }
    void setPonderFlag(bool val) { _ponderFlag = val; }
    void setMove1(const std::string & val) { _move1 = val; }
    void setMove2(const std::string & val) { _move2 = val; }
private:
    bool _ponderFlag;
    std::string _move1;
    std::string _move2;
};

}
}
}

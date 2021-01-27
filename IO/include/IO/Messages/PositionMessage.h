#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class PositionMessage : public Message {
public:
    PositionMessage() : Message(MessageType::POSITION), _startposFlag(false), _fenFlag(false), _movesFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override;
    virtual bool formatMessage(std::stringstream & line) override { return false; }
    void setStartposFlag(bool val) { _startposFlag = val; }
    void setFenFlag(bool val) { _fenFlag = val; }
    void setMovesFlag(bool val) { _movesFlag = val; }
    void setFen(const std::string & val) { _fen = val; }
    void addMove(const std::string & val) { _moves.push_back(val); }
    bool startposFlag() const { return _startposFlag; }
    bool fenFlag() const { return _fenFlag; }
    bool movesFlag() const { return _movesFlag; }
    std::string fen() const { return _fen; }
    std::vector<std::string> moves() const { return _moves; }
private:
    bool _startposFlag;
    bool _fenFlag;
    bool _movesFlag;
    std::string _fen;
    std::vector<std::string> _moves;
};

}
}
}

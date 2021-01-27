#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class GoMessage : public Message {
public:
    GoMessage() : Message(MessageType::GO), _searchmovesFlag(false), _ponderFlag(false), _wtimeFlag(false), _btimeFlag(false),
                  _wincFlag(false), _bincFlag(false), _movestogoFlag(false), _depthFlag(false), _nodesFlag(false), _mateFlag(false),
                  _movetimeFlag(false), _infiniteFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override;
    virtual bool formatMessage(std::stringstream & line) override { return false; }
    void setSearchmovesFlag(bool val) { _searchmovesFlag = val; }
    void setPonderFlag(bool val) { _ponderFlag = val; }
    void setWtimeFlag(bool val) { _wtimeFlag = val; }
    void setBtimeFlag(bool val) { _btimeFlag = val; }
    void setWincFlag(bool val) { _wincFlag = val; }
    void setBincFlag(bool val) { _bincFlag = val; }
    void setMovestogoFlag(bool val) { _movestogoFlag = val; }
    void setDepthFlag(bool val) { _depthFlag = val; }
    void setNodesFlag(bool val) { _nodesFlag = val; }
    void setMateFlag(bool val) { _mateFlag = val; }
    void setMovetimeFlag(bool val) { _movetimeFlag = val; }
    void setInfiniteFlag(bool val) { _infiniteFlag = val; }
    void setWtime(int val) { _wtime = val; }
    void setBtime(int val) { _btime = val; }
    void setWinc(int val) { _winc = val; }
    void setBinc(int val) { _binc = val; }
    void setMovestogo(int val) { _movestogo = val; }
    void setDepth(int val) { _depth = val; }
    void setNodes(int val) { _nodes = val; }
    void setMate(int val) { _mate = val; }
    void setMovetime(int val) { _movetime = val; }
    void addMove(const std::string & val) { _moves.push_back(val); }
    bool searchmovesFlag() const { return _searchmovesFlag; }
    bool ponderFlag() const { return _ponderFlag; }
    bool wtimeFlag() const { return _wtimeFlag; }
    bool btimeFlag() const { return _btimeFlag; }
    bool wincFlag() const { return _wincFlag; }
    bool bincFlag() const { return _bincFlag; }
    bool movestogoFlag() const { return _movestogoFlag; }
    bool depthFlag() const { return _depthFlag; }
    bool nodesFlag() const { return _nodes; }
    bool mateFlag() const { return _mateFlag; }
    bool movetimeFlag() const { return _movetimeFlag; }
    bool infiniteFlag() const { return _infiniteFlag; }
    int wtime() const { return _wtime; }
    int btime() const { return _btime; }
    int winc() const { return _winc; }
    int binc() const { return _binc; }
    int movestogo() const { return _movestogo; }
    int depth() const { return _depth; }
    int nodes() const { return _nodes; }
    int mate() const { return _mate; }
    int movetime() const { return _movetime; }
    std::vector<std::string> moves() const { return _moves; }

private:
    bool _searchmovesFlag;
    bool _ponderFlag;
    bool _wtimeFlag;
    bool _btimeFlag;
    bool _wincFlag;
    bool _bincFlag;
    bool _movestogoFlag;
    bool _depthFlag;
    bool _nodesFlag;
    bool _mateFlag;
    bool _movetimeFlag;
    bool _infiniteFlag;
    int _wtime;
    int _btime;
    int _winc;
    int _binc;
    int _movestogo;
    int _depth;
    int _nodes;
    int _mate;
    int _movetime;
    std::vector<std::string> _moves;
};

}
}
}

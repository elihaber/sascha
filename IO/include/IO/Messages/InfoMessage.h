#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class InfoMessage : public Message {
public:
    InfoMessage() : Message(MessageType::INFO), _selDepthFlag(false), _depthFlag(false), _nodesFlag(false), _mateFlag(false), _timeFlag(false), _pvFlag(false), _multiPvFlag(false),
                    _scoreFlag(false), _centipawnFlag(false), _lowerBoundFlag(false), _upperBoundFlag(false), _currMoveFlag(false),
                    _currMoveNumberFlag(false), _hashFullFlag(false), _nodesPerSecondFlag(false), _tableBaseHitsFlag(false),
                    _cpuLoadFlag(false), _displayStringFlag(false), _refutationFlag(false), _currLineFlag(false), _cpuNumberFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;
    bool selDepthFlag() const { return _selDepthFlag; }
    bool depthFlag() const { return _depthFlag; }
    bool nodesFlag() const { return _nodesFlag; }
    bool mateFlag() const { return _mateFlag; }
    bool timeFlag() const { return _timeFlag; }
    bool pvFlag() const { return _pvFlag; }
    bool multiPvFlag() const { return _multiPvFlag; }
    bool scoreFlag() const { return _scoreFlag; }
    bool centipawnFlag() const { return _centipawnFlag; }
    bool lowerBoundFlag() const { return _lowerBoundFlag; }
    bool upperBoundFlag() const { return _upperBoundFlag; }
    bool currMoveFlag() const { return _currMoveFlag; }
    bool currMoveNumberFlag() const { return _currMoveNumberFlag; }
    bool hashFullFlag() const { return _hashFullFlag; }
    bool nodesPerSecondFlag() const { return _nodesPerSecondFlag; }
    bool tableBaseHitsFlag() const { return _tableBaseHitsFlag; }
    bool cpuLoadFlag() const { return _cpuLoadFlag; }
    bool displayStringFlag() const { return _displayStringFlag; }
    bool refutationFlag() const { return _refutationFlag; }
    bool currLineFlag() const { return _currLineFlag; }
    bool cpuNumberFlag() const { return _cpuNumberFlag; }
    int selDepth() const { return _selDepth; }
    int time() const { return _time; }
    int multiPv() const { return _multiPv; }
    int centipawn() const { return _centipawn; }
    std::string currMove() const { return _currMove; }
    int currMoveNumber() const { return _currMoveNumber; }
    int hashFull() const { return _hashFull; }
    int nodesPerSecond() const { return _nodesPerSecond; }
    int tableBaseHits() const { return _tableBaseHits; }
    int cpuLoad() const { return _cpuLoad; }
    std::string displayString() const { return _displayString; }
    int cpuNumber() const { return _cpuNumber; }
    std::vector<std::string> moves() const { return _moves; }
    int depth() const { return _depth; }
    int nodes() const { return _nodes; }
    int mate() const { return _mate; }
    void setSelDepthFlag(bool val) { _selDepthFlag = val; }
    void setDepthFlag(bool val) { _depthFlag = val; }
    void setNodesFlag(bool val) { _nodesFlag = val; }
    void setMateFlag(bool val) { _mateFlag = val; }
    void setTimeFlag(bool val) { _timeFlag = val; }
    void setPvFlag(bool val) { _pvFlag = val; }
    void setMultiPvFlag(bool val) { _multiPvFlag = val; }
    void setScoreFlag(bool val) { _scoreFlag = val; }
    void setCentipawnFlag(bool val) { _centipawnFlag = val; }
    void setLowerBoundFlag(bool val) { _lowerBoundFlag = val; }
    void setUpperBoundFlag(bool val) { _upperBoundFlag = val; }
    void setCurrMoveFlag(bool val) { _currMoveFlag = val; }
    void setCurrMoveNumberFlag(bool val) { _currMoveNumberFlag = val; }
    void setHashFullFlag(bool val) { _hashFullFlag = val; }
    void setNodesPerSecondFlag(bool val) { _nodesPerSecondFlag = val; }
    void setTableBaseHitsFlag(bool val) { _tableBaseHitsFlag = val; }
    void setCpuLoadFlag(bool val) { _cpuLoadFlag = val; }
    void setDisplayStringFlag(bool val) { _displayStringFlag = val; }
    void setRefutationFlag(bool val) { _refutationFlag = val; }
    void setCurrLineFlag(bool val) { _currLineFlag = val; }
    void setCpuNumberFlag(bool val) { _cpuNumberFlag = val; }
    void setSelDepth(int val) { _selDepth = val; }
    void setTime(int val) { _time = val; }
    void setMultiPv(int val) { _multiPv = val; }
    void setCentipawn(int val) { _centipawn = val; }
    void setCurrMove(const std::string & val) { _currMove = val; }
    void setCurrMoveNumber(int val) { _currMoveNumber = val; }
    void setHashFull(int val) { _hashFull = val; }
    void setNodesPerSecond(int val) { _nodesPerSecond = val; }
    void setTableBaseHits(int val) { _tableBaseHits = val; }
    void setCpuLoad(int val) { _cpuLoad = val; }
    void setDisplayString(const std::string & val) { _displayString = val; }
    void setCpuNumber(int val) { _cpuNumber = val; }
    void addMove(const std::string & val) { _moves.push_back(val); }
    void setDepth(int val) { _depth = val; }
    void setNodes(int val) { _nodes = val; }
    void setMate(int val) { _mate = val; }

private:
    bool _selDepthFlag;
    bool _depthFlag;
    bool _nodesFlag;
    bool _mateFlag;
    bool _timeFlag;
    bool _pvFlag;
    bool _multiPvFlag;
    bool _scoreFlag;
    bool _centipawnFlag;
    bool _lowerBoundFlag;
    bool _upperBoundFlag;
    bool _currMoveFlag;
    bool _currMoveNumberFlag;
    bool _hashFullFlag;
    bool _nodesPerSecondFlag;
    bool _tableBaseHitsFlag;
    bool _cpuLoadFlag;
    bool _displayStringFlag;
    bool _refutationFlag;
    bool _currLineFlag;
    bool _cpuNumberFlag;
    int _selDepth;
    int _time;
    int _multiPv;
    int _centipawn;
    std::string _currMove;
    int _currMoveNumber;
    int _hashFull;
    int _nodesPerSecond;
    int _tableBaseHits;
    int _cpuLoad;
    std::string _displayString;
    int _cpuNumber;
    std::vector<std::string> _moves;
    int _depth;
    int _nodes;
    int _mate;
};

}
}
}

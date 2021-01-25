#pragma once

#include <utility>
#include <string>
#include <vector>

enum class MessageType {
    NONE,

    // Input
    UCI,
    DEBUG,
    ISREADY,
    SETOPTION,
    REGISTER,
    UCINEWGAME,
    POSITION,
    GO,
    STOP,
    PONDERHIT,
    QUIT,

    // Outupt
    ID,
    UCIOK,
    READYOK,
    BESTMOVE,
    COPYPROTECTION,
    REGISTRATION,
    INFO,
    OPTION
};

class Message {
public:
    Message(MessageType messageType) : _messageType(messageType) { }
    MessageType messageType() const { return _messageType; }
    void setMessageType(MessageType val) { _messageType = val; }
private:
    MessageType _messageType;
};

class UciMessage : public Message {
public:
    UciMessage() : Message(MessageType::UCI) { }
};

class DebugMessage : public Message {
public:
    DebugMessage() : Message(MessageType::DEBUG), _debugFlag(false) { }
    void setDebugFlag(bool val) { _debugFlag = val; }
    bool debugFlag() const { return _debugFlag; }
private:
    bool _debugFlag;
};

class IsReadyMessage : public Message {
public:
    IsReadyMessage() : Message(MessageType::ISREADY) { }
};

class SetOptionMessage : public Message {
public:
    SetOptionMessage() : Message(MessageType::SETOPTION) { }
    void setOptionNameValue(const std::string & name, const std::string & val) { _optionNameValue = std::make_pair(name, val); }
private:
    std::pair<std::string, std::string> _optionNameValue;
};

class RegisterMessage : public Message {
public:
    RegisterMessage() : Message(MessageType::REGISTER), _registerLaterFlag(false), _registerNameFlag(false), _registerCodeFlag(false) { }
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

class UciNewGameMessage : public Message {
public:
    UciNewGameMessage() : Message(MessageType::UCINEWGAME) { }
};

class PositionMessage : public Message {
public:
    PositionMessage() : Message(MessageType::POSITION), _startposFlag(false), _fenFlag(false), _movesFlag(false) { }
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

class GoMessage : public Message {
public:
    GoMessage() : Message(MessageType::GO), _searchmovesFlag(false), _ponderFlag(false), _wtimeFlag(false), _btimeFlag(false),
                  _wincFlag(false), _bincFlag(false), _movestogoFlag(false), _depthFlag(false), _nodesFlag(false), _mateFlag(false),
                  _movetimeFlag(false), _infiniteFlag(false) { }
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

class StopMessage : public Message {
public:
    StopMessage() : Message(MessageType::STOP) { }
};

class PonderHitMessage : public Message {
public:
    PonderHitMessage() : Message(MessageType::PONDERHIT) { }
};

class QuitMessage : public Message {
public:
    QuitMessage() : Message(MessageType::QUIT) { }
};

class IdMessage : public Message {
public:
    IdMessage() : Message(MessageType::ID), _nameFlag(false), _authorFlag(false) { }
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

class UciOkMessage : public Message {
public:
    UciOkMessage() : Message(MessageType::UCIOK) { }
};

class ReadyOkMessage : public Message {
public:
    ReadyOkMessage() : Message(MessageType::READYOK) { }
};

class BestMoveMessage : public Message {
public:
    BestMoveMessage() : Message(MessageType::BESTMOVE), _ponderFlag(false) { }
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

class CopyProtectionMessage : public Message {
public:
    CopyProtectionMessage() : Message(MessageType::COPYPROTECTION), _checkingFlag(false), _okFlag(false), _errorFlag(false) { }
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

class RegistrationMessage : public Message {
public:
    RegistrationMessage() : Message(MessageType::REGISTRATION), _checkingFlag(false), _okFlag(false), _errorFlag(false) { }
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

class InfoMessage : public Message {
public:
    InfoMessage() : Message(MessageType::INFO), _selDepthFlag(false), _depthFlag(false), _nodesFlag(false), _mateFlag(false), _timeFlag(false), _pvFlag(false), _multiPvFlag(false),
                    _scoreFlag(false), _centipawnFlag(false), _lowerBoundFlag(false), _upperBoundFlag(false), _currMoveFlag(false),
                    _currMoveNumberFlag(false), _hashFullFlag(false), _nodesPerSecondFlag(false), _tableBaseHitsFlag(false),
                    _cpuLoadFlag(false), _displayStringFlag(false), _refutationFlag(false), _currLineFlag(false), _cpuNumberFlag(false) { }
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

class OptionMessage : public Message {
public:
    OptionMessage() : Message(MessageType::OPTION), _defaultFlag(false), _minFlag(false), _maxFlag(false), _varFlag(false), _nameFlag(false) { }
    bool defaultFlag() const { return _defaultFlag; }
    bool minFlag() const { return _minFlag; }
    bool maxFlag() const { return _maxFlag; }
    bool varFlag() const { return _varFlag; }
    bool nameFlag() const { return _nameFlag; }
    std::string optionType() const { return _optionType; }
    std::string defaultVal() const { return _defaultVal; }
    std::string name() const { return _name; }
    int min() const { return _min; }
    int max() const { return _max; }
    std::vector<std::string> vars() const { return _vars; }
    void setDefaultFlag(bool val) { _defaultFlag = val; }
    void setMinFlag(bool val) { _minFlag = val; }
    void setMaxFlag(bool val) { _maxFlag = val; }
    void setVarFlag(bool val) { _varFlag = val; }
    void setNameFlag(bool val) { _nameFlag = val; }
    void setOptionType(const std::string & val) { _optionType = val; }
    void setDefaultVal(const std::string & val) { _defaultVal = val; }
    void setName(const std::string & val) { _name = val; }
    void setMin(int val) { _min = val; }
    void setMax(int val) { _max = val; }
    void addVar(const std::string & val) { _vars.push_back(val); }

private:
    bool _defaultFlag;
    bool _minFlag;
    bool _maxFlag;
    bool _varFlag;
    bool _nameFlag;
    std::string _optionType;
    std::string _defaultVal;
    std::string _name;
    int _min;
    int _max;
    std::vector<std::string> _vars;
};

class Message1 {
public:
    void setType(MessageType messageType) { _messageType = messageType; }
    MessageType type() const { return _messageType; }

    // DEBUG message
    void setDebugFlag(bool val) { _debugFlag = val; }

    // SETOPTION message
    void setOptionNameValue(const std::string & name, const std::string & val) { _optionNameValue = std::make_pair(name, val); }

    // REGISTER message
    void setRegisterLaterFlag(bool val) { _registerLaterFlag = val; }
    void setRegisterNameFlag(bool val) { _registerNameFlag = val; }
    void setRegisterCodeFlag(bool val) { _registerCodeFlag = val; }
    void setRegisterName(const std::string & val) { _registerName = val; }
    void setRegisterCode(const std::string & val) { _registerCode = val; }

    // POSITION message
    void setStartposFlag(bool val) { _startposFlag = val; }
    void setFenFlag(bool val) { _fenFlag = val; }
    void setFen(const std::string & val) { _fen = val; }
    void setMovesFlag(bool val) { _movesFlag = val; }
    void addMove(const std::string & val) { _moves.push_back(val); }
    bool fenFlag() const { return _fenFlag; }
    bool movesFlag() const { return _movesFlag; }
    std::vector<std::string> moves() const { return _moves; }
    bool startposFlag() const { return _startposFlag; }

    // GO message
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
    bool ponderFlag() const { return _ponderFlag; }
    bool depthFlag() const { return _depthFlag; }
    bool nodesFlag() const { return _nodes; }
    bool mateFlag() const { return _mateFlag; }
    int mate() const { return _mate; }
    int depth() const { return _depth; }
    int nodes() const { return _nodes; }

    // ID message
    bool nameFlag() const { return _nameFlag; }
    bool authorFlag() const { return _authorFlag; }
    std::string name() const { return _name; }
    std::string author() const { return _author; }
    void setNameFlag(bool val) { _nameFlag = val; }
    void setAuthorFlag(bool val) { _authorFlag = val; }
    void setName(const std::string & val) { _name = val; }
    void setAuthor(const std::string & val) { _author = val; }

    // BESTMOVE message
    std::string move1() const { return _move1; }
    std::string move2() const { return _move2; }

    // COPYPROTECTION and REGISTRATIO messages
    bool checkingFlag() const { return _checkingFlag; }
    bool okFlag() const { return _okFlag; }
    bool errorFlag() const { return _errorFlag; }

    // INFO message
    bool selDepthFlag() const { return _selDepthFlag; }
    int selDepth() const { return _selDepth; }
    bool timeFlag() const { return _timeFlag; }
    int time() const { return _time; }
    bool pvFlag() const { return _pvFlag; }
    bool multiPvFlag() const { return _multiPvFlag; }
    int multiPv() const { return _multiPv; }
    bool scoreFlag() const { return _scoreFlag; }
    bool centipawnFlag() const { return _centipawnFlag; }
    int centipawn() const { return _centipawn; }
    bool lowerBoundFlag() const { return _lowerBoundFlag; }
    bool upperBoundFlag() const { return _upperBoundFlag; }
    bool currMoveFlag() const { return _currMoveFlag; }
    std::string currMove() const { return _currMove; }
    bool currMoveNumberFlag() const { return _currMoveNumberFlag; }
    int currMoveNumber() const { return _currMoveNumber; }
    bool hashFullFlag() const { return _hashFullFlag; }
    int hashFull() const { return _hashFull; }
    bool nodesPerSecondFlag() const { return _nodesPerSecondFlag; }
    int nodesPerSecond() const { return _nodesPerSecond; }
    bool tableBaseHitsFlag() const { return _tableBaseHitsFlag; }
    int tableBaseHits() const { return _tableBaseHits; }
    bool cpuLoadFlag() const { return _cpuLoadFlag; }
    int cpuLoad() const { return _cpuLoad; }
    bool displayStringFlag() const { return _displayStringFlag; }
    std::string displayString() const { return _displayString; }
    bool refutationFlag() const { return _refutationFlag; }
    bool currLineFlag() const { return _currLineFlag; }
    bool cpuNumberFlag() const { return _cpuNumberFlag; }
    int cpuNumber() const { return _cpuNumber; }

    // OPTION message
    std::string optionType() const { return _optionType; }
    bool defaultFlag() const { return _defaultFlag; }
    std::string defaultVal() const { return _defaultVal; }
    bool minFlag() const { return _minFlag; }
    int min() const { return _min; }
    bool maxFlag() const { return _maxFlag; }
    int max() const { return _max; }
    bool varFlag() const { return _varFlag; }
    std::vector<std::string> vars() const { return _vars; }

private:
    MessageType _messageType;

    // DEBUG message
    bool _debugFlag;

    // SETOPTION message
    std::pair<std::string, std::string> _optionNameValue;

    // REGISTER message
    bool _registerLaterFlag;
    bool _registerNameFlag;
    bool _registerCodeFlag;
    std::string _registerName;
    std::string _registerCode;

    // POSITION message
    bool _startposFlag;
    bool _fenFlag;
    bool _movesFlag;
    std::string _fen;
    std::vector<std::string> _moves;

    // GO message
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

    // ID message
    bool _nameFlag;
    bool _authorFlag;
    std::string _name;
    std::string _author;

    // BESTMOVE message
    std::string _move1;
    std::string _move2;

    // COPYPROTECTION and REGISTRATION messages
    bool _checkingFlag;
    bool _okFlag;
    bool _errorFlag;

    // INFO message
    bool _selDepthFlag;
    int _selDepth;
    bool _timeFlag;
    int _time;
    bool _pvFlag;
    bool _multiPvFlag;
    int _multiPv;
    bool _scoreFlag;
    bool _centipawnFlag;
    int _centipawn;
    bool _lowerBoundFlag;
    bool _upperBoundFlag;
    bool _currMoveFlag;
    std::string _currMove;
    bool _currMoveNumberFlag;
    int _currMoveNumber;
    bool _hashFullFlag;
    int _hashFull;
    bool _nodesPerSecondFlag;
    int _nodesPerSecond;
    bool _tableBaseHitsFlag;
    int _tableBaseHits;
    bool _cpuLoadFlag;
    int _cpuLoad;
    bool _displayStringFlag;
    std::string _displayString;
    bool _refutationFlag;
    bool _currLineFlag;
    bool _cpuNumberFlag;
    int _cpuNumber;

    // OPTION message
    std::string _optionType;
    bool _defaultFlag;
    std::string _defaultVal;
    bool _minFlag;
    int _min;
    bool _maxFlag;
    int _max;
    bool _varFlag;
    std::vector<std::string> _vars;
};

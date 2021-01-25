#pragma once

#include "Player.h"

class MessageQueue;
class Board;

class Engine {

public:
    Engine(MessageQueue & incomingMessages, MessageQueue & outgoingMessages) : _incomingMessages(incomingMessages), _outgoingMessages(outgoingMessages), _isDone(false) { }
    void start();
    bool isDone();

private:
    void _handleInputQueue();

    MessageQueue & _incomingMessages;
    MessageQueue & _outgoingMessages;
    bool _isDone;
    std::shared_ptr<Board> _board;
    Player _whitePlayer;
    Player _blackPlayer;

};

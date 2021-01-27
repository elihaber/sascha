#pragma once

#include "Gameplay/Player.h"

namespace Sascha {

namespace IO { class MessageQueue; }
namespace Gameplay { class Board; }

namespace Engine {

class Engine {

public:
    Engine(Sascha::IO::MessageQueue & incomingMessages, Sascha::IO::MessageQueue & outgoingMessages) : _incomingMessages(incomingMessages), _outgoingMessages(outgoingMessages), _isDone(false) { }
    void start();
    bool isDone();

private:
    void _handleInputQueue();

    Sascha::IO::MessageQueue & _incomingMessages;
    Sascha::IO::MessageQueue & _outgoingMessages;
    bool _isDone;
    std::shared_ptr<Sascha::Gameplay::Board> _board;
    Sascha::Gameplay::Player _whitePlayer;
    Sascha::Gameplay::Player _blackPlayer;

};

}
}

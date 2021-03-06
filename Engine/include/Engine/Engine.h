#pragma once

namespace Sascha {

namespace IO { class MessageQueue; }
namespace Gameplay { class Board; }

namespace Engine {

namespace Evaluators { class Evaluator; }

class Options;

class Engine {

public:
    Engine(Sascha::IO::MessageQueue & incomingMessages, Sascha::IO::MessageQueue & outgoingMessages);
    void start();
    bool isDone();

private:
    void _handleInputQueue();

    Sascha::IO::MessageQueue & _incomingMessages;
    Sascha::IO::MessageQueue & _outgoingMessages;
    bool _isDone;
    std::shared_ptr<Sascha::Gameplay::Board> _board;
    std::shared_ptr<Evaluators::Evaluator> _evaluator;
    //std::thread _evaluatorThread;
    bool _evaluatorRunning;
    bool _startEvaluatorFlag;
    std::shared_ptr<Options> _options;
};

}
}

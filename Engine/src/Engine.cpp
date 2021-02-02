#include <thread>
#include <memory>
#include "Engine/Engine.h"
#include "IO/Messages/AllMessages.h"
#include "IO/MessageQueue.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"
#include "Globals.h"
#include "Engine/Evaluators/Evaluator.h"

using namespace std::chrono_literals;

namespace Sascha {
namespace Engine {

using namespace Sascha::IO::Messages;
using Sascha::Gameplay::Board;
using Sascha::Gameplay::Move;

Engine::Engine(Sascha::IO::MessageQueue & incomingMessages, Sascha::IO::MessageQueue & outgoingMessages) :
        _incomingMessages(incomingMessages), _outgoingMessages(outgoingMessages), _isDone(false), _evaluatorRunning(false), _startEvaluatorFlag(false) {
    _evaluator = Evaluators::Evaluator::createEvaluator();
}

void Engine::start() {
    std::thread evaluatorThread;
    srand (time(NULL));
    while (true) {
//        MAINLOG("Engine start loop")
        if (_isDone) {
            MAINLOG("Engine killing loop")
            break;
        }

        _handleInputQueue();

        if (_evaluatorRunning) {
            if (_evaluator->isDone()) {
                _evaluatorRunning = false;
                evaluatorThread.join();

                auto bestMove = _evaluator->calculatedMove();
                try {
                auto bestMoveMessage = std::static_pointer_cast<BestMoveMessage>(Message::createMessage(MessageType::BESTMOVE));
                bestMoveMessage->setMove1(bestMove->uciFormat());
                _outgoingMessages.addMessage(bestMoveMessage);
                }
                catch (std::runtime_error&) {
                }
                MAINLOG("Outgoing message queue has " << _outgoingMessages.size() << " messages")
            }
        }

        if (_startEvaluatorFlag) {
            _startEvaluatorFlag = false;
            _evaluatorRunning = true;
            evaluatorThread = std::thread(Evaluators::Evaluator::calculateBestMove, _evaluator);
        }

        std::this_thread::sleep_for(100ms);
    }
}

void Engine::_handleInputQueue() {
    while (true) {
        auto message = _incomingMessages.readNextMessage();
        if (message->messageType() == MessageType::NONE) {
            break;
        }
        if (message->messageType() == MessageType::UCI) {
            auto idMessage = std::static_pointer_cast<IdMessage>(Message::createMessage(MessageType::ID));
            idMessage->setNameFlag(true);
            idMessage->setName("Sascha");
            idMessage->setAuthorFlag(true);
            idMessage->setAuthor("Eli Haber");
            _outgoingMessages.addMessage(idMessage);

            auto uciokMessage = std::static_pointer_cast<UciOkMessage>(Message::createMessage(MessageType::UCIOK));
            _outgoingMessages.addMessage(uciokMessage);
        }

        if (message->messageType() == MessageType::ISREADY) {
            auto readyokMessage = std::static_pointer_cast<ReadyOkMessage>(Message::createMessage(MessageType::READYOK));
            _outgoingMessages.addMessage(readyokMessage);
        }

        if (message->messageType() == MessageType::POSITION) {
            std::shared_ptr<PositionMessage> realMessage = std::static_pointer_cast<PositionMessage>(message);
            _board = std::make_shared<Board>(_whitePlayer, _blackPlayer);
            if (realMessage->startposFlag()) {
                _board->setUpStartingPosition();
            }
            else if (realMessage->fenFlag()) {
                _board->setUpFromFen(realMessage->fen());
            }
            if (realMessage->movesFlag()) {
                auto uciMoves = realMessage->moves();
                for (size_t i = 0; i < uciMoves.size(); ++i) {
                    auto move = std::make_shared<Move>(uciMoves[i], _board);
                    if (i == uciMoves.size() - 1) {
                        _board->handleMoveForFullAnalysis(move);
                    }
                    else {
                        _board->handleMoveForSingleAnalysis(move);
                    }
                }
            }
        }

        if (message->messageType() == MessageType::GO) {
            MAINLOG("Handling go")
            _evaluator->setBoard(_board);
            _startEvaluatorFlag = true;
            //std::thread evaluatorThread(Sascha::Engine::Evaluators::Evaluator::calculateBestMove, _evaluator);
        }

        if (message->messageType() == MessageType::QUIT) {
            MAINLOG("Engine handling quit")
            _isDone = true;
        }
    }
}

bool Engine::isDone() {
    return _isDone;
}

}
}

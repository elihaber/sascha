#include <thread>
#include <memory>
#include "Engine.h"
#include "Message.h"
#include "MessageQueue.h"
#include "Board.h"
#include "Move.h"
#include "Globals.h"

using namespace std::chrono_literals;

void Engine::start() {
    srand (time(NULL));
    while (true) {
        if (_isDone) {
            MAINLOG("Engine killing loop")
            break;
        }

        _handleInputQueue();

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
            auto idMessage = std::make_shared<IdMessage>();
            idMessage->setNameFlag(true);
            idMessage->setName("Sascha");
            idMessage->setAuthorFlag(true);
            idMessage->setAuthor("Eli Haber");
            _outgoingMessages.addMessage(idMessage);

            auto uciokMessage = std::make_shared<UciOkMessage>();
            _outgoingMessages.addMessage(uciokMessage);
        }

        if (message->messageType() == MessageType::ISREADY) {
            auto readyokMessage = std::make_shared<ReadyOkMessage>();
            _outgoingMessages.addMessage(readyokMessage);
        }

        if (message->messageType() == MessageType::POSITION) {
            std::shared_ptr<PositionMessage> realMessage = std::static_pointer_cast<PositionMessage>(message);
            if (realMessage->startposFlag()) {
                _board = std::make_shared<Board>(_whitePlayer, _blackPlayer);
                _board->setUpStartingPosition();
                if (realMessage->movesFlag()) {
                    auto moves = realMessage->moves();
                    for (auto & move : moves) {
                        _board->handleMoveAlgebraic(move);
                    }
                }
            }
            else if (realMessage->fenFlag()) {
                _board = std::make_shared<Board>(_whitePlayer, _blackPlayer);
                _board->setUpFromFen(realMessage->fen());
                if (realMessage->movesFlag()) {
                    auto moves = realMessage->moves();
                    for (auto & move : moves) {
                        _board->handleMoveAlgebraic(move);
                    }
                }
            }

        }

        if (message->messageType() == MessageType::GO) {
            MAINLOG("Handling go")
            std::vector<std::shared_ptr<Move>> possibleMoves;
            _board->getPossibleMoves(possibleMoves);
            MAINLOG_NNL("Got " << possibleMoves.size() << " possible moves:")
            for (size_t i = 0; i < possibleMoves.size(); ++i) {
                MAINLOG_NNL(" " << possibleMoves[i]->algebraicNotation())
            }
            MAINLOG("");
            int randIndex = rand() % possibleMoves.size();
            MAINLOG("Selected index " << randIndex << " which is " << possibleMoves[randIndex]->algebraicNotation())
            auto bestMoveMessage = std::make_shared<BestMoveMessage>();
            bestMoveMessage->setMove1(possibleMoves[randIndex]->algebraicNotation());
            _outgoingMessages.addMessage(bestMoveMessage);
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
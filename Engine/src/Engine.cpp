#include <thread>
#include <memory>
#include "Engine/Engine.h"
#include "IO/Messages/AllMessages.h"
#include "IO/MessageQueue.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"
#include "Globals.h"
#include "Engine/Evaluators/Evaluator.h"
#include "Engine/Options.h"

using namespace std::chrono_literals;

namespace Sascha {
namespace Engine {

using namespace Sascha::IO::Messages;
using Sascha::Gameplay::Board;
using Sascha::Gameplay::Move;

Engine::Engine(Sascha::IO::MessageQueue & incomingMessages, Sascha::IO::MessageQueue & outgoingMessages) :
        _incomingMessages(incomingMessages), _outgoingMessages(outgoingMessages), _isDone(false), _evaluatorRunning(false), _startEvaluatorFlag(false), _options(std::make_shared<Options>()) {
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
        static bool firstIsReadyReceived = false;
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

            auto optionMessage1 = std::static_pointer_cast<OptionMessage>(Message::createMessage(MessageType::OPTION));
            optionMessage1->setNameFlag(true);
            optionMessage1->setName("Hash");
            optionMessage1->setOptionType("spin");
            optionMessage1->setMinFlag(true);
            optionMessage1->setMin(1);
            optionMessage1->setMaxFlag(true);
            optionMessage1->setMax(256);
            optionMessage1->setDefaultFlag(true);
            optionMessage1->setDefaultVal("1");
            _outgoingMessages.addMessage(optionMessage1);

            auto optionMessage2 = std::static_pointer_cast<OptionMessage>(Message::createMessage(MessageType::OPTION));
            optionMessage2->setNameFlag(true);
            optionMessage2->setName("OwnBook");
            optionMessage2->setOptionType("check");
            optionMessage2->setDefaultFlag(true);
            optionMessage2->setDefaultVal("false");
            _outgoingMessages.addMessage(optionMessage2);

            auto optionMessage3 = std::static_pointer_cast<OptionMessage>(Message::createMessage(MessageType::OPTION));
            optionMessage3->setNameFlag(true);
            optionMessage3->setName("Algorithm");
            optionMessage3->setOptionType("combo");
            optionMessage3->setVarFlag(true);
            optionMessage3->addVar("Minimax");
            optionMessage3->addVar("Random");
            optionMessage3->setDefaultFlag(true);
            optionMessage3->setDefaultVal("Minimax");
            _outgoingMessages.addMessage(optionMessage3);

            auto optionMessage4 = std::static_pointer_cast<OptionMessage>(Message::createMessage(MessageType::OPTION));
            optionMessage4->setNameFlag(true);
            optionMessage4->setName("SearchDepth");
            optionMessage4->setOptionType("spin");
            optionMessage4->setMinFlag(true);
            optionMessage4->setMin(2);
            optionMessage4->setMaxFlag(true);
            optionMessage4->setMax(20);
            optionMessage4->setDefaultFlag(true);
            optionMessage4->setDefaultVal("4");
            _outgoingMessages.addMessage(optionMessage4);

            auto optionMessage5 = std::static_pointer_cast<OptionMessage>(Message::createMessage(MessageType::OPTION));
            optionMessage5->setNameFlag(true);
            optionMessage5->setName("InitialMoveCount");
            optionMessage5->setOptionType("spin");
            optionMessage5->setMinFlag(true);
            optionMessage5->setMin(0);
            optionMessage5->setMaxFlag(true);
            optionMessage5->setMax(10);
            optionMessage5->setDefaultFlag(true);
            optionMessage5->setDefaultVal("4");
            _outgoingMessages.addMessage(optionMessage5);

            auto uciokMessage = std::static_pointer_cast<UciOkMessage>(Message::createMessage(MessageType::UCIOK));
            _outgoingMessages.addMessage(uciokMessage);
        }

        if (message->messageType() == MessageType::SETOPTION) {
            std::shared_ptr<SetOptionMessage> realMessage = std::static_pointer_cast<SetOptionMessage>(message);
            if (realMessage->option().first.compare("Hash") == 0) {
                try {
                    _options->setHashSize(std::stoi(realMessage->option().second));
                }
                catch (std::runtime_error) {
                    MAINLOG("ERROR: Failed to set hash size to requested value: " << realMessage->option().second)
                }
            }
            else if (realMessage->option().first.compare("OwnBook") == 0) {
                if (realMessage->option().second.compare("true") == 0 || realMessage->option().second.compare("TRUE") == 0) {
                    _options->setOwnBook(true);
                }
                else if (realMessage->option().second.compare("false") == 0 || realMessage->option().second.compare("FALSE") == 0) {
                    _options->setOwnBook(false);
                }
                else {
                    MAINLOG("ERROR: Failed to set own book flag to requested value: " << realMessage->option().second)
                }
            }
            else if (realMessage->option().first.compare("Algorithm") == 0) {
                if (realMessage->option().second.compare("Minimax") == 0) {
                    _options->setAlgorithm(Algorithm::MINIMAX);
                }
                else if (realMessage->option().second.compare("Random") == 0) {
                    _options->setAlgorithm(Algorithm::RANDOM);
                }
                else {
                    MAINLOG("ERROR: Failed to set algorithm to requested value: " << realMessage->option().second)
                }
            }
            else if (realMessage->option().first.compare("SearchDepth") == 0) {
                try {
                    _options->setSearchDepth(std::stoi(realMessage->option().second));
                }
                catch (std::runtime_error) {
                    MAINLOG("ERROR: Failed to set search depth to requested value: " << realMessage->option().second)
                }
            }
            else if (realMessage->option().first.compare("InitialMoveCount") == 0) {
                try {
                    _options->setInitialMoveCount(std::stoi(realMessage->option().second));
                }
                catch (std::runtime_error) {
                    MAINLOG("ERROR: Failed to set initial move count to requested value: " << realMessage->option().second)
                }
            }
        }

        if (message->messageType() == MessageType::ISREADY) {
            auto readyokMessage = std::static_pointer_cast<ReadyOkMessage>(Message::createMessage(MessageType::READYOK));

            if (!firstIsReadyReceived) {
                _evaluator = Evaluators::Evaluator::createEvaluator(_options);
                firstIsReadyReceived = true;
            }
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

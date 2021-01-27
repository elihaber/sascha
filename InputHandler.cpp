#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <thread>
#include <future>
#include "InputHandler.h"
#include "Message.h"
#include "MessageQueue.h"
#include "Globals.h"

using namespace std::chrono_literals;
namespace Sascha {
namespace IO {

using namespace Messages;

void InputHandler::start(std::future<void> futureObj) {
    _endFlag = false;

    std::string line;

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        std::getline(std::cin, line);
        // Forward line to other thread
        
        INPUTLOG(line)
        MAINLOG_NNL("in >> ")
        if (line.empty()) {
            MAINLOG("[empty line] >> ignored")
        }
        else {
            MAINLOG_NNL(line << " >> ")

            auto message = _parseLine(line);
            if (message->messageType() != MessageType::NONE) {
                _incomingMessages.addMessage(message);
                MAINLOG("processed message type " << (int)message->messageType())
            }
            else {
                MAINLOG("ignored")
            }
        }
        if (line.compare("quit") == 0) {
            std::this_thread::sleep_for(300ms);
        }
        std::this_thread::sleep_for(100ms);
    }
}

void InputHandler::end() {
    _endFlag = true;
}

std::shared_ptr<Message> InputHandler::_parseLine(const std::string & line) {
    auto nomessage = std::make_shared<Message>(MessageType::NONE);

    std::istringstream iss(line);
    //std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::ostream_iterator<std::string>(std::cout, "\n"));
//    std::vector<std::string> tokens;
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    
    const std::string & firstToken = tokens[0];

    if (firstToken.compare("uci") == 0) {
        auto message = std::make_shared<UciMessage>();
        return message;
    }

    if (firstToken.compare("debug") == 0) {
        auto message = std::make_shared<DebugMessage>();
        message->setDebugFlag((tokens.size() > 1 && tokens[1].compare("on") == 0));
        return message;
    }

    if (firstToken.compare("isready") == 0) {
        auto message = std::make_shared<IsReadyMessage>();
        return message;
    }

    if (firstToken.compare("setoption") == 0) {
        auto message = std::make_shared<SetOptionMessage>();
        if (tokens.size() < 5 || tokens[1].compare("name") != 0) {
            return nomessage;
        }

        int valPos = -1;
        for (int i = 2; i < tokens.size(); ++i) {
            if (tokens[i].compare("value") == 0) {
                valPos = i;
                break;
            }
        }

        if (valPos == -1) {
            return nomessage;
        }

        message->setOptionNameValue(_concatTokens(tokens, 2, valPos), _concatTokens(tokens, valPos + 1, tokens.size()));

        return message;
    }

    if (firstToken.compare("register") == 0) {
        auto message = std::make_shared<RegisterMessage>();

        if (tokens.size() < 2) {
            return nomessage;
        }

        if (tokens[1].compare("later") == 0) {
            message->setRegisterLaterFlag(true);
            return message;
        }

        if (tokens.size() < 3) {
            return nomessage;
        }

        if (tokens[1].compare("name") == 0) {
            message->setRegisterNameFlag(true);
            message->setRegisterName(_concatTokens(tokens, 2, tokens.size()));
            return message;
        }

        if (tokens[1].compare("code") == 0) {
            message->setRegisterCodeFlag(true);
            message->setRegisterCode(_concatTokens(tokens, 2, tokens.size()));
            return message;
        }

        return nomessage;
    }

    if (firstToken.compare("ucinewgame") == 0) {
        auto message = std::make_shared<UciNewGameMessage>();
        return message;
    }

    if (firstToken.compare("position") == 0) {
        auto message = std::make_shared<PositionMessage>();

        if (tokens.size() < 2) {
            return nomessage;
        }

        int lastUsedPos = -1;

        if (tokens[1].compare("startpos") == 0) {
            message->setStartposFlag(true);
            lastUsedPos = 1;
        }
        else if (tokens[1].compare("fen") == 0) {
            message->setFenFlag(true);
        }
        else {
            MAINLOG("Returning false because no startpos and no fenflag")
            return nomessage;
        }

        int movesPos = -1;
        for (int i = 2; i < tokens.size(); ++i) {
            if (tokens[i].compare("moves") == 0) {
                movesPos = i;
                break;
            }
        }

        message->setMovesFlag(movesPos != -1);

        if (message->fenFlag()) {
            if (movesPos == 2) {
                MAINLOG("Returning false because fenflag and movesPos == 2")
                return nomessage;
            }
            message->setFen(_concatTokens(tokens, 2, movesPos));
        }
        
        if (message->movesFlag()) {
            for (int i = movesPos + 1; i < tokens.size(); ++i) {
                message->addMove(tokens[i]);
            }
        }

        return message;
    }

    if (firstToken.compare("go") == 0) {
        auto message = std::make_shared<GoMessage>();

        //Get keywords and positions.
        std::vector<std::string> keywords = { "searchmoves", "ponder", "wtime", "btime", "winc", "binc", "movestogo", "depth", "nodes", "mate", "movetime", "infinite" };
        std::vector<std::string> keytokens;
        std::vector<int> keypositions;
        for (int i = 0; i < tokens.size(); ++i) {
            if (std::find(keywords.begin(), keywords.end(), tokens[i]) != keywords.end()) {
                keytokens.push_back(tokens[i]);
                keypositions.push_back(i);
            }
        }

        for (int i = 0; i < keytokens.size(); ++i) {
            std::string key = keytokens[i];
            int keypos = keypositions[i];
            int nextkeypos = tokens.size();
            if (i < keytokens.size() - 1) {
                nextkeypos = keypositions[i + 1];
            }

            if (key.compare("searchmoves") == 0) {
                message->setSearchmovesFlag(true);
                for (int j = keypos + 1; j < nextkeypos; ++j) {
                    message->addMove(tokens[j]);
                }
            }

            if (key.compare("ponder") == 0) {
                message->setPonderFlag(true);
            }

            if (key.compare("wtime") == 0) {
                message->setWtimeFlag(true);
                try {
                    message->setWtime(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    return nomessage;
                }
            }

            if (key.compare("btime") == 0) {
                message->setBtimeFlag(true);
                try {
                    message->setBtime(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    MAINLOG("Exception in btime while converting " << tokens[keypos + i])
                    return nomessage;
                }
            }

            if (key.compare("winc") == 0) {
                message->setWincFlag(true);
                try {
                    message->setWinc(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    MAINLOG("Exception in winc while converting " << tokens[keypos + i])
                    return nomessage;
                }
            }

            if (key.compare("binc") == 0) {
                message->setBincFlag(true);
                try {
                    message->setBinc(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    MAINLOG("Exception in binc while converting " << tokens[keypos + i])
                    return nomessage;
                }
            }

            if (key.compare("movestogo") == 0) {
                message->setMovestogoFlag(true);
                try {
                    message->setMovestogo(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    return nomessage;
                }
            }

            if (key.compare("depth") == 0) {
                message->setDepthFlag(true);
                try {
                    message->setDepth(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    return nomessage;
                }
            }

            if (key.compare("nodes") == 0) {
                message->setNodesFlag(true);
                try {
                    message->setNodes(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    return nomessage;
                }
            }

            if (key.compare("mate") == 0) {
                message->setMateFlag(true);
                try {
                    message->setMate(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    return nomessage;
                }
            }

            if (key.compare("movetime") == 0) {
                message->setMovetimeFlag(true);
                try {
                    message->setMovetime(std::stoi(tokens[keypos + 1]));
                }
                catch (std::exception &) {
                    return nomessage;
                }
            }

            if (key.compare("infinite") == 0) {
                message->setInfiniteFlag(true);
            }

            return message;
        }
    }

    if (firstToken.compare("stop") == 0) {
        auto message = std::make_shared<StopMessage>();
        return message;
    }

    if (firstToken.compare("ponderhit") == 0) {
        auto message = std::make_shared<PonderHitMessage>();
        return message;
    }

    if (firstToken.compare("quit") == 0) {
        auto message = std::make_shared<QuitMessage>();
        return message;
    }

    return nomessage;
}

std::string InputHandler::_concatTokens(const std::vector<std::string> tokens, int start, int end) {
    std::stringstream sstr;
    for(size_t i = start; i < end; ++i)  {
        if(i != start) {
            sstr << " ";
        }
        sstr << tokens[i];
    }

    return sstr.str();        
}

}
}

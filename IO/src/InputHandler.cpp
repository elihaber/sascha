#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <thread>
#include <future>
#include "IO/InputHandler.h"
#include "IO/Messages/AllMessages.h"
#include "IO/MessageQueue.h"
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
    auto nomessage = Message::createMessage(MessageType::NONE);

    std::istringstream iss(line);
    //std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::ostream_iterator<std::string>(std::cout, "\n"));
//    std::vector<std::string> tokens;
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    
    const std::string & firstToken = tokens[0];

    std::shared_ptr<Message> message;

    if (firstToken.compare("uci") == 0) {
        message = Message::createMessage(MessageType::UCI);
    }
    else if (firstToken.compare("debug") == 0) {
        message = Message::createMessage(MessageType::DEBUG);
    }
    else if (firstToken.compare("isready") == 0) {
        message = Message::createMessage(MessageType::ISREADY);
    }
    else if (firstToken.compare("setoption") == 0) {
        message = Message::createMessage(MessageType::SETOPTION);
    }
    else if (firstToken.compare("register") == 0) {
        message = Message::createMessage(MessageType::REGISTER);
    }
    else if (firstToken.compare("ucinewgame") == 0) {
        message = Message::createMessage(MessageType::UCINEWGAME);
    }
    else if (firstToken.compare("position") == 0) {
        message = Message::createMessage(MessageType::POSITION);
    }
    else if (firstToken.compare("go") == 0) {
        message = std::make_shared<GoMessage>();
    }
    else if (firstToken.compare("stop") == 0) {
        message = std::make_shared<StopMessage>();
    }
    else if (firstToken.compare("ponderhit") == 0) {
        message = std::make_shared<PonderHitMessage>();
    }
    else if (firstToken.compare("quit") == 0) {
        message = std::make_shared<QuitMessage>();
    }
    else {
        return nomessage;
    }

    message->parseInputLine(line);
    return message;
}

}
}

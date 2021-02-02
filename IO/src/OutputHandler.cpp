#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <thread>
#include <memory>
#include <future>
#include "IO/OutputHandler.h"
#include "IO/Messages/Message.h"
#include "IO/MessageQueue.h"
#include "Globals.h"

using namespace std::chrono_literals;

namespace Sascha {
namespace IO {

using namespace Messages;

void OutputHandler::start(std::future<void> futureObj) {
    std::string line;

    while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout) {
        auto message = _outgoingMessages.readNextMessage();
        if (message->messageType() != MessageType::NONE) {
            std::stringstream line;
            if (message->formatMessage(line)) {
                OUTPUTLOG("out << " << line.str())
                MAINLOG("out << " << line.str())
                MAINLOG("Outgo1ng message queue has " << _outgoingMessages.size() << " messages")
                std::cout << line.str() << std::endl;
            }
            else {
                MAINLOG("Failed to format message for output")
            }
        }

        std::this_thread::sleep_for(100ms);
    }
    OUTPUTLOG("Output loop has exited")
    MAINLOG("Output loop has exited")
}

void OutputHandler::end() {
    _endFlag = true;
}

}
}

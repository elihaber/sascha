#pragma once

#include <atomic>
#include <vector>
#include <string>

namespace Sascha {
namespace IO {

class MessageQueue;
namespace Messages { class Message; }

class InputHandler {

public:
    InputHandler(MessageQueue & incomingMessages) : _incomingMessages(incomingMessages), _endFlag(false) { }

    void start(std::future<void> futureObj);
    void end();

private:
    std::shared_ptr<Messages::Message> _parseLine(const std::string & line);

    MessageQueue & _incomingMessages;
    bool _endFlag;
};

}
}

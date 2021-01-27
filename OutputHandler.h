#pragma once

#include <atomic>
#include <vector>
#include <string>

namespace Sascha {
namespace IO {

class MessageQueue;
namespace Messages { class Message; }

class OutputHandler {

public:
    OutputHandler(MessageQueue & outgoingMessages) : _outgoingMessages(outgoingMessages), _endFlag(false) { }

    void start(std::future<void> futureObj);
    void end();

private:
    bool _formatMessage(std::shared_ptr<Messages::Message> message, std::stringstream & line);

    MessageQueue & _outgoingMessages;
    bool _endFlag;
};

}
}

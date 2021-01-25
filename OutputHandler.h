#pragma once

#include <atomic>
#include <vector>
#include <string>

class MessageQueue;
class Message;

class OutputHandler {

public:
    OutputHandler(MessageQueue & outgoingMessages) : _outgoingMessages(outgoingMessages), _endFlag(false) { }

    void start(std::future<void> futureObj);
    void end();

private:
    bool _formatMessage(std::shared_ptr<Message> message, std::stringstream & line);

    MessageQueue & _outgoingMessages;
    bool _endFlag;
};

#pragma once

#include <atomic>
#include <vector>
#include <string>

class MessageQueue;
class Message;

class InputHandler {

public:
    InputHandler(MessageQueue & incomingMessages) : _incomingMessages(incomingMessages), _endFlag(false) { }

    void start(std::future<void> futureObj);
    void end();

private:
    std::shared_ptr<Message> _parseLine(const std::string & line);
    std::string _concatTokens(const std::vector<std::string> tokens, int start, int end);

    MessageQueue & _incomingMessages;
    bool _endFlag;
};
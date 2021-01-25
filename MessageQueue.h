#pragma once

#include <queue>
#include <mutex>
#include "Message.h"

class MessageQueue {
public:
    MessageQueue(int id) : _id(id) { }
    void addMessage(std::shared_ptr<Message> message);
    std::shared_ptr<Message> readNextMessage();
private:
    std::queue<std::shared_ptr<Message>> _messageQue;
    std::mutex _queProtector;
    int _id;
};

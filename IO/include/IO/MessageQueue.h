#pragma once

#include <queue>
#include <mutex>
#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {

namespace Messages { class Message; }

class MessageQueue {
public:
    MessageQueue(int id) : _id(id) { }
    void addMessage(std::shared_ptr<Messages::Message> message);
    std::shared_ptr<Messages::Message> readNextMessage();
    int size();
private:
    std::queue<std::shared_ptr<Messages::Message>> _messageQue;
    std::mutex _queProtector;
    int _id;
};

}
}

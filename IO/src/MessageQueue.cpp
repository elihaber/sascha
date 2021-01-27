#include <memory>
#include "IO/MessageQueue.h"
#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {

using Messages::Message;
using Messages::MessageType;

void MessageQueue::addMessage(std::shared_ptr<Message> message) {
    std::lock_guard<std::mutex> lg(_queProtector);
    _messageQue.push(message);
}

std::shared_ptr<Message> MessageQueue::readNextMessage() {
    std::lock_guard<std::mutex> lg(_queProtector);
    if (_messageQue.empty()) {
        return std::make_shared<Message>(MessageType::NONE);
    }
    auto message = _messageQue.front();
    _messageQue.pop();
    return message;
}

}
}

#include <iostream>
#include <thread>
#include <future>
#include "InputHandler.h"
#include "OutputHandler.h"
#include "MessageQueue.h"
#include "Engine.h"
#include "log.h"

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
   log::out << "Welcome to Sascha!" << std::endl << std::endl; log::flush();

    bool res = Position::areSquaresKnightsMoveFromEachOther(Position(4,6), Position(2,5));
    if (res) {
        log::out << "ATTACK!" << std::endl; log::flush();
    }
    else {
        log::out << "All clear!" << std::endl; log::flush();
    }

    std::promise<void> exitSignal1;
    std::future<void> futureObj1 = exitSignal1.get_future();
    std::promise<void> exitSignal2;
    std::future<void> futureObj2 = exitSignal2.get_future();

    // Create incoming commands queue
    MessageQueue incomingMessages(1);

    // Create Outgoing commands queue
    MessageQueue outgoingMessages(2);

    // Create engine
    Engine engine(incomingMessages, outgoingMessages);
    std::thread engineThread(Engine::start, engine);
    
    // Start input handler
    InputHandler inputHandler(incomingMessages);
    std::thread inputThread(InputHandler::start, inputHandler, std::move(futureObj1));

    // Start output handler
    OutputHandler outputHandler(outgoingMessages);
    std::thread outputThread(OutputHandler::start, outputHandler, std::move(futureObj2));

//    while (true) {
//        log::out << "Main checking if engine is done" << std::endl; log::flush();
//        if (engine.isDone()) {
//            log::out << "Main found engine is done" << std::endl; log::flush();
//            break;
//        }
//        std::this_thread::sleep_for(100ms);
//    }
//
//    log::out << "Engine has ended -- closing." << std::endl; log::flush();

    engineThread.join();
    log::out << "Engine thread closed." << std::endl; log::flush();

    exitSignal1.set_value();
    exitSignal2.set_value();
    //outputHandler.end();
    outputThread.join();
    log::out << "Output thread closed." << std::endl; log::flush();

    //inputHandler.end();
    inputThread.join();
    log::out << "Input thread closed." << std::endl; log::flush();

    log::out << "Goodbye." << std::endl; log::flush();

    return 0;
}

#include <iostream>
#include <thread>
#include <future>
#include "InputHandler.h"
#include "OutputHandler.h"
#include "MessageQueue.h"
#include "Engine.h"
#include "Globals.h"

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
   MAINLOG("Welcome to Sascha!")

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
//        MAINLOG("Main checking if engine is done")
//        if (engine.isDone()) {
//            MAINLOG("Main found engine is done")
//            break;
//        }
//        std::this_thread::sleep_for(100ms);
//    }
//
//    MAINLOG("Engine has ended -- closing.")

    engineThread.join();
    MAINLOG("Engine thread closed.")

    exitSignal1.set_value();
    exitSignal2.set_value();
    //outputHandler.end();
    outputThread.join();
    MAINLOG("Output thread closed.")

    //inputHandler.end();
    inputThread.join();
    MAINLOG("Input thread closed.")

    MAINLOG("Goodbye.")

    return 0;
}

#pragma once

#include <memory>
#include <vector>
#include "Enums.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
    class Options;
namespace Evaluators {

class Evaluator {
public:
    Evaluator(std::shared_ptr<Sascha::Engine::Options> options) : _options(options) { }
    void setBoard(std::shared_ptr<Sascha::Gameplay::Board> board) { _board = board; }
    virtual void calculateBestMove() = 0;
    static std::shared_ptr<Evaluator> createEvaluator(std::shared_ptr<Sascha::Engine::Options> options);
    std::shared_ptr<Gameplay::Move> calculatedMove() const { return _calculatedMove; }
    bool isDone() const { return _isDone; }
protected:
    std::shared_ptr<Sascha::Gameplay::Board> _board;
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> _moves;
    std::shared_ptr<Gameplay::Move> _calculatedMove;
    std::shared_ptr<Sascha::Engine::Options> _options;
    bool _isDone;
};

}
}
}

#pragma once

#include <memory>
#include <vector>


namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

class Evaluator {
public:
    Evaluator() { }
    void setBoard(std::shared_ptr<Sascha::Gameplay::Board> board) { _board = board; }
    virtual void calculateBestMove() = 0;
    static std::shared_ptr<Evaluator> createEvaluator();
    std::shared_ptr<Gameplay::Move> calculatedMove() const { return _calculatedMove; }
    bool isDone() const { return _isDone; }
protected:
    std::shared_ptr<Sascha::Gameplay::Board> _board;
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> _moves;
    std::shared_ptr<Gameplay::Move> _calculatedMove;
    bool _isDone;
};
    
}
}
}

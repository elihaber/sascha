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
    virtual std::shared_ptr<Sascha::Gameplay::Move> getBestMove() = 0;
    static std::shared_ptr<Evaluator> createEvaluator();
protected:
    std::shared_ptr<Sascha::Gameplay::Board> _board;
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> _moves;
};
    
}
}
}

#pragma once

#include <memory>
#include <vector>
#include <map>
#include "Evaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

class TotalPieceValueEvaluator : public Evaluator {
public:
    TotalPieceValueEvaluator() : Evaluator() { }

    virtual int getBestMoveIndex(std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & moves) override;

private:
    std::pair<float, std::shared_ptr<Gameplay::Move>>  _calcBestEval(int numPliesLeft);
    float _evaluateMoveSingle(std::shared_ptr<Gameplay::Move> move);
    void _getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves);
    void _getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves);
    bool _compareFloat(float x, float y, float epsilon = 0.001f);

    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> _currentLine;
};
    
}
}
}
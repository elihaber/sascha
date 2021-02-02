#pragma once

#include <memory>
#include <vector>
#include <map>
#include "Evaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

class ParallelPieceValueEvaluator : public Evaluator {
public:
    ParallelPieceValueEvaluator() : Evaluator() { }

    virtual void calculateBestMove() override;

private:
    std::pair<float, std::shared_ptr<Gameplay::Move>>  _calcBestEval(std::shared_ptr<Gameplay::Board> board, int numPliesLeft);
    float _evaluateMoveSingle(std::shared_ptr<Gameplay::Board> board, std::shared_ptr<Gameplay::Move> move, bool & evaluationCalculatedPossibleMoves);
    void _getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves);
    void _getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves);
    bool _compareFloat(float x, float y, float epsilon = 0.001f);
    void _searchDeeper(std::shared_ptr<Gameplay::Move> move, std::shared_ptr<Gameplay::Board> board);

    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> _currentLine;
    long _totalHandle1Time;
    long _totalHandle2Time;
    long _totalHandle3Time;
    long _totalUndo1Time;
    long _totalUndo2Time;
    long _totalUndo3Time;
    long _totalTime;
};
    
}
}
}

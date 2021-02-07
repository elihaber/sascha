#pragma once

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include "Evaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
    class Options;
namespace Evaluators {

typedef std::vector<std::shared_ptr<Sascha::Gameplay::Move>> MoveVector;
typedef std::pair<float, std::shared_ptr<Sascha::Gameplay::Move>> EvalMovePair;
typedef std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> OrderedEvalMoveMap;
typedef std::unordered_map<std::string, float> UnorderedMoveEvalMap;

class MinimaxMultiThreadEvaluator : public Evaluator {
public:
    MinimaxMultiThreadEvaluator(std::shared_ptr<Sascha::Engine::Options> options) : Evaluator(options) { }

    virtual void calculateBestMove() override;

private:
    void _dummy(std::shared_ptr<Gameplay::Move> move, std::shared_ptr<Sascha::Gameplay::Board> board, int a, OrderedEvalMoveMap & result) {}
    EvalMovePair _calcBestEval(std::shared_ptr<Sascha::Gameplay::Board> board, int numPliesLeft);
    float _evaluateMoveSingle();
    bool _compareFloat(float x, float y, float epsilon = 0.001f);

    EvalMovePair _handleEndNode(std::shared_ptr<Sascha::Gameplay::Board> board, const std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & possibleMoves);
    void _evaluateMoves(std::shared_ptr<Sascha::Gameplay::Board> board, const MoveVector & possibleMoves, OrderedEvalMoveMap & orderedMoves, UnorderedMoveEvalMap & moveAPrioriEvals);
    void _selectNextBatchOfBestMoves(const OrderedEvalMoveMap & orderedMoves, const MoveVector & excludeMoves, MoveVector & selectedMoves);
    bool _getBestEvaluatedMove(const OrderedEvalMoveMap & evaluatedMoves, bool isLevel0, EvalMovePair & bestEvaluatedMove);
    void _searchDeeperWrapper(std::shared_ptr<Gameplay::Move> move, std::shared_ptr<Gameplay::Board> board, int numPliesLeft, OrderedEvalMoveMap & result, MoveVector & testedMoves);
    void _searchDeeper(std::shared_ptr<Gameplay::Move> move, std::shared_ptr<Gameplay::Board> board, int numPliesLeft, OrderedEvalMoveMap & result, MoveVector & testedMoves);
    
    void _addEvaluatedBestMove(EvalMovePair item) {}
    void _addAlreadyTestedMove(std::shared_ptr<Sascha::Gameplay::Move> move) {}

    float _currEval;
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> _currentLine;
    long _totalHandle1Time;
    long _totalHandle2Time;
    long _totalHandle3Time;
    long _totalUndo1Time;
    long _totalUndo2Time;
    long _totalUndo3Time;
    long _totalTime;
    int _searchDepth;
    int _initialMoveCount;
};
    
}
}
}

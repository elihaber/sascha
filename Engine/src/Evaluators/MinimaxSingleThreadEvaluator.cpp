#include <vector>
#include <map>
#include <cmath>
#include <chrono>
#include <sstream>
#include "Engine/Evaluators/MinimaxSingleThreadEvaluator.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"
#include "Engine/Options.h"

namespace Sascha {
namespace Engine {
namespace Evaluators {

using Gameplay::Board;
using Gameplay::Move;

const float HIGHTEST_EVAL = 100000.0;
const float LOWEST_EVAL = -100000.0;

void MinimaxSingleThreadEvaluator::calculateBestMove() {
    _searchDepth =_options->getSearchDepth();
    if (_searchDepth < 2) {
        _searchDepth = 2;
    }
    if (_searchDepth % 2 != 0) {
        --_searchDepth;
    }
    _initialMoveCount =_options->getInitialMoveCount();
    _isDone = false;
    _currentLine.clear();
    _totalHandle1Time = 0;
    _totalHandle2Time = 0;
    _totalHandle3Time = 0;
    _totalUndo1Time = 0;
    _totalUndo2Time = 0;
    _totalUndo3Time = 0;
    _totalTime = 0;

    float _currEval = _evaluateMoveSingle();

    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = _calcBestEval(_searchDepth - 1);
    auto t2 = std::chrono::high_resolution_clock::now();
    _totalTime = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    RECURSIONLOG("_totalHandle1Time " << _totalHandle1Time)
    RECURSIONLOG("_totalHandle2Time " << _totalHandle2Time)
    RECURSIONLOG("_totalHandle3Time " << _totalHandle3Time)
    RECURSIONLOG("_totalUndo1Time " << _totalUndo1Time)
    RECURSIONLOG("_totalUndo2Time " << _totalUndo2Time)
    RECURSIONLOG("_totalUndo3Time " << _totalUndo3Time)
    RECURSIONLOG("_totalTime " << _totalTime)
    MAINLOG("_totalHandle1Time " << _totalHandle1Time)
    MAINLOG("_totalHandle2Time " << _totalHandle2Time)
    MAINLOG("_totalHandle3Time " << _totalHandle3Time)
    MAINLOG("_totalUndo1Time " << _totalUndo1Time)
    MAINLOG("_totalUndo2Time " << _totalUndo2Time)
    MAINLOG("_totalUndo3Time " << _totalUndo3Time)
    MAINLOG("_totalTime " << _totalTime)

    auto moves = _board->getLegalMoves();
    for (size_t i = 0; i < moves.size(); ++i) {
        if (result.second->uciFormat().compare(moves[i]->uciFormat()) == 0) {
            MAINLOG("Best move is " << result.second->algebraicFormat() << " with an evaluation of " << result.first)
            _calculatedMove = moves[i];
            _isDone = true;
            return;
        }
    }
    MAINLOG("The evaluation result " << result.second->algebraicFormat() << " did not match any of the possible moves")
    throw std::runtime_error("The evaluation result did not match any of the possible moves");
}

EvalMovePair MinimaxSingleThreadEvaluator::_handleEndNode(const std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & possibleMoves) {
    float bestEval = (_board->whosTurnToGo() == Color::WHITE ? LOWEST_EVAL : HIGHTEST_EVAL);
    std::shared_ptr<Move> bestMove;

    for (auto & move : possibleMoves) {

        auto t1 = std::chrono::high_resolution_clock::now();
        _board->handleMoveForSingleAnalysis(move);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        _totalHandle1Time += duration;

        float eval = _evaluateMoveSingle();

        t1 = std::chrono::high_resolution_clock::now();
        _board->undoSingleAnalysisMove(move);
        t2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        _totalUndo1Time += duration;

        if (_board->whosTurnToGo() == Color::WHITE) {
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
        else {
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }

    return std::make_pair(bestEval, bestMove);
}

void MinimaxSingleThreadEvaluator::_evaluateMoves(const MoveVector & possibleMoves, OrderedEvalMoveMap & orderedMoves, UnorderedMoveEvalMap & moveAPrioriEvals) {
    for (auto & move : possibleMoves) {

        auto t1 = std::chrono::high_resolution_clock::now();
        _board->handleMoveForSingleAnalysis(move);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        _totalHandle2Time += duration;

        float eval = _evaluateMoveSingle();
        orderedMoves.insert(std::make_pair(eval, move));
        moveAPrioriEvals.insert(std::make_pair(move->algebraicFormat(), eval));

        t1 = std::chrono::high_resolution_clock::now();
        _board->undoSingleAnalysisMove(move);
        t2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        _totalUndo2Time += duration;
    }
}

void MinimaxSingleThreadEvaluator::_selectNextBatchOfBestMoves(const OrderedEvalMoveMap & orderedMoves, const MoveVector & excludeMoves, MoveVector & selectedMoves) {
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> stagingArea;
 
    if (orderedMoves.empty()) {
        return;
    }

    int numMovesStillNeeded = (_initialMoveCount != 0 ? _initialMoveCount : orderedMoves.size());

    // Get some information from the first move.
    Color colorMoving = orderedMoves.begin()->second->color();

    // Prepare the order we will loop through the moves.
    // The moves are ordered from lowest eval to highest, so if the player is white we want to start at the
    // end of the map, and if the player is black we want to start at the start of the map.
    auto iteratorStart = (colorMoving == Color::WHITE ? orderedMoves.end() : orderedMoves.begin());
    auto iteratorEnd = (colorMoving == Color::WHITE ? orderedMoves.begin() : orderedMoves.end());

    float prevVal = 0.0;
    bool addedOneToStagingArea = false;

    // TODO: There has GOT to be a better way to do this.
    auto & iter = iteratorStart;
    do {
        if (colorMoving == Color::WHITE) --iter;

        bool excludeThisMove = false;
        for (auto testedMove : excludeMoves) { // TODO: Would this be quicker if the vector was actually an unordered set of hashes, then we could use "find"?
            if (*testedMove == *iter->second) {
                excludeThisMove = true;
                break;
            }
        }
        if (excludeThisMove) {
            if (colorMoving == Color::BLACK) ++iter;
            continue;
        }
        if (!addedOneToStagingArea) {
            // This is the first non-excluded move we found, so add it to the staging area.
            stagingArea.push_back(iter->second);
            prevVal = iter->first;
            addedOneToStagingArea = true;
            if (colorMoving == Color::BLACK) ++iter;
            continue;
        }
        // If we are here, we have a non-excluded move, and we also have moves in the staging area.
        if (_compareFloat(iter->first, prevVal)) {
            // The current move has the same value as moves in the staging area, so add it also.
            stagingArea.push_back(iter->second);
            if (colorMoving == Color::BLACK) ++iter;
            continue;
        }
        else {
            // The current move has a different value from moves in the staging area, so handle the staging area, clear it, and then add this move to it.
            if (stagingArea.size() >= numMovesStillNeeded) {
                // The staging area has all the moves we need to finish the process, so choose random
                // moves from the staging area until we have all we need.
                for (int i = 0; i < numMovesStillNeeded; ++i) {
                    int index = rand() % stagingArea.size();
                    selectedMoves.push_back(stagingArea[index]);
                    stagingArea.erase(stagingArea.begin() + index);
                }
                break;
            }
            else {
                // The staging area is does not have enough moves to supply us with all we need, so add them all in random
                // order to our final move list, then clear the staging area and add into it the current move we are working on.

                std::vector<bool> indexesUsed(stagingArea.size(), false);
                for (int i = 0; i < stagingArea.size(); ++i) {
                    int nextIndex = -1;
                    while (nextIndex == -1) {
                        int index = rand() % stagingArea.size();
                        if (!indexesUsed[index]) {
                            indexesUsed[index] = true;
                            nextIndex = index;
                            break;
                        }
                    }
                    selectedMoves.push_back(stagingArea[nextIndex]);
                    --numMovesStillNeeded;
                }
                stagingArea.clear();
                stagingArea.push_back(iter->second);
                prevVal = iter->first;
            }
        }

        if (colorMoving == Color::BLACK) ++iter;
    } while (iter != iteratorEnd);

    if (numMovesStillNeeded > 0) {
        // After looping through all the possible moves, we still haven't filled our quota.
        // But the staging area is certain to have at lease one move left in it, and maybe more.
        // So choose random moves from the staging area until we have all we need or the
        // staging area is empty.
        for (int i = 0; i < numMovesStillNeeded; ++i) {
            if (stagingArea.empty()) {
                break;
            }
            int index = rand() % stagingArea.size();
            selectedMoves.push_back(stagingArea[index]);
            stagingArea.erase(stagingArea.begin() + index);
        }
    }
}

bool MinimaxSingleThreadEvaluator::_getBestEvaluatedMove(const OrderedEvalMoveMap & evaluatedMoves, bool isLevel0, EvalMovePair & bestEvaluatedMove) {
    bool needMoreMoves = false;
    std::pair<float, std::shared_ptr<Move>> result;

    Color colorMoving = evaluatedMoves.begin()->second->color(); // This is safe because evaluatedMoves cannot be empty

    // Prepare the order we will loop through the moves.
    // The moves are ordered from lowest eval to highest, so if the player is white we want to start at the
    // end of the map, and if the player is black we want to start at the start of the map.
//    auto iteratorStart = (colorMoving == Color::WHITE ? std::prev(evaluatedMoves.end()) : evaluatedMoves.begin());
//    auto iteratorEnd = (colorMoving == Color::WHITE ? evaluatedMoves.begin() : std::prev(evaluatedMoves.end()));
    auto iteratorStart = (colorMoving == Color::WHITE ? evaluatedMoves.end() : evaluatedMoves.begin());
    auto iteratorEnd = (colorMoving == Color::WHITE ? evaluatedMoves.begin() : evaluatedMoves.end());

    std::vector<std::shared_ptr<Move>> choiceMoves;
    float finalEval = 0.0;
    bool firstItem = true;
    auto & iter = iteratorStart;
    do {
        if (colorMoving == Color::WHITE) --iter;

//    for (auto & iter = iteratorStart; iter != iteratorEnd; (colorMoving == Color::WHITE ? --iter : ++iter)) {
        if (firstItem) {
            firstItem = false;
            choiceMoves.push_back(iter->second);
            finalEval = iter->first;
            if (isLevel0 && ((colorMoving == Color::WHITE && finalEval < _currEval) || (colorMoving == Color::BLACK && finalEval > _currEval))) {
                // The best evaluated move is worse than the current position evaluation, so tell the algorithm
                // that we need to spend some time to try to find some better moves.
                needMoreMoves = true;
            }
            if (colorMoving == Color::BLACK) ++iter;
            continue;
        }
        if (_compareFloat(iter->first, finalEval)) {
            choiceMoves.push_back(iter->second);
        }
        else {
            break;
        }

        if (colorMoving == Color::BLACK) ++iter;
    } while (iter != iteratorEnd);
//    }

    int index = rand() % choiceMoves.size();
    bestEvaluatedMove = std::make_pair(finalEval, choiceMoves[index]);

    return needMoreMoves;
}

EvalMovePair MinimaxSingleThreadEvaluator::_calcBestEval(int numPliesLeft) {

    std::stringstream logPrefix;
    for (size_t i = 0; i < _searchDepth - 1 - numPliesLeft; ++i) {
        logPrefix << "    ";
    }

    auto possibleMoves = _board->getLegalMoves();

    // Set the best eval to a bad value so that any value we get from a move will beat it.
    float bestEval = (_board->whosTurnToGo() == Color::WHITE ? LOWEST_EVAL : HIGHTEST_EVAL);

    if (numPliesLeft == 0) {
        // If we are here then we have reached the deepest level we will analyze, so all we can do
        // is test out each move and do an immediate evaluation of the result -- we can't go any
        // deeper.

        auto bestEvalAndMove = _handleEndNode(possibleMoves);

        RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestEvalAndMove.second->color()) << " Single evaluation for white -- Move: " << bestEvalAndMove.second->algebraicFormat() << " Eval: " << bestEvalAndMove.first)
        MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestEvalAndMove.second->color()) << " Single evaluation for white -- Move: " << bestEvalAndMove.second->algebraicFormat() << " Eval: " << bestEvalAndMove.first)

        return bestEvalAndMove;
    }
    else {
        // First, do a quick one-move test on all the moves, to get their immediate evaluation without
        // considering any response from the other player.

        OrderedEvalMoveMap orderedMoves;
        UnorderedMoveEvalMap moveAPrioriEvals;
        _evaluateMoves(possibleMoves, orderedMoves, moveAPrioriEvals);

        OrderedEvalMoveMap evaluatedBestMoves;
        MoveVector alreadyTestedMoves;

        while (true) {

            MoveVector bestMoves;
            _selectNextBatchOfBestMoves(orderedMoves, alreadyTestedMoves, bestMoves);

            if (!bestMoves.empty()) {
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[0]->color()) << " Out of " << orderedMoves.size() << " candidate moves, selected " << bestMoves.size())
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[0]->color()) << " Out of " << orderedMoves.size() << " candidate moves, selected " << bestMoves.size())
            }
            else if (!orderedMoves.empty()) {
                // This should never happen
                MAINLOG("ERROR: Ordered moves is not empty but best moves is")
                throw std::runtime_error("ERROR: Ordered moves is not empty but best moves is");
            }
            else {
                if (_board->isCheck()) {
                    // Checkmate
                    if (_board->whosTurnToGo() == Color::WHITE) {
                        return std::make_pair(LOWEST_EVAL, nullptr);
                    }
                    else {
                        return std::make_pair(HIGHTEST_EVAL, nullptr);
                    }
                }
                else {
                    // Stalemate
                    return std::make_pair(0.0, nullptr);
                }
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " No candidate moves")
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " No candidate moves")
            }
            
            // Note: bestMoves cannot be empty at this point

            for (int i = 0; i < bestMoves.size(); ++i) {
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " A Priory Eval: " << moveAPrioriEvals[bestMoves[i]->algebraicFormat()])
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " A Priory Eval: " << moveAPrioriEvals[bestMoves[i]->algebraicFormat()])

                auto t1 = std::chrono::high_resolution_clock::now();
                _board->handleMoveForFullAnalysis(bestMoves[i]);
                auto t2 = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
                _totalHandle3Time += duration;

                _currentLine.push_back(bestMoves[i]);
                float moveEval = _calcBestEval(numPliesLeft - 1).first;

                t1 = std::chrono::high_resolution_clock::now();
                _board->undoFullAnalysisMove(bestMoves[i]);
                t2 = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
                _totalUndo3Time += duration;

                _currentLine.pop_back();
                evaluatedBestMoves.insert(std::make_pair(moveEval, bestMoves[i]));
                alreadyTestedMoves.push_back(bestMoves[i]);
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Finished testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " Eval: " << moveEval)
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Finished testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " Eval: " << moveEval)
            }

            // Note: evaluatedBestMoves cannot be empty at this point

            EvalMovePair bestEvaluatedMove;
            bool needMoreMoves = _getBestEvaluatedMove(evaluatedBestMoves, (_searchDepth - 1 - numPliesLeft == 0), bestEvaluatedMove);

            if (needMoreMoves && alreadyTestedMoves.size() < orderedMoves.size()) {
                continue;
            }

            if (_searchDepth - 1 - numPliesLeft == 0) {
                RECURSIONLOG("Returning move " << bestEvaluatedMove.second->algebraicFormat() << " with eval " << bestEvaluatedMove.first << " (current eval is " << _currEval << ") Tested " << alreadyTestedMoves.size() << " moves out of " << orderedMoves.size() << " possible moves")
                MAINLOG("Returning move " << bestEvaluatedMove.second->algebraicFormat() << " with eval " << bestEvaluatedMove.first << " (current eval is " << _currEval << ") Tested " << alreadyTestedMoves.size() << " moves out of " << orderedMoves.size() << " possible moves")
            }

            return bestEvaluatedMove;
        }
    }
}

float MinimaxSingleThreadEvaluator::_evaluateMoveSingle() {
    float eval = 0.0;
    auto pieces = _board->pieces();
    for (auto & piece : pieces) {
        float factor = (piece->color() == Color::WHITE ? 1.0 : -1.0);
        if (piece->pieceType() == PieceType::BLANK) {
            continue;
        }
        if (piece->pieceType() == PieceType::PAWN) {
            eval += (1.0 * factor);
        }
        else if (piece->pieceType() == PieceType::KNIGHT) {
            eval += (3.0 * factor);
        }
        else if (piece->pieceType() == PieceType::BISHOP) {
            eval += (3.0 * factor);
        }
        else if (piece->pieceType() == PieceType::ROOK) {
            eval += (5.0 * factor);
        }
        else if (piece->pieceType() == PieceType::QUEEN) {
            eval += (9.0 * factor);
        }
    }
    if (_board->isCheck()) {
        if (_board->isCheckmate()) {
            if (_board->whosTurnToGo() == Color::WHITE) {
                eval = 100000.0;
            }
            else {
                eval = -100000.0;
            }
        }
    }
    else if (_board->isStalemate()) {
        eval = 0.0;
    }
    return eval;
}

bool MinimaxSingleThreadEvaluator::_compareFloat(float x, float y, float epsilon) {
   if(fabs(x - y) < epsilon) {
      return true;
   }
   return false;
}

}
}
}

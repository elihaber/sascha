#include <vector>
#include <map>
#include <cmath>
#include <chrono>
#include <algorithm>
#include <functional>
#include "Engine/Evaluators/ParallelPieceValueEvaluator.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"

#define RECURSION_LEVEL 3
#define NUM_POSSIBLE_MOVES 4

namespace Sascha {
namespace Engine {
namespace Evaluators {

using Gameplay::Board;
using Gameplay::Move;

void ParallelPieceValueEvaluator::calculateBestMove() {
    _isDone = false;
    _currentLine.clear();
    _totalHandle1Time = 0;
    _totalHandle2Time = 0;
    _totalHandle3Time = 0;
    _totalUndo1Time = 0;
    _totalUndo2Time = 0;
    _totalUndo3Time = 0;
    _totalTime = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = _calcBestEval(_board, RECURSION_LEVEL);
    auto t2 = std::chrono::high_resolution_clock::now();
    _totalTime = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    RECURSIONLOG("_totalHandle1Time " << _totalHandle1Time)
    RECURSIONLOG("_totalHandle2Time " << _totalHandle2Time)
    RECURSIONLOG("_totalHandle3Time " << _totalHandle3Time)
    RECURSIONLOG("_totalUndo1Time " << _totalUndo1Time)
    RECURSIONLOG("_totalUndo2Time " << _totalUndo2Time)
    RECURSIONLOG("_totalUndo3Time " << _totalUndo3Time)
    RECURSIONLOG("_totalTime " << _totalTime)

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

void ParallelPieceValueEvaluator::_searchDeeper(std::shared_ptr<Gameplay::Move> move, std::shared_ptr<Gameplay::Board> board) {

}

std::pair<float, std::shared_ptr<Move>> ParallelPieceValueEvaluator::_calcBestEval(std::shared_ptr<Board> board, int numPliesLeft) {
    std::shared_ptr<Board> localBoard(board->clone());

    std::string logPrefix;
    for (size_t i = 0; i < RECURSION_LEVEL - numPliesLeft; ++i) {
        logPrefix += "    ";
    }

    auto possibleMoves = localBoard->getLegalMoves();

    float bestEval = -100000.0;
    if (_board->whosTurnToGo() == Color::BLACK) {
        bestEval = 100000.0;
    }

    if (numPliesLeft == 0) {
        std::shared_ptr<Move> bestMove;
        for (auto move : possibleMoves) {

            localBoard->handleMoveForSingleAnalysis(move);

            bool evaluationCalculatedPossibleMoves;
            float eval = _evaluateMoveSingle(localBoard, move, evaluationCalculatedPossibleMoves);

            if (evaluationCalculatedPossibleMoves) {
                localBoard->undoFullAnalysisMove(move);
            }
            else {
                localBoard->undoSingleAnalysisMove(move);
            }

            if (localBoard->whosTurnToGo() == Color::WHITE) {
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

        RECURSIONLOG(logPrefix << "Level: " << (RECURSION_LEVEL - numPliesLeft) << " Player: " << colorToString(bestMove->color()) << " Single evaluation for white -- Move: " << bestMove->algebraicFormat() << " Eval: " << bestEval)

        return std::make_pair(bestEval, bestMove);
    }
    else {
        // Test every possible move and put them in an order list according to eval.
        // for best NUM_POSSIBLE_MOVES:
        std::multimap<float, std::shared_ptr<Move>> orderedMoves;
        std::map<std::string, float> moveAPrioriEvals;

        auto f = std::bind(&ParallelPieceValueEvaluator::_searchDeeper, this, std::placeholders::_1, localBoard);
        for_each(possibleMoves.begin(), possibleMoves.end(), std::bind(&ParallelPieceValueEvaluator::_searchDeeper, this, std::placeholders::_1, localBoard));

        for (auto move : possibleMoves) {
        
            localBoard->handleMoveForSingleAnalysis(move);

            bool evaluationCalculatedPossibleMoves;
            float eval = _evaluateMoveSingle(localBoard, move, evaluationCalculatedPossibleMoves);
            orderedMoves.insert(std::make_pair(eval, move));
            moveAPrioriEvals.insert(std::make_pair(move->algebraicFormat(), eval));

            if (evaluationCalculatedPossibleMoves) {
                localBoard->undoFullAnalysisMove(move);
            }
            else {
                localBoard->undoSingleAnalysisMove(move);
            }
        }

        // Do this better because if some of the values will be the same, then the order will be static.
        std::vector<std::shared_ptr<Move>> bestMoves;
        std::vector<float> bestEvals;
        int numRequired = NUM_POSSIBLE_MOVES;
        if (localBoard->whosTurnToGo() == Color::WHITE) {
            _getBestMovesFromSortedMap(numRequired, orderedMoves, orderedMoves.rbegin(), orderedMoves.rend(), bestMoves);
        }
        else {
            _getBestMovesFromSortedMap(numRequired, orderedMoves, orderedMoves.begin(), orderedMoves.end(), bestMoves);
        }

        if (!bestMoves.empty()) {
            RECURSIONLOG(logPrefix << "Level: " << (RECURSION_LEVEL - numPliesLeft) << " Player: " << colorToString(bestMoves[0]->color()) << " Out of " << orderedMoves.size() << " candidate moves, selected " << bestMoves.size())
        }
        else if (!orderedMoves.empty()) {
            RECURSIONLOG(logPrefix << "Level: " << (RECURSION_LEVEL - numPliesLeft) << " Failed to select any candidate moves")
        }
        else {
            if (localBoard->isCheck()) {
                // Checkmate
                if (localBoard->whosTurnToGo() == Color::WHITE) {
                    return std::make_pair(-100000.0, nullptr);
                }
                else {
                    return std::make_pair(100000.0, nullptr);
                }
            }
            else {
                // Stalemate
                return std::make_pair(0.0, nullptr);
            }
            RECURSIONLOG(logPrefix << "Level: " << (RECURSION_LEVEL - numPliesLeft) << " No candidate moves")
        }
        
        std::multimap<float, std::shared_ptr<Move>> evaluatedBestMoves;
        for (int i = 0; i < bestMoves.size(); ++i) {
            RECURSIONLOG(logPrefix << "Level: " << (RECURSION_LEVEL - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " A Priory Eval: " << moveAPrioriEvals[bestMoves[i]->algebraicFormat()])

            localBoard->handleMoveForFullAnalysis(bestMoves[i]);

            _currentLine.push_back(bestMoves[i]);
            float moveEval = _calcBestEval(localBoard, numPliesLeft - 1).first;

            localBoard->undoFullAnalysisMove(bestMoves[i]);

            _currentLine.pop_back();
            evaluatedBestMoves.insert(std::make_pair(moveEval, bestMoves[i]));
            RECURSIONLOG(logPrefix << "Level: " << (RECURSION_LEVEL - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Finished testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " Eval: " << moveEval)
        }
        if (localBoard->whosTurnToGo() == Color::WHITE) {
            std::vector<std::shared_ptr<Move>> choiceMoves;
            float finalEval = 0.0;
            for (auto iter = evaluatedBestMoves.rbegin(); iter != evaluatedBestMoves.rend(); ++iter) {
                if (iter == evaluatedBestMoves.rbegin()) {
                    choiceMoves.push_back(iter->second);
                    finalEval = iter->first;
                    continue;
                }
                if (_compareFloat(iter->first, finalEval)) {
                    choiceMoves.push_back(iter->second);
                }
                else {
                    break;
                }
            }
            int index = rand() % choiceMoves.size();
            return std::make_pair(finalEval, choiceMoves[index]);
        } else {
            std::vector<std::shared_ptr<Move>> choiceMoves;
            float finalEval = 0.0;
            for (auto iter = evaluatedBestMoves.begin(); iter != evaluatedBestMoves.end(); ++iter) {
                if (iter == evaluatedBestMoves.begin()) {
                    choiceMoves.push_back(iter->second);
                    finalEval = iter->first;
                    continue;
                }
                if (_compareFloat(iter->first, finalEval)) {
                    choiceMoves.push_back(iter->second);
                }
                else {
                    break;
                }
            }
            int index = rand() % choiceMoves.size();
            return std::make_pair(finalEval, choiceMoves[index]);
        }
    }
}

void ParallelPieceValueEvaluator::_getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves) {
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> stagingArea;

//    RECURSIONLOG("================ Selecting " << numMoves << " moves from " << orderedMoves.size() << " candidates")
    int numMovesStillNeeded = numMoves;
    float prevVal = 0.0;
    for (auto iter = start; iter != end; ++iter) {
        if (iter == start) {
            stagingArea.push_back(iter->second);
            prevVal = iter->first;
//            RECURSIONLOG("Moved first move into staging area with eval of " << prevVal << " staging area size is now " << stagingArea.size())
            continue;
        }
        if (_compareFloat(iter->first, prevVal)) {
            stagingArea.push_back(iter->second);
//            RECURSIONLOG("Moved another move into staging area with eval of " << iter->first << " staging area size is now " << stagingArea.size())
            continue;
        }
        else {
//            RECURSIONLOG("Need to clear out staging area because next move eval is " << iter->first)
            if (stagingArea.size() >= numMovesStillNeeded) {
//                RECURSIONLOG("Staging area has " << stagingArea.size() << " moves which is enough moves to complete our requirement of " << numMovesStillNeeded)
                // Move random from staging area to vector.
                for (int i = 0; i < numMovesStillNeeded; ++i) {
                    int index = rand() % stagingArea.size();
//                    RECURSIONLOG("Selected index " << index)
                    bestMoves.push_back(stagingArea[index]);
                    stagingArea.erase(stagingArea.begin() + index);
                }
                break;
            }
            else {
//                RECURSIONLOG("Staging area has " << stagingArea.size() << " moves which is not enough moves to complete our requirement of " << numMovesStillNeeded)
                for (auto move : stagingArea) {
                    bestMoves.push_back(move);
                    --numMovesStillNeeded;
                }
                stagingArea.clear();
                stagingArea.push_back(iter->second);
                prevVal = iter->first;
//                RECURSIONLOG("Added latest move into staging area with eval of " << prevVal << " staging area size is now " << stagingArea.size())
            }
        }
    }
    if (numMovesStillNeeded > 0) {
//        RECURSIONLOG("We still require " << numMovesStillNeeded << " moves and the staging area has " << stagingArea.size() << " moves")
        for (int i = 0; i < numMovesStillNeeded; ++i) {
            if (stagingArea.empty()) {
                break;
            }
            int index = rand() % stagingArea.size();
//            RECURSIONLOG("Selected index " << index)
            bestMoves.push_back(stagingArea[index]);
            stagingArea.erase(stagingArea.begin() + index);
        }
    }
//    RECURSIONLOG("================")
}

void ParallelPieceValueEvaluator::_getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves) {
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> stagingArea;

    int numMovesStillNeeded = numMoves;
    float prevVal = 0.0;
    for (auto iter = start; iter != end; ++iter) {
        if (iter == start) {
            stagingArea.push_back(iter->second);
            prevVal = iter->first;
            continue;
        }
        if (iter->first == prevVal) {
            stagingArea.push_back(iter->second);
            continue;
        }
        else {
            if (stagingArea.size() >= numMovesStillNeeded) {
                // Move random from staging area to vector.
                for (int i = 0; i < numMovesStillNeeded; ++i) {
                    int index = rand() % stagingArea.size();
                    bestMoves.push_back(stagingArea[index]);
                    stagingArea.erase(stagingArea.begin() + index);
                }
                break;
            }
            else {
                for (auto move : stagingArea) {
                    bestMoves.push_back(move);
                    --numMovesStillNeeded;
                }
                stagingArea.clear();
                stagingArea.push_back(iter->second);
                prevVal = iter->first;
            }
        }
    }
    if (numMovesStillNeeded > 0) {
        for (int i = 0; i < numMovesStillNeeded; ++i) {
            if (stagingArea.empty()) {
                break;
            }
            int index = rand() % stagingArea.size();
            bestMoves.push_back(stagingArea[index]);
            stagingArea.erase(stagingArea.begin() + index);
        }
    }
}

float ParallelPieceValueEvaluator::_evaluateMoveSingle(std::shared_ptr<Board> board, std::shared_ptr<Move> move, bool & evaluationCalculatedPossibleMoves) {
    evaluationCalculatedPossibleMoves = false;
    float eval = 0.0;
    auto pieces = board->pieces();
    for (auto piece : pieces) {
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
    if (board->isCheck()) {
        board->_calculateLegalMoves();
        evaluationCalculatedPossibleMoves = true;
        if (board->isCheckmate()) {
            if (board->whosTurnToGo() == Color::WHITE) {
                eval = 100000.0;
            }
            else {
                eval = -100000.0;
            }
        }
    }
    else if (board->isStalemate()) {
        eval = 0.0;
    }
    return eval;
}

bool ParallelPieceValueEvaluator::_compareFloat(float x, float y, float epsilon) {
   if(fabs(x - y) < epsilon) {
      return true;
   }
   return false;
}

}
}
}

#include <vector>
#include <map>
#include <cmath>
#include <chrono>
#include <sstream>
#include "Engine/Evaluators/TotalPieceValueEvaluator.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"
#include "Engine/Options.h"

namespace Sascha {
namespace Engine {
namespace Evaluators {

using Gameplay::Board;
using Gameplay::Move;

void TotalPieceValueEvaluator::calculateBestMove() {
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

std::pair<float, std::shared_ptr<Move>> TotalPieceValueEvaluator::_calcBestEval(int numPliesLeft) {

    std::stringstream logPrefix;
    for (size_t i = 0; i < _searchDepth - 1 - numPliesLeft; ++i) {
        logPrefix << "    ";
    }

    auto possibleMoves = _board->getLegalMoves();

    float bestEval = -100000.0;
    if (_board->whosTurnToGo() == Color::BLACK) {
        bestEval = 100000.0;
    }

    if (numPliesLeft == 0) {
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

        RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMove->color()) << " Single evaluation for white -- Move: " << bestMove->algebraicFormat() << " Eval: " << bestEval)
        MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMove->color()) << " Single evaluation for white -- Move: " << bestMove->algebraicFormat() << " Eval: " << bestEval)

        return std::make_pair(bestEval, bestMove);
    }
    else {
        // Test every possible move and put them in an order list according to eval.
        // for best _initialMoveCount:
        std::multimap<float, std::shared_ptr<Move>> orderedMoves;
        std::map<std::string, float> moveAPrioriEvals;
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

        std::multimap<float, std::shared_ptr<Move>> evaluatedBestMoves;
        std::vector<std::shared_ptr<Move>> alreadyTestedMoves;

        while (true) {

            // Do this better because if some of the values will be the same, then the order will be static.
            std::vector<std::shared_ptr<Move>> bestMoves;
            std::vector<float> bestEvals;
            int numRequired = (_initialMoveCount != 0 ? _initialMoveCount : orderedMoves.size());
            if (_board->whosTurnToGo() == Color::WHITE) {
                _getBestMovesFromSortedMap(_searchDepth - 1 - numPliesLeft, numRequired, orderedMoves, orderedMoves.rbegin(), orderedMoves.rend(), alreadyTestedMoves, bestMoves);
            }
            else {
                _getBestMovesFromSortedMap(_searchDepth - 1 - numPliesLeft, numRequired, orderedMoves, orderedMoves.begin(), orderedMoves.end(), alreadyTestedMoves, bestMoves);
            }

            if (!bestMoves.empty()) {
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[0]->color()) << " Out of " << orderedMoves.size() << " candidate moves, selected " << bestMoves.size())
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[0]->color()) << " Out of " << orderedMoves.size() << " candidate moves, selected " << bestMoves.size())
            }
            else if (!orderedMoves.empty()) {
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Failed to select any candidate moves")
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Failed to select any candidate moves")
            }
            else {
                if (_board->isCheck()) {
                    // Checkmate
                    if (_board->whosTurnToGo() == Color::WHITE) {
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
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " No candidate moves")
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " No candidate moves")
            }
            
    //        MAINLOG("Level: " << (_searchDepth - 1 - numPliesLeft))
    //        MAINLOG_NNL(" Current line:")
    //        for (int i = 0; i < _currentLine.size(); ++i) {
    //            MAINLOG_NNL(" " << _currentLine[i]->algebraicFormat())
    //        }
    //        MAINLOG_NNL(" Candidate moves:")
    //        for (int i = 0; i < bestMoves.size(); ++i) {
    //            MAINLOG_NNL(" " << bestMoves[i]->algebraicFormat())
    //        }
    //        MAINLOG("")

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
                RECURSIONLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Finished testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " Eval: " << moveEval)
                MAINLOG(logPrefix.str() << "Level: " << (_searchDepth - 1 - numPliesLeft) << " Player: " << colorToString(bestMoves[i]->color()) << " Finished testing move " << i << " out of " << bestMoves.size() << " -- Move: " << bestMoves[i]->algebraicFormat() << " Eval: " << moveEval)
            }
            for (auto & move : bestMoves) {
                alreadyTestedMoves.push_back(move);
            }
            bool needMoreMoves = false;
            std::pair<float, std::shared_ptr<Move>> result;
            if (_board->whosTurnToGo() == Color::WHITE) {
                std::vector<std::shared_ptr<Move>> choiceMoves;
                float finalEval = 0.0;
                for (auto iter = evaluatedBestMoves.rbegin(); iter != evaluatedBestMoves.rend(); ++iter) {
                    if (iter == evaluatedBestMoves.rbegin()) {
                        choiceMoves.push_back(iter->second);
                        finalEval = iter->first;
                        if (_searchDepth - 1 - numPliesLeft == 0 && finalEval < _currEval) {
                            needMoreMoves = true;
                            break;
                        }
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
                //return std::make_pair(finalEval, choiceMoves[index]);
                result = std::make_pair(finalEval, choiceMoves[index]);
            } else {
                std::vector<std::shared_ptr<Move>> choiceMoves;
                float finalEval = 0.0;
                for (auto iter = evaluatedBestMoves.begin(); iter != evaluatedBestMoves.end(); ++iter) {
                    if (iter == evaluatedBestMoves.begin()) {
                        choiceMoves.push_back(iter->second);
                        finalEval = iter->first;
                        if (_searchDepth - 1 - numPliesLeft == 0 && finalEval > _currEval) {
                            needMoreMoves = true;
                            break;
                        }
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
                //return std::make_pair(finalEval, choiceMoves[index]);
                result = std::make_pair(finalEval, choiceMoves[index]);
            }
            if (_searchDepth - 1 - numPliesLeft == 0 && needMoreMoves && alreadyTestedMoves.size() < orderedMoves.size()) {
                continue;
            }
            if (_searchDepth - 1 - numPliesLeft == 0) { RECURSIONLOG("Returning move " << result.second->algebraicFormat() << " with eval " << result.first << " (current eval is " << _currEval << ") Tested " << alreadyTestedMoves.size() << " moves out of " << orderedMoves.size() << " possible moves") MAINLOG("Returning move " << result.second->algebraicFormat() << " with eval " << result.first << " (current eval is " << _currEval << ") Tested " << alreadyTestedMoves.size() << " moves out of " << orderedMoves.size() << " possible moves") }
            return result;
        }
    }
}

void TotalPieceValueEvaluator::_getBestMovesFromSortedMap(int level, int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & testedMoves, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves) {
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> stagingArea;

//    RECURSIONLOG("================ Selecting " << numMoves << " moves from " << orderedMoves.size() << " candidates")
    int numMovesStillNeeded = numMoves;
    float prevVal = 0.0;
    bool addedOneToStagingArea = false;
    for (auto & iter = start; iter != end; ++iter) {
        bool alreadyTested = false;
        if (level == 0) {
            for (auto testedMove : testedMoves) {
                if (*testedMove == *iter->second) {
                    alreadyTested = true;
                    break;
                }
            }
        }
        if (alreadyTested) {
            continue;
        }
        if (!addedOneToStagingArea) {
            stagingArea.push_back(iter->second);
            prevVal = iter->first;
            addedOneToStagingArea = true;
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
}

void TotalPieceValueEvaluator::_getBestMovesFromSortedMap(int level, int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & testedMoves, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves) {
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> stagingArea;

    int numMovesStillNeeded = numMoves;
    float prevVal = 0.0;
    bool addedOneToStagingArea = false;
    for (auto & iter = start; iter != end; ++iter) {
        bool alreadyTested = false;
        if (level == 0) {
            for (auto testedMove : testedMoves) {
                if (*testedMove == *iter->second) {
                    alreadyTested = true;
                    break;
                }
            }
        }
        if (alreadyTested) {
            continue;
        }
        if (!addedOneToStagingArea) {
            stagingArea.push_back(iter->second);
            prevVal = iter->first;
            addedOneToStagingArea = true;
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

float TotalPieceValueEvaluator::_evaluateMoveSingle() {
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

bool TotalPieceValueEvaluator::_compareFloat(float x, float y, float epsilon) {
   if(fabs(x - y) < epsilon) {
      return true;
   }
   return false;
}

}
}
}

#include <vector>
#include <map>
#include <cmath>
#include "Engine/Evaluators/TotalPieceValueEvaluator.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"

namespace Sascha {
namespace Engine {
namespace Evaluators {

using Gameplay::Board;
using Gameplay::Move;

int TotalPieceValueEvaluator::getBestMoveIndex(std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & moves) {
    _currentLine.clear();
    auto result = _calcBestEval(5);
    for (size_t i = 0; i < moves.size(); ++i) {
        if (result.second->algebraicNotation().compare(moves[i]->algebraicNotation()) == 0) {
            MAINLOG("Best move is " << result.second->algebraicNotation() << " with an evaluation of " << result.first)
            return i;
        }
    }
    MAINLOG("The evaluation result " << result.second->algebraicNotation() << " did not match any of the possible moves")
    throw std::runtime_error("The evaluation result did not match any of the possible moves");
}

std::pair<float, std::shared_ptr<Move>> TotalPieceValueEvaluator::_calcBestEval(int numPliesLeft) {
    std::vector<std::shared_ptr<Move>> possibleMoves;
    _board->getPossibleMoves(possibleMoves);

    float bestEval = -100000.0;
    if (_board->whosTurnToGo() == Color::BLACK) {
        bestEval = 100000.0;
    }

    if (numPliesLeft == 0) {
        MAINLOG("Level: " << (5 - numPliesLeft) << " Doinng single-move evaluations")
        std::shared_ptr<Move> bestMove;
        for (auto move : possibleMoves) {
            _board->handleMove(move);
            float eval = _evaluateMoveSingle(move);
            _board->undoMove(move);
            if (_board->whosTurnToGo() == Color::WHITE) {
                if (eval > bestEval) {
                    bestEval = eval;
                    bestMove = move;
                    MAINLOG("Level: " << (5 - numPliesLeft) << " Single evaluation for white -- Move: " << move->algebraicNotation() << " Eval: " << eval)
                }
            }
            else {
                if (eval < bestEval) {
                    eval = bestEval;
                    bestMove = move;
                    MAINLOG("Level: " << (5 - numPliesLeft) << " Single evaluation for black -- Move: " << move->algebraicNotation() << " Eval: " << eval)
                }
            }
        }

        return std::make_pair(bestEval, bestMove);
    }
    else {
        // Test every possible move and put them in an order list according to eval.
        // for best 5:
        std::multimap<float, std::shared_ptr<Move>> orderedMoves;
        for (auto move : possibleMoves) {
            _board->handleMove(move);
            orderedMoves.insert(std::make_pair(_evaluateMoveSingle(move), move));
            _board->undoMove(move);
        }

        // Do this better because if some of the values will be the same, then the order will be static.
        std::vector<std::shared_ptr<Move>> bestMoves;
        std::vector<float> bestEvals;
        int numRequired = 5;
        if (_board->whosTurnToGo() == Color::WHITE) {
            _getBestMovesFromSortedMap(numRequired, orderedMoves, orderedMoves.rbegin(), orderedMoves.rend(), bestMoves);
        }
        else {
            _getBestMovesFromSortedMap(numRequired, orderedMoves, orderedMoves.begin(), orderedMoves.end(), bestMoves);
        }

        MAINLOG("Level: " << (5 - numPliesLeft))
        MAINLOG_NNL(" Current line:")
        for (int i = 0; i < _currentLine.size(); ++i) {
            MAINLOG_NNL(" " << _currentLine[i]->algebraicNotation())
        }
        MAINLOG_NNL(" Candidate moves:")
        for (int i = 0; i < bestMoves.size(); ++i) {
            MAINLOG_NNL(" " << bestMoves[i]->algebraicNotation())
        }
        MAINLOG("")

        std::multimap<float, std::shared_ptr<Move>> evaluatedBestMoves;
        for (int i = 0; i < bestMoves.size(); ++i) {
            _board->handleMove(bestMoves[i]);
            _currentLine.push_back(bestMoves[i]);
            float moveEval = _calcBestEval(numPliesLeft - 1).first;
            _board->undoMove(bestMoves[i]);
            _currentLine.pop_back();
            evaluatedBestMoves.insert(std::make_pair(moveEval, bestMoves[i]));
            MAINLOG("Level: " << (5 - numPliesLeft) << " Tested move: " << bestMoves[i]->algebraicNotation() << " Evaluation: " << moveEval)
        }
        if (_board->whosTurnToGo() == Color::WHITE) {
            return std::make_pair(evaluatedBestMoves.rbegin()->first, evaluatedBestMoves.rbegin()->second);
        } else {
            return std::make_pair(evaluatedBestMoves.begin()->first, evaluatedBestMoves.begin()->second);
        }
    }
}

void TotalPieceValueEvaluator::_getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves) {
    std::vector<std::shared_ptr<Sascha::Gameplay::Move>> stagingArea;

    int numMovesStillNeeded = numMoves;
    float prevVal = 0.0;
    for (auto iter = start; iter != end; ++iter) {
        if (iter == start) {
            stagingArea.push_back(iter->second);
            prevVal = iter->first;
            continue;
        }
        if (_compareFloat(iter->first, prevVal)) {
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

void TotalPieceValueEvaluator::_getBestMovesFromSortedMap(int numMoves, const std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>> & orderedMoves, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator start, std::multimap<float, std::shared_ptr<Sascha::Gameplay::Move>>::reverse_iterator end, std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & bestMoves) {
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

float TotalPieceValueEvaluator::_evaluateMoveSingle(std::shared_ptr<Move> move) {
    float eval = 0.0;
    auto pieces = _board->pieces();
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
    if (_board->isCheckmate()) {
        if (_board->whosTurnToGo() == Color::WHITE) {
            eval = 100000.0;
        }
        else {
            eval = -100000.0;
        }
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

#include "Gameplay/Pieces/Knight.h"
#include "Gameplay/Board.h"
#include "Gameplay/Position.h"
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

using Sascha::Gameplay::Position;

void Knight::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    std::vector<std::shared_ptr<Move>> potentialMoves;
    std::vector<Position> potentialTargets;

    if (_position.col + 1 < 8 && _position.row + 2 < 8) {
        potentialTargets.push_back(Position(_position.col + 1, _position.row + 2));
    }
    if (_position.col + 1 < 8 && _position.row - 2 > -1) {
        potentialTargets.push_back(Position(_position.col + 1, _position.row - 2));
    }
    if (_position.col + 2 < 8 && _position.row + 1 < 8) {
        potentialTargets.push_back(Position(_position.col + 2, _position.row + 1));
    }
    if (_position.col + 2 < 8 && _position.row - 1 > -1) {
        potentialTargets.push_back(Position(_position.col + 2, _position.row - 1));
    }
    if (_position.col - 1 > -1 && _position.row + 2 < 8) {
        potentialTargets.push_back(Position(_position.col - 1, _position.row + 2));
    }
    if (_position.col - 1 > -1 && _position.row - 2 > -1) {
        potentialTargets.push_back(Position(_position.col - 1, _position.row - 2));
    }
    if (_position.col - 2 > -1 && _position.row + 1 < 8) {
        potentialTargets.push_back(Position(_position.col - 2, _position.row + 1));
    }
    if (_position.col - 2 > -1 && _position.row - 1 > -1) {
        potentialTargets.push_back(Position(_position.col - 2, _position.row - 1));
    }

    for (const auto & target : potentialTargets) {
        auto move = std::make_shared<Move>(_position, target, _board);
        //MAINLOG("Potential move: " << move->algebraicFormat())
        //MAINLOG("isSquareEmpty = " << _board->isSquareEmpty(target))
        //MAINLOG("isSquarePieceColor = " << _board->isSquarePieceColor(target, _color))
        //MAINLOG("testMoveForLegality = " << _board->testMoveForLegality(move))
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
    }
}

bool Knight::calculateHasLegalMove() const {
    std::vector<std::shared_ptr<Move>> potentialMoves;
    std::vector<Position> potentialTargets;

    if (_position.col + 1 < 8 && _position.row + 2 < 8) {
        Position target(_position.col + 1, _position.row + 2);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col + 1 < 8 && _position.row - 2 > -1) {
        Position target(_position.col + 1, _position.row - 2);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col + 2 < 8 && _position.row + 1 < 8) {
        Position target(_position.col + 2, _position.row + 1);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col + 2 < 8 && _position.row - 1 > -1) {
        Position target(_position.col + 2, _position.row - 1);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col - 1 > -1 && _position.row + 2 < 8) {
        Position target(_position.col - 1, _position.row + 2);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col - 1 > -1 && _position.row - 2 > -1) {
        Position target(_position.col - 1, _position.row - 2);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col - 2 > -1 && _position.row + 1 < 8) {
        Position target(_position.col - 2, _position.row + 1);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col - 2 > -1 && _position.row - 1 > -1) {
        Position target(_position.col - 2, _position.row - 1);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }

    return false;
}

bool Knight::isAttackingSquare(const Position & square) const {
    if (square == _position) {
        return false;
    }
    return Position::areSquaresKnightsMoveFromEachOther(_position, square);
}

}
}
}

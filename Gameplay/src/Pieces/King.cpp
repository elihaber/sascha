#include "Gameplay/Pieces/King.h"
#include "Gameplay/Board.h"
#include "Gameplay/Position.h"
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

using Sascha::Gameplay::Position;

void King::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    std::vector<std::shared_ptr<Move>> potentialMoves;
    std::vector<Position> potentialTargets;

    if (_position.row < 7) {
        potentialTargets.push_back(Position(_position.col, _position.row + 1));
        if (_position.col < 7) {
            potentialTargets.push_back(Position(_position.col + 1, _position.row + 1));
        }
        if (_position.col > 0) {
            potentialTargets.push_back(Position(_position.col - 1, _position.row + 1));
        }
    }
    if (_position.row > 0) {
        potentialTargets.push_back(Position(_position.col, _position.row - 1));
        if (_position.col < 7) {
            potentialTargets.push_back(Position(_position.col + 1, _position.row - 1));
        }
        if (_position.col > 0) {
            potentialTargets.push_back(Position(_position.col - 1, _position.row - 1));
        }
    }
    if (_position.col < 7) {
        potentialTargets.push_back(Position(_position.col + 1, _position.row));
    }
    if (_position.col > 0) {
        potentialTargets.push_back(Position(_position.col - 1, _position.row));
    }

    int baseRow = (_color == Color::WHITE ? 0 : 7);
    if (_board->castlingRights(_color, CastleSide::KINGSIDE)) {
        if (!_board->isCheck() && _board->isSquareEmpty(Position(5, baseRow)) && !_board->isSquareAttacked(Position(5, baseRow), oppositeColor(_color)) && _board->isSquareEmpty(Position(6, baseRow)) && !_board->isSquareAttacked(Position(6, baseRow), oppositeColor(_color))) {
            potentialTargets.push_back(Position(6, baseRow));
        }
    }
    if (_board->castlingRights(_color, CastleSide::QUEENSIDE)) {
        if (!_board->isCheck() && _board->isSquareEmpty(Position(3, baseRow)) && !_board->isSquareAttacked(Position(3, baseRow), oppositeColor(_color)) && _board->isSquareEmpty(Position(2, baseRow)) && !_board->isSquareAttacked(Position(2, baseRow), oppositeColor(_color)) && _board->isSquareEmpty(Position(1, baseRow))) {
            potentialTargets.push_back(Position(2, baseRow));
        }
    }

    for (const auto & target : potentialTargets) {
        auto move = std::make_shared<Move>(_position, target, _board);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) /* && !_board->isSquareAttacked(target, oppositeColor(_color)) */ && _board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
    }
}

bool King::calculateHasLegalMove() const {
    std::vector<std::shared_ptr<Move>> potentialMoves;
    std::vector<Position> potentialTargets;

    if (_position.row < 7) {
        Position target(_position.col, _position.row + 1);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
        if (_position.col < 7) {
            Position target(_position.col + 1, _position.row + 1);
            if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
                return true;
            }
        }
        if (_position.col > 0) {
            Position target(_position.col - 1, _position.row + 1);
            if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
                return true;
            }
        }
    }
    if (_position.row > 0) {
        Position target(_position.col, _position.row - 1);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
        if (_position.col < 7) {
            Position target(_position.col + 1, _position.row - 1);
            if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
                return true;
            }
        }
        if (_position.col > 0) {
            Position target(_position.col - 1, _position.row - 1);
            if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
                return true;
            }
        }
    }
    if (_position.col < 7) {
        Position target(_position.col + 1, _position.row);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }
    if (_position.col > 0) {
        Position target(_position.col - 1, _position.row);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(std::make_shared<Move>(_position, target, _board))) {
            return true;
        }
    }

    int baseRow = (_color == Color::WHITE ? 0 : 7);
    if (_board->castlingRights(_color, CastleSide::KINGSIDE)) {
        if (!_board->isCheck() && _board->isSquareEmpty(Position(5, baseRow)) && !_board->isSquareAttacked(Position(5, baseRow), oppositeColor(_color)) && _board->isSquareEmpty(Position(6, baseRow)) && !_board->isSquareAttacked(Position(6, baseRow), oppositeColor(_color))) {
            if (_board->testMoveForLegality(std::make_shared<Move>(_position, Position(6, baseRow), _board))) {
                return true;
            }
        }
    }
    if (_board->castlingRights(_color, CastleSide::QUEENSIDE)) {
        if (!_board->isCheck() && _board->isSquareEmpty(Position(3, baseRow)) && !_board->isSquareAttacked(Position(3, baseRow), oppositeColor(_color)) && _board->isSquareEmpty(Position(2, baseRow)) && !_board->isSquareAttacked(Position(2, baseRow), oppositeColor(_color)) && _board->isSquareEmpty(Position(1, baseRow))) {
            if (_board->testMoveForLegality(std::make_shared<Move>(_position, Position(2, baseRow), _board))) {
                return true;
            }
        }
    }

    return false;
}

bool King::isAttackingSquare(const Position & square) const {
    if (square == _position) {
        return false;
    }
    return Position::areSquaresTouching(_position, square);
}

}
}
}

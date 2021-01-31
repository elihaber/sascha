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

    if (_color == Color::WHITE) {
        if (_board->whitePlayer().hasCastlingRightsKingSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(5, 0)) && !_board->isSquareAttacked(Position(5, 0), oppositeColor(_color)) && _board->isSquareEmpty(Position(6, 0)) && !_board->isSquareAttacked(Position(6, 0), oppositeColor(_color))) {
                potentialTargets.push_back(Position(6, 0));
            }
        }
        if (_board->whitePlayer().hasCastlingRightsQueenSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(3, 0)) && !_board->isSquareAttacked(Position(3, 0), oppositeColor(_color)) && _board->isSquareEmpty(Position(2, 0)) && !_board->isSquareAttacked(Position(2, 0), oppositeColor(_color)) && _board->isSquareEmpty(Position(1, 0))) {
                potentialTargets.push_back(Position(2, 0));
            }
        }
    }
    else {
        if (_board->blackPlayer().hasCastlingRightsKingSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(5, 7)) && !_board->isSquareAttacked(Position(5, 7), oppositeColor(_color)) && _board->isSquareEmpty(Position(6, 7)) && !_board->isSquareAttacked(Position(6, 7), oppositeColor(_color))) {
                potentialTargets.push_back(Position(6, 0));
            }
        }
        if (_board->blackPlayer().hasCastlingRightsQueenSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(3, 7)) && !_board->isSquareAttacked(Position(3, 7), oppositeColor(_color)) && _board->isSquareEmpty(Position(2, 7)) && !_board->isSquareAttacked(Position(2, 7), oppositeColor(_color)) && _board->isSquareEmpty(Position(1, 7))) {
                potentialTargets.push_back(Position(2, 0));
            }
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

    if (_color == Color::WHITE) {
        if (_board->whitePlayer().hasCastlingRightsKingSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(5, 0)) && !_board->isSquareAttacked(Position(5, 0), oppositeColor(_color)) && _board->isSquareEmpty(Position(6, 0)) && !_board->isSquareAttacked(Position(6, 0), oppositeColor(_color))) {
                if (_board->testMoveForLegality(std::make_shared<Move>(_position, Position(6, 0), _board))) {
                    return true;
                }
            }
        }
        if (_board->whitePlayer().hasCastlingRightsQueenSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(3, 0)) && !_board->isSquareAttacked(Position(3, 0), oppositeColor(_color)) && _board->isSquareEmpty(Position(2, 0)) && !_board->isSquareAttacked(Position(2, 0), oppositeColor(_color)) && _board->isSquareEmpty(Position(1, 0))) {
                if (_board->testMoveForLegality(std::make_shared<Move>(_position, Position(2, 0), _board))) {
                    return true;
                }
            }
        }
    }
    else {
        if (_board->blackPlayer().hasCastlingRightsKingSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(5, 7)) && !_board->isSquareAttacked(Position(5, 7), oppositeColor(_color)) && _board->isSquareEmpty(Position(6, 7)) && !_board->isSquareAttacked(Position(6, 7), oppositeColor(_color))) {
                if (_board->testMoveForLegality(std::make_shared<Move>(_position, Position(6, 7), _board))) {
                    return true;
                }
            }
        }
        if (_board->blackPlayer().hasCastlingRightsQueenSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(3, 7)) && !_board->isSquareAttacked(Position(3, 7), oppositeColor(_color)) && _board->isSquareEmpty(Position(2, 7)) && !_board->isSquareAttacked(Position(2, 7), oppositeColor(_color)) && _board->isSquareEmpty(Position(1, 7))) {
                if (_board->testMoveForLegality(std::make_shared<Move>(_position, Position(2, 7), _board))) {
                    return true;
                }
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

#include "Gameplay/Pieces/Queen.h"
#include "Gameplay/Board.h"
#include "Gameplay/Position.h"
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

using Sascha::Gameplay::Position;

void Queen::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    int targetRow = _position.row + 1;
    while (targetRow < 8) {
        Position target(_position.col, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetRow;
    }
    targetRow = _position.row - 1;
    while (targetRow > -1) {
        Position target(_position.col, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetRow;
    }
    int targetCol = _position.col + 1;
    while (targetCol < 8) {
        Position target(targetCol, _position.row);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetCol;
    }
    targetCol = _position.col - 1;
    while (targetCol > -1) {
        Position target(targetCol, _position.row);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
    }
    targetCol = _position.col + 1;
    targetRow = _position.row + 1;
    while (targetCol < 8 && targetRow < 8) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetCol;
        ++targetRow;
    }
    targetCol = _position.col - 1;
    targetRow = _position.row - 1;
    while (targetCol > -1 && targetRow > -1) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
        --targetRow;
    }
    targetCol = _position.col + 1;
    targetRow = _position.row - 1;
    while (targetCol < 8 && targetRow > -1) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetCol;
        --targetRow;
    }
    targetCol = _position.col - 1;
    targetRow = _position.row + 1;
    while (targetCol > -1 && targetRow < 8) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
        ++targetRow;
    }
}

bool Queen::calculateHasLegalMove() const {
    int targetRow = _position.row + 1;
    while (targetRow < 8) {
        Position target(_position.col, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetRow;
    }
    targetRow = _position.row - 1;
    while (targetRow > -1) {
        Position target(_position.col, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetRow;
    }
    int targetCol = _position.col + 1;
    while (targetCol < 8) {
        Position target(targetCol, _position.row);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetCol;
    }
    targetCol = _position.col - 1;
    while (targetCol > -1) {
        Position target(targetCol, _position.row);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
    }
    targetCol = _position.col + 1;
    targetRow = _position.row + 1;
    while (targetCol < 8 && targetRow < 8) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetCol;
        ++targetRow;
    }
    targetCol = _position.col - 1;
    targetRow = _position.row - 1;
    while (targetCol > -1 && targetRow > -1) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
        --targetRow;
    }
    targetCol = _position.col + 1;
    targetRow = _position.row - 1;
    while (targetCol < 8 && targetRow > -1) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        ++targetCol;
        --targetRow;
    }
    targetCol = _position.col - 1;
    targetRow = _position.row + 1;
    while (targetCol > -1 && targetRow < 8) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _board);
        if (_board->testMoveForLegality(move)) {
            return true;
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
        ++targetRow;
    }

    return false;
}

bool Queen::isAttackingSquare(const Position & square) const {
    if (square == _position) {
        return false;
    }
    if (Position::areSquaresOnSameColumn(_position, square)) {
        if (_board->canSquaresSeeEachOtherOnColumn(_position, square)) {
            return true;
        }
    }
    if (Position::areSquaresOnSameRow(_position, square)) {
        if (_board->canSquaresSeeEachOtherOnRow(_position, square)) {
            return true;
        }
    }
    if (Position::areSquaresOnSameDiagonalUp(_position, square)) {
        if (_board->canSquaresSeeEachOtherOnDiagonalUp(_position, square)) {
            return true;
        }
    }
    if (Position::areSquaresOnSameDiagonalDown(_position, square)) {
        if (_board->canSquaresSeeEachOtherOnDiagonalDown(_position, square)) {
            return true;
        }
    }
    return false;
}

}
}
}

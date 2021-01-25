#include "Piece.h"
#include "Board.h"
#include "Move.h"
#include "Position.h"
#include "log.h"

std::shared_ptr<Piece> Piece::createPiece(PieceType pieceType, Color color, int col, int row, std::shared_ptr<Board> board) {
    if (pieceType == PieceType::PAWN) {
        return std::make_shared<Pawn>(color, col, row, board);
    }
    if (pieceType == PieceType::ROOK) {
        return std::make_shared<Rook>(color, col, row, board);
    }
    if (pieceType == PieceType::BISHOP) {
        return std::make_shared<Bishop>(color, col, row, board);
    }
    if (pieceType == PieceType::KNIGHT) {
        return std::make_shared<Knight>(color, col, row, board);
    }
    if (pieceType == PieceType::QUEEN) {
        return std::make_shared<Queen>(color, col, row, board);
    }
    if (pieceType == PieceType::KING) {
        return std::make_shared<King>(color, col, row, board);
    }
    if (pieceType == PieceType::BLANK) {
        return std::make_shared<BlankPiece>(color, col, row, board);
    }
    throw std::runtime_error("Incorrect type of piece to create");
}

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
            if (!_board->isCheck() && _board->isSquareEmpty(Position(5, 0)) && !_board->isSquareAttacked(Position(5, 0)) && _board->isSquareEmpty(Position(6, 0)) && !_board->isSquareAttacked(Position(6, 0))) {
                potentialTargets.push_back(Position(6, 0));
            }
        }
        if (_board->whitePlayer().hasCastlingRightsQueenSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(3, 0)) && !_board->isSquareAttacked(Position(3, 0)) && _board->isSquareEmpty(Position(2, 0)) && !_board->isSquareAttacked(Position(2, 0)) && _board->isSquareEmpty(Position(1, 0))) {
                potentialTargets.push_back(Position(2, 0));
            }
        }
    }
    else {
        if (_board->blackPlayer().hasCastlingRightsKingSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(5, 7)) && !_board->isSquareAttacked(Position(5, 7)) && _board->isSquareEmpty(Position(6, 7)) && !_board->isSquareAttacked(Position(6, 7))) {
                potentialTargets.push_back(Position(6, 0));
            }
        }
        if (_board->blackPlayer().hasCastlingRightsQueenSide()) {
            if (!_board->isCheck() && _board->isSquareEmpty(Position(3, 7)) && !_board->isSquareAttacked(Position(3, 7)) && _board->isSquareEmpty(Position(2, 7)) && !_board->isSquareAttacked(Position(2, 7)) && _board->isSquareEmpty(Position(1, 7))) {
                potentialTargets.push_back(Position(2, 0));
            }
        }
    }

    for (const auto & target : potentialTargets) {
        auto move = std::make_shared<Move>(_position, target, _pieceType);
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
    }
}

void Queen::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    int targetRow = _position.row + 1;
    while (targetRow < 8) {
        Position target(_position.col, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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

void Rook::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    int targetRow = _position.row + 1;
    while (targetRow < 8) {
        Position target(_position.col, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
        if (!_board->isSquareEmpty(target)) {
            break;
        }
        --targetCol;
    }
}

void Bishop::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    int targetCol = _position.col + 1;
    int targetRow = _position.row + 1;
    while (targetCol < 8 && targetRow < 8) {
        Position target(targetCol, targetRow);
        if (_board->isSquarePieceColor(target, _color)) {
            break;
        }
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
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
        auto move = std::make_shared<Move>(_position, target, _pieceType);
        log::out << "Potential move: " << move->algebraicNotation() << std::endl;
        log::out << "isSquareEmpty = " << _board->isSquareEmpty(target) << std::endl;
        log::out << "isSquarePieceColor = " << _board->isSquarePieceColor(target, _color) << std::endl;
        log::out << "testMoveForLegality = " << _board->testMoveForLegality(move) << std::endl; log::flush();
        if ((_board->isSquareEmpty(target) || !_board->isSquarePieceColor(target, _color)) && _board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
    }
}

void Pawn::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    std::vector<std::shared_ptr<Move>> potentialMoves;
    std::vector<Position> potentialTargets;

    if (_color == Color::WHITE) {
        auto target = Position(_position.col, _position.row + 1);
        if (_board->isSquareEmpty(target)) {
            potentialTargets.push_back(target);
            target = Position(_position.col, _position.row + 2);
            if (_position.row == 1 && _board->isSquareEmpty(target)) {
                potentialTargets.push_back(target);
            }
        }
        if (_position.col > 0) {
            target = Position(_position.col - 1, _position.row + 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                potentialTargets.push_back(target);
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialTargets.push_back(target);
                    }
                }
            }
        }
        if (_position.col < 7) {
            target = Position(_position.col + 1, _position.row + 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                potentialTargets.push_back(target);
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialTargets.push_back(target);
                    }
                }
            }
        }
    }
    else {
        auto target = Position(_position.col, _position.row - 1);
        if (_board->isSquareEmpty(target)) {
            potentialTargets.push_back(target);
            target = Position(_position.col, _position.row - 2);
            if (_position.row == 6 && _board->isSquareEmpty(target)) {
                potentialTargets.push_back(target);
            }
        }
        if (_position.col > 0) {
            target = Position(_position.col - 1, _position.row - 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                potentialTargets.push_back(target);
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialTargets.push_back(target);
                    }
                }
            }
        }
        if (_position.col < 7) {
            target = Position(_position.col + 1, _position.row - 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                potentialTargets.push_back(target);
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialTargets.push_back(target);
                    }
                }
            }
        }
    }

    for (const auto & target : potentialTargets) {
        if (target.row != 0 && target.row != 7) {
            auto move = std::make_shared<Move>(_position, target, _pieceType);
            if (_board->testMoveForLegality(move)) {
                possibleMoves.push_back(move);
            }
        }
        else {
            auto move = std::make_shared<Move>(_position, target, _pieceType);
            move->setHasPromotion(true);
            move->setPromotionResult(PieceType::KNIGHT);
            if (_board->testMoveForLegality(move)) {
                possibleMoves.push_back(move);
            }
            move = std::make_shared<Move>(_position, target, _pieceType);
            move->setHasPromotion(true);
            move->setPromotionResult(PieceType::BISHOP);
            if (_board->testMoveForLegality(move)) {
                possibleMoves.push_back(move);
            }
            move = std::make_shared<Move>(_position, target, _pieceType);
            move->setHasPromotion(true);
            move->setPromotionResult(PieceType::ROOK);
            if (_board->testMoveForLegality(move)) {
                possibleMoves.push_back(move);
            }
            move = std::make_shared<Move>(_position, target, _pieceType);
            move->setHasPromotion(true);
            move->setPromotionResult(PieceType::QUEEN);
            if (_board->testMoveForLegality(move)) {
                possibleMoves.push_back(move);
            }
        }
    }
}

void BlankPiece::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const { }

bool King::isAttackingSquare(const Position & square) const {
    return Position::areSquaresTouching(_position, square);
}

bool Queen::isAttackingSquare(const Position & square) const {
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

bool Rook::isAttackingSquare(const Position & square) const {
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
    return false;
}

bool Bishop::isAttackingSquare(const Position & square) const {
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

bool Knight::isAttackingSquare(const Position & square) const {
    log::out << "Checking if knight on square " << _position.col << "," << _position.row << " is attacking square " << square.col << "," << square.row << std::endl; log::flush();
    return Position::areSquaresKnightsMoveFromEachOther(_position, square);
}

bool Pawn::isAttackingSquare(const Position & square) const {
    auto attackSquares = Position::getSquaresWherePawnCanAttack(square, _color);
    for (auto attackSquare : attackSquares) {
        if (attackSquare.col == _position.col && attackSquare.row == _position.row) {
            return true;
        }
    }
    return false;
}

bool BlankPiece::isAttackingSquare(const Position & square) const {
    return false;
}
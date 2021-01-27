#include "Gameplay/Pieces/Pawn.h"
#include "Gameplay/Board.h"
#include "Gameplay/Position.h"
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

using Sascha::Gameplay::Position;

void Pawn::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    std::vector<std::shared_ptr<Move>> potentialMoves;

    if (_color == Color::WHITE) {
        auto target = Position(_position.col, _position.row + 1);
        if (_board->isSquareEmpty(target)) {
            if (target.row != 7) {
                potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
            }
            else {
                auto move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::KNIGHT);
                potentialMoves.push_back(move);
                move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::BISHOP);
                potentialMoves.push_back(move);
                move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::ROOK);
                potentialMoves.push_back(move);
                move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::QUEEN);
                potentialMoves.push_back(move);
            }
            target = Position(_position.col, _position.row + 2);
            if (_position.row == 1 && _board->isSquareEmpty(target)) {
                potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
            }
        }
        if (_position.col > 0) {
            target = Position(_position.col - 1, _position.row + 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                if (target.row != 7) {
                    potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                }
                else {
                    auto move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::KNIGHT);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::BISHOP);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::ROOK);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::QUEEN);
                    potentialMoves.push_back(move);
                }
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                    }
                }
            }
        }
        if (_position.col < 7) {
            target = Position(_position.col + 1, _position.row + 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                if (target.row != 7) {
                    potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                }
                else {
                    auto move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::KNIGHT);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::BISHOP);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::ROOK);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::QUEEN);
                    potentialMoves.push_back(move);
                }
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                    }
                }
            }
        }
    }
    else {
        auto target = Position(_position.col, _position.row - 1);
        if (_board->isSquareEmpty(target)) {
            if (target.row != 0) {
                potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
            }
            else {
                auto move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::KNIGHT);
                potentialMoves.push_back(move);
                move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::BISHOP);
                potentialMoves.push_back(move);
                move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::ROOK);
                potentialMoves.push_back(move);
                move = std::make_shared<Move>(_position, target, _pieceType);
                move->setHasPromotion(true);
                move->setPromotionResult(PieceType::QUEEN);
                potentialMoves.push_back(move);
            }
            target = Position(_position.col, _position.row - 2);
            if (_position.row == 6 && _board->isSquareEmpty(target)) {
                potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
            }
        }
        if (_position.col > 0) {
            target = Position(_position.col - 1, _position.row - 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                if (target.row != 0) {
                    potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                }
                else {
                    auto move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::KNIGHT);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::BISHOP);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::ROOK);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::QUEEN);
                    potentialMoves.push_back(move);
                }
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                    }
                }
            }
        }
        if (_position.col < 7) {
            target = Position(_position.col + 1, _position.row - 1);
            if (!_board->isSquareEmpty(target) && !_board->isSquarePieceColor(target, _color)) {
                if (target.row != 0) {
                    potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                }
                else {
                    auto move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::KNIGHT);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::BISHOP);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::ROOK);
                    potentialMoves.push_back(move);
                    move = std::make_shared<Move>(_position, target, _pieceType);
                    move->setHasPromotion(true);
                    move->setPromotionResult(PieceType::QUEEN);
                    potentialMoves.push_back(move);
                }
            }
            else {
                Position enPassantTarget;
                if (_board->enPassantTarget(enPassantTarget)) {
                    if (enPassantTarget == target) {
                        potentialMoves.push_back(std::make_shared<Move>(_position, target, _pieceType));
                    }
                }
            }
        }
    }

    for (const auto & move : potentialMoves) {
        if (_board->testMoveForLegality(move)) {
            possibleMoves.push_back(move);
        }
    }
}

bool Pawn::isAttackingSquare(const Position & square) const {
    if (square == _position) {
        return false;
    }
    auto attackSquares = Position::getSquaresWherePawnCanAttack(square, _color);
    for (auto attackSquare : attackSquares) {
        if (attackSquare.col == _position.col && attackSquare.row == _position.row) {
            return true;
        }
    }
    return false;
}

}
}
}

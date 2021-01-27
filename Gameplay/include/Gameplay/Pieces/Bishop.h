#pragma once

#include "Piece.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

class Bishop : public Piece {
public:
    Bishop(std::shared_ptr<Board> board) : Piece(PieceType::BISHOP, board) { }
    Bishop(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::BISHOP, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const override;
    bool isAttackingSquare(const Position & square) const override;
};

}
}
}

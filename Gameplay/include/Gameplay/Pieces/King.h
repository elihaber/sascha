#pragma once

#include "Piece.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

class King : public Piece {
public:
    King(std::shared_ptr<Board> board) : Piece(PieceType::KING, board) { }
    King(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::KING, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const override;
    bool isAttackingSquare(const Position & square) const override;
};

}
}
}

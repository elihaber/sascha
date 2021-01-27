#pragma once

#include "Piece.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

class Pawn : public Piece {
public:
    Pawn(std::shared_ptr<Board> board) : Piece(PieceType::PAWN, board) { }
    Pawn(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::PAWN, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const override;
    bool isAttackingSquare(const Position & square) const override;
};

}
}
}

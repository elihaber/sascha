#pragma once

#include "Piece.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

class Rook : public Piece {
public:
    Rook(std::shared_ptr<Board> board) : Piece(PieceType::ROOK, board) { }
    Rook(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::ROOK, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const override;
    bool calculateHasLegalMove() const override ;
    bool isAttackingSquare(const Position & square) const override;
};

}
}
}

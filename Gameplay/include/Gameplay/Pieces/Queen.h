#pragma once

#include "Piece.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

class Queen : public Piece {
public:
    Queen(std::shared_ptr<Board> board) : Piece(PieceType::QUEEN, board) { }
    Queen(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::QUEEN, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const override;
    bool calculateHasLegalMove() const override ;
    bool isAttackingSquare(const Position & square) const override;
};

}
}
}

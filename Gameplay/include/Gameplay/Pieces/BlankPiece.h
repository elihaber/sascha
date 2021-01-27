#pragma once

#include <vector>
#include "Piece.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

class BlankPiece : public Piece {
public:
    BlankPiece(std::shared_ptr<Board> board) : Piece(PieceType::BLANK, board) { }
    BlankPiece(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::BLANK, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const override;
    virtual bool isBlankPiece() override { return true; }
    bool isAttackingSquare(const Position & square) const override;
};

}
}
}

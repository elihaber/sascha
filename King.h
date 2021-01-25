#pragma once

#include "Piece.h"

class King : public Piece {
public:
    King(std::shared_ptr<Board> board) : Piece(PieceType::KING, board) { }
    King(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::KING, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::string> & possibleMoves) const override { }
};

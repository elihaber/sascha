#pragma once

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(std::shared_ptr<Board> board) : Piece(PieceType::BISHOP, board) { }
    Bishop(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::BISHOP, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::string> & possibleMoves) const override;
};

#pragma once

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(std::shared_ptr<Board> board) : Piece(PieceType::KNIGHT, board) { }
    Knight(Color color, int col, int row, std::shared_ptr<Board> board) : Piece(PieceType::KNIGHT, color, col, row, board) { }
    void getPossibleMoves(std::vector<std::string> & possibleMoves) const override;
};

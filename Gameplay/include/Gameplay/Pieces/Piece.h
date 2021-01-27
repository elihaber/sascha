#pragma once

#include <memory>
#include <vector>
#include "Gameplay/Position.h"
#include "Enums.h"

namespace Sascha {
namespace Gameplay {

class Board;
class Move;

namespace Pieces {

class Piece {
public:
    Piece(PieceType pieceType, Color color, int col, int row, std::shared_ptr<Board> board) : _pieceType(pieceType), _color(color), _position(col, row), _board(board) { }
    Piece(PieceType pieceType, std::shared_ptr<Board> board) : _pieceType(pieceType), _board(board) { }
    void setColor(Color val) { _color = val; }
    void setPosition(int col, int row) { _position.col = col; _position.row = row; }
    Position position() const { return _position; }
    virtual bool isBlankPiece() { return false; }
    void setHasMoved(bool val) { _hasMoved = val; }
    void incrementNumTimesMoved() { ++_numTimesMoved; _hasMoved = true; }
    void decrementNumTimesMoved() { --_numTimesMoved; if (_numTimesMoved == 0) { _hasMoved = false; } }
    bool hasMoved() const { return _hasMoved; }
    PieceType pieceType() const { return _pieceType; }
    Color color() const { return _color; }
    virtual void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const = 0;
    virtual bool isAttackingSquare(const Position & square) const = 0;
    void setNumTimesMoved(int val) { _numTimesMoved = val; }
    int numTimesMoved() const { return _numTimesMoved; }
    void setPieceType(PieceType val) { _pieceType = val; }

    static std::shared_ptr<Piece> createPiece(PieceType pieceType, Color color, int col, int row, std::shared_ptr<Board> board);

protected:
    int _maxVal;
    bool _isOnBoard;
    bool _hasMoved;
    int _numTimesMoved;
    Color _color;
    Position _position;
    PieceType _pieceType;
    std::shared_ptr<Board> _board;
};

}
}
}

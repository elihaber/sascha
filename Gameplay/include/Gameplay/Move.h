#pragma once

#include <memory>
#include <string>
#include "Position.h"

namespace Sascha {
namespace Gameplay {

class Board;

class Move {
public:
    Move(const Position & source, const Position & target, const std::shared_ptr<Board> & board);
    Move(const std::string & algebraicNotation, const std::shared_ptr<Board> & board);

    Position source() const { return _source; }
    Position target() const { return _target; }
    Color color() const { return _color; }
    PieceType pieceType() const { return _pieceType; }
    bool isCapture() const { return _isCapture; }
    PieceType pieceTypeCaptured() const { return _pieceTypeCaptured; }
    bool isPromotion() const { return _isPromotion; }
    PieceType promotionResult() const { return _promotionResult; }
    bool isCastle() const { return _isCastle; }
    CastleSide castleSide() const { return _castleSide; }
    std::string uciFormat() const { return _uciFormat; }
    std::string algebraicFormat() const { return _algebraicFormat; }
    CastleSide ruinedCastling() const { return _ruinedCastling; }

    void setPromotionResult(PieceType val);
    void setRuinedCastling(CastleSide val) { _ruinedCastling = val; }

    bool operator ==(const Move & other) const {
        return (_source == other.source() && _target == other.target());
    }

private:
    void _findColor();
    void _findPieceType();
    void _calculateIsCapture();
    void _calculateIsPromotion();
    void _calculateIsCastle();
    void _calculateUciFormat();
    void _calculateInternalValues();
    void _calculateAlgebraicFormat();

    Position _source;
    Position _target;
    std::shared_ptr<Board> _board;
    Color _color;
    PieceType _pieceType;
    bool _isCapture;
    PieceType _pieceTypeCaptured;
    bool _isPromotion;
    PieceType _promotionResult;
    bool _isCastle;
    CastleSide _castleSide;
    std::string _uciFormat;
    std::string _algebraicFormat;
    CastleSide _ruinedCastling;
};

}
}

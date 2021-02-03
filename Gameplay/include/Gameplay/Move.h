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
    bool isEnPassant() const { return _isEnPassant; }
    void setRuinedEnPassant(bool val) { _ruinedEnPassant = val; }
    void setRuinedEnPassantTarget(const Position & pos) { _ruinedEnPassantTarget = pos; }
    void setEnabledEnPassant(bool val) { _enabledEnPassant = val; }
    void setEnabledEnPassantTarget(const Position & pos) { _enabledEnPassantTarget = pos; }
    CastleSide castleSide() const { return _castleSide; }
    std::string uciFormat() const { return _uciFormat; }
    std::string algebraicFormat() const { return _algebraicFormat; }
    bool ruinedCastling(int index) const { return _ruinedCastling[index]; }
    bool ruinedEnPassant() const { return _ruinedEnPassant; }
    Position ruinedEnPassantTarget() const { return _ruinedEnPassantTarget; }
    bool enabledEnPassant() const { return _enabledEnPassant; }
    Position enabledEnPassantTarget() const { return _enabledEnPassantTarget; }

    void setPromotionResult(PieceType val);
    void setRuinedCastling(CastleSide castleSide, bool val) { _ruinedCastling[castleSideToInt(castleSide)] = val; }

    bool operator ==(const Move & other) const {
        return (_source == other.source() && _target == other.target());
    }

private:
    void _findColor();
    void _findPieceType();
    void _calculateIsCapture();
    void _calculateIsPromotion();
    void _calculateIsCastle();
    void _calculateIsEnPassant();
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
    bool _ruinedCastling[2];
    bool _isEnPassant;
    bool _ruinedEnPassant;
    Position _ruinedEnPassantTarget;
    bool _enabledEnPassant;
    Position _enabledEnPassantTarget;
};

}
}

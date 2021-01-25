#pragma once

#include <string>
#include "Position.h"
class Move {
public:
    Move(const Position & source, const Position & target, PieceType pieceType) : _source(source), _target(target), _hasPromotion(false), _pieceType(pieceType) {
        _calculateAlgebraicNotation();
        _calculateIsCastle();
    }
    Move(const std::string & algebraicNotation, PieceType pieceType) : _algebraicNotation(algebraicNotation), _hasPromotion(false), _pieceType(pieceType) {
        _calculateInternalValues();
        _calculateIsCastle();
    }
    Position source() const { return _source; }
    Position target() const { return _target; }
    std::string algebraicNotation() const { return _algebraicNotation; }
    bool hasPromotion() const { return _hasPromotion; }
    PieceType promotionResult() const { return _promotionResult; }
    void setHasPromotion(bool val);
    void setPromotionResult(PieceType val);
    bool isCastle() const { return _isCastle; }
private:
    void _calculateAlgebraicNotation();
    void _calculateInternalValues();
    void _calculateIsCastle();

    Position _source;
    Position _target;
    std::string _algebraicNotation;
    bool _hasPromotion;
    PieceType _promotionResult;
    bool _isCastle;
    PieceType _pieceType;
};

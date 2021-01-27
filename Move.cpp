#include "Move.h"

namespace Sascha {
namespace Gameplay {

void Move::_calculateAlgebraicNotation() {
    _algebraicNotation.clear();
    _algebraicNotation += ('a' + _source.col);
    _algebraicNotation += ('1' + _source.row);
    _algebraicNotation += ('a' + _target.col);
    _algebraicNotation += ('1' + _target.row);
    if (_hasPromotion) {
        if (_promotionResult == PieceType::QUEEN) {
            _algebraicNotation += 'q';
        }
        else if (_promotionResult == PieceType::ROOK) {
            _algebraicNotation += 'r';
        }
        else if (_promotionResult == PieceType::BISHOP) {
            _algebraicNotation += 'b';
        }
        else if (_promotionResult == PieceType::KNIGHT) {
            _algebraicNotation += 'n';
        }
    }
}

void Move::_calculateInternalValues() {
    Position::algebraicToInternal(_algebraicNotation.substr(0, 2), _source);
    Position::algebraicToInternal(_algebraicNotation.substr(2, 4), _target);
    if (_algebraicNotation.size() == 5) {
        _hasPromotion = true;
        if (_algebraicNotation[4] == 'q') {
            _promotionResult = PieceType::QUEEN;
        }
        else if (_algebraicNotation[4] == 'b') {
            _promotionResult = PieceType::BISHOP;
        }
        else if (_algebraicNotation[4] == 'n' || _algebraicNotation[4] == 'k') {
            _promotionResult = PieceType::KNIGHT;
        }
        else if (_algebraicNotation[4] == 'r') {
            _promotionResult = PieceType::ROOK;
        }
    }
}

void Move::_calculateIsCastle() {
    _isCastle = (_pieceType == PieceType::KING && _source.row == _target.row && abs(_source.col - _target.col) > 1);
}

void Move::setHasPromotion(bool val) {
    _hasPromotion = val;
    _calculateAlgebraicNotation();
}

void Move::setPromotionResult(PieceType val) {
    _promotionResult = val;
    _calculateAlgebraicNotation();
}

}
}

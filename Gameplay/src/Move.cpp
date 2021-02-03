#include "Gameplay/Move.h"
#include "Gameplay/Board.h"

namespace Sascha {
namespace Gameplay {

Move::Move(const Position & source, const Position & target, const std::shared_ptr<Board> & board) : 
        _board(board), _source(source), _target(target), _promotionResult(PieceType::BLANK), _ruinedEnPassant(false), _enabledEnPassant(false)  {
    _calculateUciFormat();
    _findColor();
    _findPieceType();
    _calculateIsCapture();
    _calculateIsPromotion();
    _calculateIsCastle();
    _calculateIsEnPassant();
    _calculateAlgebraicFormat();
}
Move::Move(const std::string & uciFormat, const std::shared_ptr<Board> & board) : _board(board), _uciFormat(uciFormat), _ruinedEnPassant(false), _enabledEnPassant(false) {
    _calculateInternalValues();
    _findColor();
    _findPieceType();
    _calculateIsCapture();
    _calculateIsCastle();
    _calculateIsEnPassant();
    _calculateAlgebraicFormat();
}

void Move::_findColor() {
    _color = _board->pieceAt(_source)->color();
}

void Move::_findPieceType() {
    _pieceType = _board->pieceAt(_source)->pieceType();
}

void Move::_calculateIsCapture() {
    _isCapture = (!_board->pieceAt(_target)->isBlankPiece() && _board->pieceAt(_target)->color() != _board->pieceAt(_source)->color());
    if (_isCapture) {
        _pieceTypeCaptured = _board->pieceAt(_target)->pieceType();
    }
}

void Move::_calculateIsPromotion() {
    _isPromotion = (_pieceType == PieceType::PAWN && (_target.row == 0 || _target.row == 7));
}

void Move::_calculateIsCastle() {
    _isCastle = (_pieceType == PieceType::KING && _source.row == _target.row && abs(_source.col - _target.col) > 1);
    if (_isCastle) {
        _castleSide = (_target.col == 6 ? CastleSide::KINGSIDE : CastleSide::QUEENSIDE);
    }
}

void Move::_calculateIsEnPassant() {
    Position enPassantTarget;
    _board->enPassantTarget(enPassantTarget);
    _isEnPassant = (_pieceType == PieceType::PAWN && _board->hasEnPassantTarget() && enPassantTarget == _target);
}

void Move::_calculateUciFormat() {
    _uciFormat.clear();
    _uciFormat += ('a' + _source.col);
    _uciFormat += ('1' + _source.row);
    _uciFormat += ('a' + _target.col);
    _uciFormat += ('1' + _target.row);
    if (_isPromotion) {
        if (_promotionResult == PieceType::QUEEN) {
            _uciFormat += 'q';
        }
        else if (_promotionResult == PieceType::ROOK) {
            _uciFormat += 'r';
        }
        else if (_promotionResult == PieceType::BISHOP) {
            _uciFormat += 'b';
        }
        else if (_promotionResult == PieceType::KNIGHT) {
            _uciFormat += 'n';
        }
    }
    else {
        _promotionResult = PieceType::BLANK;
    }
}

void Move::_calculateAlgebraicFormat() {
    _algebraicFormat.clear();
    if (_pieceType == PieceType::KING) {
        _algebraicFormat += 'K';
    }
    else if (_pieceType == PieceType::QUEEN) {
        _algebraicFormat += 'Q';
    }
    else if (_pieceType == PieceType::ROOK) {
        _algebraicFormat += 'R';
    }
    else if (_pieceType == PieceType::BISHOP) {
        _algebraicFormat += 'B';
    }
    else if (_pieceType == PieceType::KNIGHT) {
        _algebraicFormat += 'N';
    }
    else if (_pieceType == PieceType::PAWN) {
        if (_isCapture) {
            _algebraicFormat += ('a' + _source.col);
        }
    }
    if (_pieceType != PieceType::PAWN && _pieceType != PieceType::KING) {
        auto allAttackers = _board->getAllAttackerOfTypeOnSquare(_pieceType, _color, _target);
        bool sameRank = false;
        bool sameFile = false;
        for (auto & attacker : allAttackers) {
            if (attacker->position().row == _source.row) {
                sameRank = true;
            }
            if (attacker->position().col == _source.col) {
                sameFile = true;
            }
        }
        if (sameRank) {
            _algebraicFormat += ('a' + _source.col);
        }
        if (sameFile) {
            _algebraicFormat += ('1' + _source.row);
        }
    }
    if (_isCapture) {
        _algebraicFormat += 'x';
    }
    _algebraicFormat += ('a' + _target.col);
    _algebraicFormat += ('1' + _target.row);
    if (_isPromotion) {
        _algebraicFormat += '=';
        if (_promotionResult == PieceType::QUEEN) {
            _algebraicFormat += 'Q';
        }
        else if (_promotionResult == PieceType::ROOK) {
            _algebraicFormat += 'R';
        }
        else if (_promotionResult == PieceType::BISHOP) {
            _algebraicFormat += 'B';
        }
        else if (_promotionResult == PieceType::KNIGHT) {
            _algebraicFormat += 'N';
        }
        else {
            _algebraicFormat += 'X';
        }
    }
}

void Move::_calculateInternalValues() {
    Position::uciToInternal(_uciFormat.substr(0, 2), _source);
    Position::uciToInternal(_uciFormat.substr(2, 4), _target);
    if (_uciFormat.size() == 5) {
        _isPromotion = true;
        if (_uciFormat[4] == 'q') {
            _promotionResult = PieceType::QUEEN;
        }
        else if (_uciFormat[4] == 'b') {
            _promotionResult = PieceType::BISHOP;
        }
        else if (_uciFormat[4] == 'n' || _uciFormat[4] == 'k') {
            _promotionResult = PieceType::KNIGHT;
        }
        else if (_uciFormat[4] == 'r') {
            _promotionResult = PieceType::ROOK;
        }
    }
    else {
        _isPromotion = false;
        _promotionResult = PieceType::BLANK;
    }
}

void Move::setPromotionResult(PieceType val) {
    if (!_isPromotion) {
        MAINLOG("ERROR: Setting promotion result for move that is not promotion: " << _algebraicFormat)
        throw std::runtime_error("ERROR: Setting promotion result for move that is not promotion");
    }

    _promotionResult = val;
    _calculateUciFormat();
    _calculateAlgebraicFormat();
}

}
}

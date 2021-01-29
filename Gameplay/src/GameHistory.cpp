#include "Gameplay/GameHistory.h"
#include "Gameplay/Move.h"
#include "Gameplay/Pieces/Piece.h"

namespace Sascha {
namespace Gameplay {

void GameHistory::addMove(const std::shared_ptr<Move> & move, const std::string & fen) {
    _fens.push(fen);
    _moves.push(move);
}

void GameHistory::addCapturedPiece(const std::shared_ptr<Pieces::Piece> & piece) {
    _capturedPieces.push(piece);
}

void GameHistory::addPromotedPawn(const std::shared_ptr<Pieces::Piece> & piece) {
    _promotedPawns.push(piece);
}

void GameHistory::undoLastMove() {
    _fens.pop();

     auto move = _moves.top();
     if (move->isCapture()) {
         _capturedPieces.pop();
     }
     if (move->isPromotion()) {
         _promotedPawns.pop();
     }

    _moves.pop();
}

bool GameHistory::isEmpty() const {
    return _fens.empty();
}

bool GameHistory::lastFen(std::string & fen) const {
    if (_fens.empty()) {
        return false;
    }
    fen = _fens.top();
    return true;
}

bool GameHistory::lastMove(std::shared_ptr<Move> & move) const {
    if (_moves.empty()) {
        return false;
    }
    move = _moves.top();
    return true;
}

bool GameHistory::lastCapturedPiece(std::shared_ptr<Pieces::Piece> & piece) const {
    if (_capturedPieces.empty()) {
        return false;
    }
    piece = _capturedPieces.top();
    return true;
}

bool GameHistory::lastPromotedPawn(std::shared_ptr<Pieces::Piece> & piece) const {
    if (_promotedPawns.empty()) {
        return false;
    }
    piece = _promotedPawns.top();
    return true;
}

}
}

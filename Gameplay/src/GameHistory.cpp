#include "Gameplay/GameHistory.h"
#include "Gameplay/Move.h"
#include "Gameplay/Pieces/Piece.h"

namespace Sascha {
namespace Gameplay {

void GameHistory::clear() {
    while (!_fens.empty())
        _fens.pop();
    while (!_moves.empty())
        _moves.pop();
    while (!_capturedPieces.empty())
        _capturedPieces.pop();
    while (!_promotedPawns.empty())
        _promotedPawns.pop();
}
void GameHistory::addStartupFen(const std::string & fen) {
    _fens.push(fen);
}

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
     if (move->isCapture() || move->isEnPassant()) {
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

GameHistory GameHistory::clone() const {
    GameHistory clone;
    //std::shared_ptr<GameHistory> clone = std::make_shared<GameHistory>();
    clone.setFens(std::stack<std::string>(_fens));
    clone.setMoves(std::stack<std::shared_ptr<Move>>(_moves));
    clone.setCapturedPieces(std::stack<std::shared_ptr<Pieces::Piece>>(_capturedPieces));
    clone.setPromotedPawns(std::stack<std::shared_ptr<Pieces::Piece>>(_promotedPawns));
    return clone;
}

void GameHistory::copy_reverse(std::stack<std::string> & source, std::stack<std::string> & dest) {
    while(!source.empty()) {
        dest.push(std::string(source.top()));
        source.pop();
    }
}


void GameHistory::CopyStack(const std::stack<std::string> & source, std::stack<std::string> & target) {
    std::stack<std::string> tmpCopy(source);
    std::stack<std::string> tmp;
    copy_reverse(tmpCopy, tmp);
    copy_reverse(tmp, target);
}

}
}

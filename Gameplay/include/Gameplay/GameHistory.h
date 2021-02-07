#pragma once

#include <string>
#include <stack>
#include <memory>
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {

namespace Pieces { class Piece; }

class Board;

class GameHistory {
public:
    void addMove(const std::shared_ptr<Move> & move, const std::string & fen);
    void addCapturedPiece(const std::shared_ptr<Pieces::Piece> & piece);
    void addPromotedPawn(const std::shared_ptr<Pieces::Piece> & piece);
    void undoLastMove();
    bool isEmpty() const;
    bool lastFen(std::string & fen) const;
    bool lastMove(std::shared_ptr<Move> & move) const;
    bool lastCapturedPiece(std::shared_ptr<Pieces::Piece> & move) const;
    bool lastPromotedPawn(std::shared_ptr<Pieces::Piece> & move) const;
    void clear();
    void addStartupFen(const std::string & fen);
    GameHistory clone() const;

    // Methods for clone
    std::stack<std::string> & getFens() { return _fens; }
    void setFens(std::stack<std::string> val) { _fens = val; }
    std::stack<std::shared_ptr<Move>> & getMoves() { return _moves; }
    void setMoves(std::stack<std::shared_ptr<Move>> val) { _moves = val; }
    std::stack<std::shared_ptr<Pieces::Piece>> & getCapturedPieces() { return _capturedPieces; }
    void setCapturedPieces(std::stack<std::shared_ptr<Pieces::Piece>> val) { _capturedPieces = val; }
    std::stack<std::shared_ptr<Pieces::Piece>> & getPromotedPawns() { return _promotedPawns; }
    void setPromotedPawns(std::stack<std::shared_ptr<Pieces::Piece>> val) { _promotedPawns = val; }

    static void copy_reverse(std::stack<std::string> & source, std::stack<std::string> & dest);
    static void CopyStack(const std::stack<std::string> & source, std::stack<std::string> & target);
private:
    std::stack<std::string> _fens;
    std::stack<std::shared_ptr<Move>> _moves;
    std::stack<std::shared_ptr<Pieces::Piece>> _capturedPieces;
    std::stack<std::shared_ptr<Pieces::Piece>> _promotedPawns;
};

}
}
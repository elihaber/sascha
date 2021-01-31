#pragma once

#include <string>
#include <stack>
#include <memory>
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {

namespace Pieces { class Piece; }

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
private:
    std::stack<std::string> _fens;
    std::stack<std::shared_ptr<Move>> _moves;
    std::stack<std::shared_ptr<Pieces::Piece>> _capturedPieces;
    std::stack<std::shared_ptr<Pieces::Piece>> _promotedPawns;
};

}
}
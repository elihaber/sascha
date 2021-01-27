#pragma once

#include <vector>
#include <string>
#include "Enums.h"
#include "Gameplay/Pieces/Piece.h"
#include "Gameplay/Player.h"

namespace Sascha {
namespace Gameplay {

class Move;

class Board : public std::enable_shared_from_this<Board> {
public:
    Board(Player & whitePlayer, Player & blackPlayer) : _whitePlayer(whitePlayer), _blackPlayer(blackPlayer) { }
    void setUpStartingPosition();
    void setUpFromFen(const std::string & fen);
    void handleMove(const std::shared_ptr<Move> & move);
    void undoMove(const std::shared_ptr<Move> & move);
    void handleMoveAlgebraic(const std::string & moveAlgebraic);
    int findPiecePosByPosition(const Position & pos);
    void getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const;
    bool scanForCheck(bool otherPlayer = false);
    bool isCheck() const { return _isCheck; }
    bool isSquareAttacked(const Position & square, Color attackerColor) const;
    bool canSquaresSeeEachOtherOnColumn(Position pos1, Position pos2);
    bool canSquaresSeeEachOtherOnRow(Position pos1, Position pos2);
    bool canSquaresSeeEachOtherOnDiagonalUp(Position pos1, Position pos2);
    bool canSquaresSeeEachOtherOnDiagonalDown(Position pos1, Position pos2);
    Color whosTurnToGo() const { return _whosTurnToGo; }
    void calculateLegalMoves(std::vector<std::shared_ptr<Move>> moves) const;
    bool isSquareEmpty(const Position pos) const;
    bool isSquarePieceColor(const Position pos, Color color) const;
    bool testMoveForLegality(const std::shared_ptr<Move> move);
    bool hasEnPassantTarget() const { return _hasEnPassantTarget; }
    bool enPassantTarget(Position & position) const { position = _enPassantTarget; return _hasEnPassantTarget; }
    Player & whitePlayer() const { return _whitePlayer; }
    Player & blackPlayer() const { return _blackPlayer; }

private:
    void _calculateFen();

    std::vector<std::shared_ptr<Pieces::Piece>> _pieces;
    Color _whosTurnToGo;
    Player & _whitePlayer;
    Player & _blackPlayer;
    bool _hasEnPassantTarget;
    Position _enPassantTarget;
    int _halfMoveClock;
    int _fullMoveNumber;
    bool _isCheck;
    std::string _fen;

    // Previous board state
    Color _prevWhosTurnToGo;
    bool _prevHasEnPassantTarget;
    Position _prevEnPassantTarget;
    int _prevHalfmoveClock;
    int _prevFullMoveNumber;
    bool _prevIsCheck;
    bool _prevWhiteCastlingKing;
    bool _prevWhiteCastlingQueen;
    bool _prevBlackCastlingKing;
    bool _prevBlackCastlingQueen;
    bool _prevHasCapturedPiece;
    Position _prevCapturedPiecePosition;
    PieceType _prevCapturedPieceType;
    Color _prevCapturedPieceColor;
    int _prevCapturedPieceNumTimesMoved;
    bool _prevHasPromotion;
    bool _prevMoveIsCastle;
    std::string _prevFen;
};

}
}

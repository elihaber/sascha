#pragma once

#include <vector>
#include <string>
#include "Enums.h"
#include "Gameplay/Pieces/Piece.h"
#include "Gameplay/Player.h"
#include "Gameplay/GameHistory.h"

namespace Sascha {
namespace Gameplay {

class Move;

class Board : public std::enable_shared_from_this<Board> {
public:
    Board(Player & whitePlayer, Player & blackPlayer) : _whitePlayer(whitePlayer), _blackPlayer(blackPlayer) { }

    /**
     * Prepares the board for a new game, with white to move.
     */
    void setUpStartingPosition();

    /**
     * Sets up the board according to the provided FEN.
     */
    void setUpFromFen(const std::string & fen);

    /**
     * Performs a move on the board and populates data structures so that
     * further moves can be done and analyzed.
     */
    void handleMoveForFullAnalysis(const std::shared_ptr<Move> & move);

    /**
     * Performs a move on the board but only populates information for analyzing
     * that move, and does not support doing further moves.
     */
    void handleMoveForSingleAnalysis(const std::shared_ptr<Move> & move);

    /**
     * Undoes a "full analysis" move.
     */
    void undoFullAnalysisMove(const std::shared_ptr<Move> & move);

    /**
     * Undoes a "single analysis" move.
     */
    void undoSingleAnalysisMove(const std::shared_ptr<Move> & move);

    bool isCheck() const { return _isCheck; }
    bool isSquareAttacked(const Position & square, Color attackerColor) const;
    bool canSquaresSeeEachOtherOnColumn(Position pos1, Position pos2);
    bool canSquaresSeeEachOtherOnRow(Position pos1, Position pos2);
    bool canSquaresSeeEachOtherOnDiagonalUp(Position pos1, Position pos2);
    bool canSquaresSeeEachOtherOnDiagonalDown(Position pos1, Position pos2);
    Color whosTurnToGo() const { return _whosTurnToGo; }
    bool isSquareEmpty(const Position pos) const;
    bool isSquarePieceColor(const Position pos, Color color) const;
    bool testMoveForLegality(const std::shared_ptr<Move> move);
    bool hasEnPassantTarget() const { return _hasEnPassantTarget; }
    bool enPassantTarget(Position & position) const { position = _enPassantTarget; return _hasEnPassantTarget; }
    std::shared_ptr<Piece> pieceAt(const Position & position) const;
    std::vector<std::shared_ptr<Piece>> getAllAttackerOfTypeOnSquare(PieceType pieceType, Color color, Position target);
    Player & whitePlayer() const { return _whitePlayer; }
    Player & blackPlayer() const { return _blackPlayer; }
    std::vector<std::shared_ptr<Pieces::Piece>> pieces() { return _pieces; }
    bool isCheckmate();
    bool isStalemate();
    const std::vector<std::shared_ptr<Move>> & getLegalMoves() const;
    void _calculateLegalMoves();
    void _calculateHasLegalMove();

private:
    void _handleMove(const std::shared_ptr<Move> & move);
    void _undoMove(const std::shared_ptr<Move> & move);
    void _calculateFen();
    void _doubleCheckFen();
    bool _scanForCheck(bool otherPlayer = false);

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
    GameHistory _gameHistory;
    std::vector<std::shared_ptr<Move>> _legalMoves;
    bool _hasLegalMove;

    // Previous board state
    int _prevHalfmoveClock;
};

}
}

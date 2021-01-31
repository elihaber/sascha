#include <sstream>
#include "Gameplay/Board.h"
#include "Gameplay/Pieces/Piece.h"
#include "Gameplay/Pieces/BlankPiece.h"
#include "Gameplay/Move.h"
#include "Globals.h"
#include "Gameplay/Position.h"

using Sascha::Gameplay::Pieces::BlankPiece;

namespace Sascha {
namespace Gameplay {

void Board::setUpFromFen(const std::string & fen) {
    _gameHistory.clear();
    _gameHistory.addStartupFen(fen);
    auto blankPiece =std::make_shared<Pieces::BlankPiece>(shared_from_this());
    _pieces.clear();
    for (size_t i = 0; i < 64; ++i) {
        Position pos;
        Position::indexToInternal(i, pos);
        _pieces.push_back(Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, pos.col, pos.row, shared_from_this()));
    }
    //_pieces.assign(64, blankPiece);
    int pos = 0;
    int col = 0;
    int row = 7;
    char currChar = fen[pos];
    while (currChar == ' ') {
        currChar = fen[++pos];
    }
    while (currChar != ' ') {
        if (currChar == 'r') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::ROOK, Color::BLACK, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'n') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::KNIGHT, Color::BLACK, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'b') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::BISHOP, Color::BLACK, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'q') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::QUEEN, Color::BLACK, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'k') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::KING, Color::BLACK, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'p') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::PAWN, Color::BLACK, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'R') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::ROOK, Color::WHITE, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'N') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::KNIGHT, Color::WHITE, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'B') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::BISHOP, Color::WHITE, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'Q') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::QUEEN, Color::WHITE, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'K') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::KING, Color::WHITE, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == 'P') {
            _pieces[Position::internalToIndex(Position(col, row))] = Pieces::Piece::createPiece(PieceType::PAWN, Color::WHITE, col, row, shared_from_this());
            ++col;
        }
        else if (currChar == '/') {
            --row;
            col = 0;
        }
        else if (currChar >= '1' && currChar <= '8') {
            col += (currChar - '0');
        }
        currChar = fen[++pos];
    }

    while (currChar == ' ') {
        currChar = fen[++pos];
    }

    if (currChar == 'w') {
        _whosTurnToGo = Color::WHITE;
    }
    else {
        _whosTurnToGo = Color::BLACK;
    }

    currChar = fen[++pos];
    while (currChar == ' ') {
        currChar = fen[++pos];
    }

    _whitePlayer.setCastlingRightsKingSide(false);
    _whitePlayer.setCastlingRightsQueenSide(false);
    _blackPlayer.setCastlingRightsKingSide(false);
    _blackPlayer.setCastlingRightsQueenSide(false);
    while (currChar != ' ') {
        if (currChar == '-') {
            // do nothing
        }
        else if (currChar == 'k') {
            _blackPlayer.setCastlingRightsKingSide(true);
        }
        else if (currChar == 'q') {
            _blackPlayer.setCastlingRightsQueenSide(true);
        }
        else if (currChar == 'K') {
            _whitePlayer.setCastlingRightsKingSide(true);
        }
        else if (currChar == 'Q') {
            _whitePlayer.setCastlingRightsQueenSide(true);
        }
        currChar = fen[++pos];
    }

    while (currChar == ' ') {
        currChar = fen[++pos];
    }

    //currChar = fen[++pos];
    if (currChar == '-') {
        _hasEnPassantTarget = false;
    }
    else {
        _hasEnPassantTarget = true;
        Position::algebraicToInternal(fen.substr(pos, pos + 2), _enPassantTarget);
        currChar = fen[++pos];
    }

    currChar = fen[++pos];
    while (currChar == ' ') {
        currChar = fen[++pos];
    }
    int nextBlankPos = pos;
    while (fen[nextBlankPos] != ' ') {
        ++nextBlankPos;
    }

    _halfMoveClock = std::stoi(fen.substr(pos, nextBlankPos));

    pos = nextBlankPos;
    currChar = fen[pos];
    while (currChar == ' ') {
        currChar = fen[++pos];
    }

    nextBlankPos = pos;
    while (nextBlankPos < fen.size() && fen[nextBlankPos] != ' ') {
        ++nextBlankPos;
    }

    _fullMoveNumber = std::stoi(fen.substr(pos, nextBlankPos));
}

void Board::setUpStartingPosition() {
    setUpFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void Board::handleMove(const std::shared_ptr<Move> & move, bool isLegalityTest) {
    MAINLOG("Handling move " << move->algebraicFormat())
    Position startPos = move->source();
    Position endPos = move->target();
    int startIndex = Position::internalToIndex(startPos);
    int endIndex = Position::internalToIndex(endPos);

    auto & movedPiece = _pieces[startIndex];

    if (!move->isCastle()) {
        if (move->pieceType() == PieceType::KING) {
            if (movedPiece->color() == Color::WHITE) {
                if (!_whitePlayer.hasCastled() && _whitePlayer.hasCastlingRightsKingSide() && _whitePlayer.hasCastlingRightsQueenSide()) {
                    _whitePlayer.setCastlingRightsKingSide(false);
                    _whitePlayer.setCastlingRightsQueenSide(false);
                    move->setRuinedCastling(CastleSide::BOTH);
                }
                else if (!_whitePlayer.hasCastled() && _whitePlayer.hasCastlingRightsKingSide()) {
                    _whitePlayer.setCastlingRightsKingSide(false);
                    move->setRuinedCastling(CastleSide::KINGSIDE);
                }
                else if (!_whitePlayer.hasCastled() && _whitePlayer.hasCastlingRightsQueenSide()) {
                    _whitePlayer.setCastlingRightsQueenSide(false);
                    move->setRuinedCastling(CastleSide::QUEENSIDE);
                }
            }
            if (movedPiece->color() == Color::BLACK) {
                if (!_blackPlayer.hasCastled() && _blackPlayer.hasCastlingRightsKingSide() && _blackPlayer.hasCastlingRightsQueenSide()) {
                    _blackPlayer.setCastlingRightsKingSide(false);
                    _blackPlayer.setCastlingRightsQueenSide(false);
                    move->setRuinedCastling(CastleSide::BOTH);
                }
                else if (!_blackPlayer.hasCastled() && _blackPlayer.hasCastlingRightsKingSide()) {
                    _blackPlayer.setCastlingRightsKingSide(false);
                    move->setRuinedCastling(CastleSide::KINGSIDE);
                }
                else if (!_blackPlayer.hasCastled() && _blackPlayer.hasCastlingRightsQueenSide()) {
                    _blackPlayer.setCastlingRightsQueenSide(false);
                    move->setRuinedCastling(CastleSide::QUEENSIDE);
                }
            }
        }
        else if (move->pieceType() == PieceType::ROOK) {
            if (move->source() == Position(0, 0) && move->color() == Color::WHITE && _whitePlayer.hasCastlingRightsQueenSide() && !_whitePlayer.hasCastled()) {
                _whitePlayer.setCastlingRightsQueenSide(false);
                move->setRuinedCastling(CastleSide::QUEENSIDE);
            }
            else if (move->source() == Position(7, 0) && move->color() == Color::WHITE && _whitePlayer.hasCastlingRightsKingSide() && !_whitePlayer.hasCastled()) {
                _whitePlayer.setCastlingRightsKingSide(false);
                move->setRuinedCastling(CastleSide::KINGSIDE);
            }
            else if (move->source() == Position(0, 7) && move->color() == Color::BLACK && _blackPlayer.hasCastlingRightsQueenSide() && !_blackPlayer.hasCastled()) {
                _blackPlayer.setCastlingRightsQueenSide(false);
                move->setRuinedCastling(CastleSide::QUEENSIDE);
            }
            else if (move->source() == Position(7, 7) && move->color() == Color::BLACK && _blackPlayer.hasCastlingRightsKingSide() && !_blackPlayer.hasCastled()) {
                _blackPlayer.setCastlingRightsKingSide(false);
                move->setRuinedCastling(CastleSide::KINGSIDE);
            }
        }
    }
    else {
        if (move->color() == Color::WHITE) {
            if (_whitePlayer.hasCastlingRightsQueenSide() && _whitePlayer.hasCastlingRightsKingSide()) {
                _whitePlayer.setCastlingRightsQueenSide(false);
                _whitePlayer.setCastlingRightsKingSide(false);
                move->setRuinedCastling(CastleSide::BOTH);
            }
            else if (_whitePlayer.hasCastlingRightsQueenSide()) {
                _whitePlayer.setCastlingRightsQueenSide(false);
                move->setRuinedCastling(CastleSide::QUEENSIDE);
            }
            else if (_whitePlayer.hasCastlingRightsKingSide()) {
                _whitePlayer.setCastlingRightsKingSide(false);
                move->setRuinedCastling(CastleSide::KINGSIDE);
            }
        }
        else {
            if (_blackPlayer.hasCastlingRightsQueenSide() && _blackPlayer.hasCastlingRightsKingSide()) {
                _blackPlayer.setCastlingRightsQueenSide(false);
                _blackPlayer.setCastlingRightsKingSide(false);
                move->setRuinedCastling(CastleSide::BOTH);
            }
            else if (_blackPlayer.hasCastlingRightsQueenSide()) {
                _blackPlayer.setCastlingRightsQueenSide(false);
                move->setRuinedCastling(CastleSide::QUEENSIDE);
            }
            else if (_blackPlayer.hasCastlingRightsKingSide()) {
                _blackPlayer.setCastlingRightsKingSide(false);
                move->setRuinedCastling(CastleSide::KINGSIDE);
            }
        }
    }

    if (_hasEnPassantTarget) {
        move->setRuinedEnPassant(true);
        move->setRuinedEnPassantTarget(_enPassantTarget);
    }

    _hasEnPassantTarget = false;
    bool pawnMove = false;
    
    if (movedPiece->pieceType() == PieceType::PAWN) {
        pawnMove = true;
        if (endPos.row - startPos.row == 2) {
            _hasEnPassantTarget = true;
            _enPassantTarget = Position(endPos.col, endPos.row - 1);
            move->setEnabledEnPassant(true);
            move->setEnabledEnPassantTarget(_enPassantTarget);
        }
        else if (startPos.row - endPos.row == 2) {
            _hasEnPassantTarget = true;
            _enPassantTarget = Position(endPos.col, endPos.row + 1);
            move->setEnabledEnPassant(true);
            move->setEnabledEnPassantTarget(_enPassantTarget);
        }
    }

    bool capture = (_pieces[endIndex]->pieceType() != PieceType::BLANK);

    _prevHalfmoveClock = _halfMoveClock;

    if (pawnMove || capture) {
        _halfMoveClock = 0;
    }

    if (_whosTurnToGo == Color::BLACK) {
        ++_fullMoveNumber;
        _whosTurnToGo = Color::WHITE;
    }
    else {
        _whosTurnToGo = Color::BLACK;
    }

    if (move->isPromotion()) {
        if (move->isCapture()) {
            _gameHistory.addCapturedPiece(_pieces[endIndex]);
        }
        _pieces[endIndex] = Pieces::Piece::createPiece(move->promotionResult(), movedPiece->color(), endPos.col, endPos.row, shared_from_this());
        _gameHistory.addPromotedPawn(_pieces[startIndex]);
        _pieces[startIndex] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
    }
    else if (move->isCastle()) {
        Position rookStartPos(0, 0);
        Position rookEndPos(0, 0);
        if (startPos.row == 0 && endPos.col > startPos.col) {
            rookStartPos.col = 7;
            rookStartPos.row = 0;
            rookEndPos.col = 5;
            rookEndPos.row = 0;
        }
        else if (startPos.row == 0 && endPos.col < startPos.col) {
            rookStartPos.col = 0;
            rookStartPos.row = 0;
            rookEndPos.col = 3;
            rookEndPos.row = 0;
        }
        else if (startPos.row == 7 && endPos.col > startPos.col) {
            rookStartPos.col = 7;
            rookStartPos.row = 7;
            rookEndPos.col = 5;
            rookEndPos.row = 7;
        }
        else {
            rookStartPos.col = 0;
            rookStartPos.row = 7;
            rookEndPos.col = 3;
            rookEndPos.row = 7;
        }
        // King
        _pieces[Position::internalToIndex(endPos)] = _pieces[Position::internalToIndex(startPos)];
        _pieces[Position::internalToIndex(startPos)] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
        _pieces[Position::internalToIndex(endPos)]->setPosition(endPos.col, endPos.row);
        // Rook
        _pieces[Position::internalToIndex(rookEndPos)] = _pieces[Position::internalToIndex(rookStartPos)];
        _pieces[Position::internalToIndex(rookStartPos)] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
        _pieces[Position::internalToIndex(rookEndPos)]->setPosition(rookEndPos.col, rookEndPos.row);
    }
    else {
        if (move->isCapture()) {
            _gameHistory.addCapturedPiece(_pieces[endIndex]);
        }
        else if (move->isEnPassant()) {
            if (move->color() == Color::WHITE) {
                Position enPassantPawnActualPos(_enPassantTarget.col, _enPassantTarget.row - 1);
                _gameHistory.addCapturedPiece(_pieces[Position::internalToIndex(enPassantPawnActualPos)]);
                _pieces[Position::internalToIndex(enPassantPawnActualPos)] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
            }
            else {
                Position enPassantPawnActualPos(_enPassantTarget.col, _enPassantTarget.row + 1);
                _gameHistory.addCapturedPiece(_pieces[Position::internalToIndex(enPassantPawnActualPos)]);
                _pieces[Position::internalToIndex(enPassantPawnActualPos)] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
            }
        }
        else {
        }
        _pieces[endIndex] = _pieces[startIndex];
        _pieces[endIndex]->setPosition(endPos.col, endPos.row);
        _pieces[startIndex] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
    }

    _isCheck = scanForCheck();

    _calculateFen();
    _doubleCheckFen();

    MAINLOG("About to save FEN: " << _fen)

    _gameHistory.addMove(move, _fen);

    if (!isLegalityTest) {
        _calculateLegalMoves();
    }

}

void Board::handleMoveUciFormat(const std::string & uciFormat) {
    auto move = std::make_shared<Move>(uciFormat, shared_from_this());
    handleMove(move);
}

void Board::undoMove(const std::shared_ptr<Move> & move, bool isLegalityTest) {
    if (_gameHistory.isEmpty()) {
        MAINLOG("ERROR: Attempt to undo move when hisory is empty");
        throw std::runtime_error("ERROR: Attempt to undo move when hisory is empty");
    }

    std::shared_ptr<Move> lastMove;
    if (!_gameHistory.lastMove(lastMove)) {
        MAINLOG("ERROR: Attempt to get last move from history returned false");
        throw std::runtime_error("ERROR: Attempt to get last move from history returned false");
    }

    if (!(*lastMove == *move)) {
        MAINLOG("ERROR: Last move " << lastMove->algebraicFormat() << " does not match undo move " << move->algebraicFormat());
        throw std::runtime_error("ERROR: Last move does not match undo move");
    }

    std::string lastFen;
    if (!_gameHistory.lastFen(lastFen)) {
        MAINLOG("ERROR: Attempt to get last FEN from history returned false");
        throw std::runtime_error("ERROR: Attempt to get last FEN from history returned false");
    }

    std::shared_ptr<Piece> lastCapturedPiece;
    if (lastMove->isCapture() || lastMove->isEnPassant()) {
        if (!_gameHistory.lastCapturedPiece(lastCapturedPiece)) {
            MAINLOG("ERROR: No last captured piece but last move was a capture");
            throw std::runtime_error("ERROR: No last captured piece but last move was a capture");
        }
    }
    else {
        lastCapturedPiece = Piece::createPiece(PieceType::BLANK, Color::WHITE, 0, 0, shared_from_this());
    }

    std::shared_ptr<Piece> lastPromotedPawn;
    if (lastMove->isPromotion()) {
        if (!_gameHistory.lastPromotedPawn(lastPromotedPawn)) {
            MAINLOG("ERROR: No last promoted pawn but last move was a promotion");
            throw std::runtime_error("ERROR: No last promoted pawn but last move was a promotion");
        }
    }
    else {
        lastPromotedPawn = Piece::createPiece(PieceType::BLANK, Color::WHITE, 0, 0, shared_from_this());
    }

    _gameHistory.undoLastMove();

    if (_gameHistory.isEmpty()) {
        MAINLOG("ERROR: History is empty after undoing last move");
        throw std::runtime_error("ERROR: History is empty after undoing last move");
    }

    if (!_gameHistory.lastFen(lastFen)) {
        MAINLOG("ERROR: Attempt to get last FEN from history returned false after deleting previous move");
        throw std::runtime_error("ERROR: Attempt to get last FEN from history returned false after deleting previous move");
    }

    MAINLOG("About to undo move by setting up from FEN: " << lastFen);

    //setUpFromFenIncremental(lastFen, lastMove, lastCapturedPiece);
    _undoMoveOnBoard(lastMove, lastFen, lastCapturedPiece, lastPromotedPawn, isLegalityTest);
}

void Board::_undoMoveOnBoard(const std::shared_ptr<Move> & move, const std::string & fen, const std::shared_ptr<Piece> & capturedPiece, const std::shared_ptr<Piece> & promotedPawn, bool isLegalityTest) {
    MAINLOG("Undoing move " << move->algebraicFormat())
    if (move->pieceType() == PieceType::KING) {
        MAINLOG("HERE")
    }
    _calculateFen();
    std::string fenBeforeUndo = _fen;

    Position startPos = move->source();
    Position endPos = move->target();
    int startIndex = Position::internalToIndex(startPos);
    int endIndex = Position::internalToIndex(endPos);

    if (move->isPromotion()) {
        _pieces[startIndex] = promotedPawn;
        if (move->isCapture()) {
            _pieces[endIndex] = capturedPiece;
        }
        else {
            _pieces[endIndex] = Piece::createPiece(PieceType::BLANK, Color::WHITE, endPos.col, endPos.row, shared_from_this());
        }
    }
    else if (move->isCastle()) {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        if (move->color() == Color::WHITE) {
            if (move->castleSide() == CastleSide::KINGSIDE) {
                _pieces[Position::internalToIndex(Position(7, 0))] = _pieces[Position::internalToIndex(Position(5, 0))];
                _pieces[Position::internalToIndex(Position(7, 0))]->setPosition(7, 0);
                _pieces[Position::internalToIndex(Position(4, 0))] = _pieces[Position::internalToIndex(Position(6, 0))];
                _pieces[Position::internalToIndex(Position(4, 0))]->setPosition(4, 0);
                _pieces[Position::internalToIndex(Position(5, 0))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 5, 0, shared_from_this());
                _pieces[Position::internalToIndex(Position(6, 0))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 6, 0, shared_from_this());
            }
            else {
                _pieces[Position::internalToIndex(Position(0, 0))] = _pieces[Position::internalToIndex(Position(3, 0))];
                _pieces[Position::internalToIndex(Position(0, 0))]->setPosition(0, 0);
                _pieces[Position::internalToIndex(Position(4, 0))] = _pieces[Position::internalToIndex(Position(2, 0))];
                _pieces[Position::internalToIndex(Position(4, 0))]->setPosition(4, 0);
                _pieces[Position::internalToIndex(Position(1, 0))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 1, 0, shared_from_this());
                _pieces[Position::internalToIndex(Position(2, 0))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 2, 0, shared_from_this());
                _pieces[Position::internalToIndex(Position(3, 0))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 3, 0, shared_from_this());
            }
        }
        else {
            if (move->castleSide() == CastleSide::KINGSIDE) {
                _pieces[Position::internalToIndex(Position(7, 7))] = _pieces[Position::internalToIndex(Position(5, 7))];
                _pieces[Position::internalToIndex(Position(7, 7))]->setPosition(7, 7);
                _pieces[Position::internalToIndex(Position(4, 7))] = _pieces[Position::internalToIndex(Position(6, 7))];
                _pieces[Position::internalToIndex(Position(4, 7))]->setPosition(4, 7);
                _pieces[Position::internalToIndex(Position(5, 7))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 5, 7, shared_from_this());
                _pieces[Position::internalToIndex(Position(6, 7))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 6, 7, shared_from_this());
            }
            else {
                _pieces[Position::internalToIndex(Position(0, 7))] = _pieces[Position::internalToIndex(Position(3, 7))];
                _pieces[Position::internalToIndex(Position(0, 7))]->setPosition(0, 7);
                _pieces[Position::internalToIndex(Position(4, 7))] = _pieces[Position::internalToIndex(Position(2, 7))];
                _pieces[Position::internalToIndex(Position(4, 7))]->setPosition(4, 7);
                _pieces[Position::internalToIndex(Position(1, 7))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 1, 7, shared_from_this());
                _pieces[Position::internalToIndex(Position(2, 7))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 2, 7, shared_from_this());
                _pieces[Position::internalToIndex(Position(3, 7))] = Piece::createPiece(PieceType::BLANK, Color::WHITE, 3, 7, shared_from_this());
            }
        }
        if (move->color() == Color::WHITE) {
            _whitePlayer.setHasCastled(false);
        }
        else {
            _blackPlayer.setHasCastled(false);
        }
    }
    else if (move->isCapture()) {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        _pieces[endIndex] = capturedPiece;
    }
    else if (move->isEnPassant()) {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        if (move->color() == Color::WHITE) {
            Position locationOfCapturedPawn(endPos.col, endPos.row - 1);
            _pieces[Position::internalToIndex(locationOfCapturedPawn)] = capturedPiece;
        }
        else {
            Position locationOfCapturedPawn(endPos.col, endPos.row + 1);
            _pieces[Position::internalToIndex(locationOfCapturedPawn)] = capturedPiece;
        }
        _pieces[endIndex] = Piece::createPiece(PieceType::BLANK, Color::WHITE, endPos.col, endPos.row, shared_from_this());
    }
    else {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        _pieces[endIndex] = Piece::createPiece(PieceType::BLANK, Color::WHITE, endPos.col, endPos.row, shared_from_this());
    }

    if (move->enabledEnPassant()) {
        _hasEnPassantTarget = false;
        _enPassantTarget = Position(0, 0);
    }
    if (move->ruinedEnPassant()) {
        MAINLOG("Undoing move that ruined en passant")
        _hasEnPassantTarget = true;
        _enPassantTarget = move->ruinedEnPassantTarget();
        MAINLOG("Setting board en passant target to " << _enPassantTarget.col << "," << _enPassantTarget.row)
    }

    if (move->color() == Color::BLACK) {
        --_fullMoveNumber;
    }

    _whosTurnToGo = move->color();
    _halfMoveClock = _prevHalfmoveClock;

    if (move->ruinedCastling() == CastleSide::BOTH) {
        if (move->color() == Color::WHITE) {
            _whitePlayer.setCastlingRightsKingSide(true);
            _whitePlayer.setCastlingRightsQueenSide(true);
        }
        else {
            _blackPlayer.setCastlingRightsKingSide(true);
            _blackPlayer.setCastlingRightsQueenSide(true);
        }
    }
    else if (move->ruinedCastling() == CastleSide::KINGSIDE) {
        if (move->color() == Color::WHITE) {
            _whitePlayer.setCastlingRightsKingSide(true);
        }
        else {
            _blackPlayer.setCastlingRightsKingSide(true);
        }
    }
    else if (move->ruinedCastling() == CastleSide::QUEENSIDE) {
        if (move->color() == Color::WHITE) {
            _whitePlayer.setCastlingRightsQueenSide(true);
        }
        else {
            _blackPlayer.setCastlingRightsQueenSide(true);
        }
    }

    _calculateFen();

    if (_fen.compare(fen) != 0) {
        MAINLOG("ERROR ERROR ERROR !!!!! Fen does not match after undo")
        MAINLOG("Expected fen:")
        MAINLOG(fen)
        MAINLOG("Fen after undo:")
        MAINLOG(_fen)
        MAINLOG("Fen before undo:")
        MAINLOG(fenBeforeUndo)
        MAINLOG("Move was " << move->algebraicFormat())
        throw std::runtime_error("ERROR: Fen does not match after undo");
    }

    _isCheck = scanForCheck();
    if (!isLegalityTest) {
        _calculateLegalMoves();
    }
}

bool Board::scanForCheck(bool otherPlayer) {
    // Find the king whose turn it is
    std::shared_ptr<Pieces::Piece> king;
    if (!otherPlayer) {
//        MAINLOG("Looking for king that is of color " << (int)_whosTurnToGo)
    } else {
//        MAINLOG("Looking for king that is NOT of color " << (int)_whosTurnToGo)
    }
    for (auto piece : _pieces) {
        if (!otherPlayer) {
            if (piece->pieceType() == PieceType::KING && piece->color() == _whosTurnToGo) {
                king = piece;
                break;
            }
        }
        else {
            if (piece->pieceType() == PieceType::KING && piece->color() != _whosTurnToGo) {
                king = piece;
                break;
            }
        }
    }

//    MAINLOG("Checking if king on square " << king->position().col << "," << king->position().row << " is attacked")
    return isSquareAttacked(king->position(), oppositeColor(king->color()));
}

void Board::calculateLegalMoves(std::vector<std::shared_ptr<Move>> moves) const {

}

bool Board::isSquareAttacked(const Position & square, Color attackerColor) const {
    int squareCol = square.col;
    int squareRow = square.row;

//    MAINLOG("Checking if position " << squareCol << "," << squareRow << " is attacked by " << _pieces.size() << " pieces")
    for (auto piece : _pieces) {
        if (piece->pieceType() == PieceType::BLANK || piece->color() != attackerColor) {
            continue;
        }
        if (piece->isAttackingSquare(square)) {
//            MAINLOG("Piece type " << (int)piece->pieceType() << " on " << piece->position().col << "," << piece->position().row << " is attacking")
            return true;
        }
        else {
//            MAINLOG("Piece type " << (int)piece->pieceType() << " on " << piece->position().col << "," << piece->position().row << " is NOT attacking")
        }
    }
    return false;
}

bool Board::canSquaresSeeEachOtherOnColumn(Position pos1, Position pos2) {
    int col = pos1.col;
    int startRow = std::min(pos1.row, pos2.row);
    int endRow = std::max(pos1.row, pos2.row);

    for (int i = startRow + 1; i < endRow; ++i) {
        if (_pieces[Position::internalToIndex(Position(col, i))]->pieceType() != PieceType::BLANK) {
            return false;
        }
    }
    return true;
}

bool Board::canSquaresSeeEachOtherOnRow(Position pos1, Position pos2) {
    int row = pos1.row;
    int startCol = std::min(pos1.col, pos2.col);
    int endCol = std::max(pos1.col, pos2.col);

    for (int i = startCol + 1; i < endCol; ++i) {
        if (_pieces[Position::internalToIndex(Position(i, row))]->pieceType() != PieceType::BLANK) {
            return false;
        }
    }
    return true;
}

bool Board::canSquaresSeeEachOtherOnDiagonalUp(Position pos1, Position pos2) {
    Position startPos = pos1;
    Position endPos = pos2;
    if (pos2.col < pos1.col) {
        startPos = pos2;
        endPos = pos1;
    }

    Position curPos = Position(startPos.col + 1, startPos.row + 1);
    while (curPos.col < endPos.col && curPos.row < endPos.row) {
        if (_pieces[Position::internalToIndex(curPos)]->pieceType() != PieceType::BLANK) {
            return false;
        }
        curPos = Position(curPos.col + 1, curPos.row + 1);
    }
    return true;
}

bool Board::canSquaresSeeEachOtherOnDiagonalDown(Position pos1, Position pos2) {
    Position startPos = pos1;
    Position endPos = pos2;
    if (pos2.col < pos1.col) {
        startPos = pos2;
        endPos = pos1;
    }

    Position curPos = Position(startPos.col + 1, startPos.row - 1);
    while (curPos.col < endPos.col && curPos.row > endPos.row) {
        if (_pieces[Position::internalToIndex(curPos)]->pieceType() != PieceType::BLANK) {
            return false;
        }
        curPos = Position(curPos.col + 1, curPos.row - 1);
    }
    return true;
}

bool Board::isSquareEmpty(const Position pos) const {
    return (_pieces[Position::internalToIndex(pos)]->pieceType() == PieceType::BLANK);
}

bool Board::isSquarePieceColor(const Position pos, Color color) const {
    return (!isSquareEmpty(pos) && _pieces[Position::internalToIndex(pos)]->color() == color);
}

bool Board::testMoveForLegality(const std::shared_ptr<Move> move) {
    if (_pieces[Position::internalToIndex(move->target())]->pieceType() == PieceType::KING) {
        // Cannot capture a king!
        return false;
    }
    MAINLOG("Testing move for legality: " << move->algebraicFormat())
    handleMove(move, true);
//    MAINLOG("Player who is to move: " << (int)_whosTurnToGo)
    bool result = !scanForCheck(true);
    undoMove(move, true);
    if (result) {
        MAINLOG("Move is legal")
    } else {
        MAINLOG("Move is not legal")
    }
    return result;
}

int Board::findPiecePosByPosition(const Position & pos) {
    for (size_t i = 0; i < _pieces.size(); ++i) {
        if (_pieces[i]->position() == pos) {
            return i;
        }
    }
    return -1;
}

void Board::_calculateLegalMoves() {
    _legalMoves.clear();
    for (const auto & piece : _pieces) {
        if (piece->color() != _whosTurnToGo) {
            continue;
        }

        std::vector<std::shared_ptr<Move>> pieceMoves;
        piece->getPossibleMoves(pieceMoves);
        _legalMoves.insert(_legalMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }
}

const std::vector<std::shared_ptr<Move>> & Board::getLegalMoves() const {
    return _legalMoves;
}

//void Board::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
//    for (const auto & piece : _pieces) {
//        if (piece->color() != _whosTurnToGo) {
//            continue;
//        }
//
//        std::vector<std::shared_ptr<Move>> pieceMoves;
//        piece->getPossibleMoves(pieceMoves);
//        possibleMoves.insert(possibleMoves.end(), pieceMoves.begin(), pieceMoves.end());
//    }
//}

void Board::_calculateFen() {
    std::stringstream sstr;
    for (int row = 7; row >= 0; --row) {
        int col = 0;
        int blankCount = 0;
        while (col < 8) {
            int index = Position::internalToIndex(Position(col, row));
            if (_pieces[index]->pieceType() != PieceType::BLANK) {
                if (blankCount > 0) {
                    sstr << blankCount;
                }
                blankCount = 0;
            }
            if (_pieces[index]->pieceType() == PieceType::PAWN) {
                sstr << (_pieces[index]->color() == Color::WHITE ? 'P' : 'p');
            }
            else if (_pieces[index]->pieceType() == PieceType::KNIGHT) {
                sstr << (_pieces[index]->color() == Color::WHITE ? 'N' : 'n');
            }
            else if (_pieces[index]->pieceType() == PieceType::BISHOP) {
                sstr << (_pieces[index]->color() == Color::WHITE ? 'B' : 'b');
            }
            else if (_pieces[index]->pieceType() == PieceType::ROOK) {
                sstr << (_pieces[index]->color() == Color::WHITE ? 'R' : 'r');
            }
            else if (_pieces[index]->pieceType() == PieceType::QUEEN) {
                sstr << (_pieces[index]->color() == Color::WHITE ? 'Q' : 'q');
            }
            else if (_pieces[index]->pieceType() == PieceType::KING) {
                sstr << (_pieces[index]->color() == Color::WHITE ? 'K' : 'k');
            }
            else {
                ++blankCount;
            }
            ++col;
        }
        if (blankCount > 0) {
            sstr << blankCount;
        }
        if (row != 0) {
            sstr << '/';
        }
    }
    sstr << ' ';
    if (_whosTurnToGo == Color::WHITE) {
        sstr << 'w';
    }
    else {
        sstr << 'b';
    }
    sstr << ' ';
    bool someoneCanCastle = false;
    if (_whitePlayer.hasCastlingRightsKingSide()) {
        sstr << 'K';
        someoneCanCastle = true;
    }
    if (_whitePlayer.hasCastlingRightsQueenSide()) {
        sstr << 'Q';
        someoneCanCastle = true;
    }
    if (_blackPlayer.hasCastlingRightsKingSide()) {
        sstr << 'k';
        someoneCanCastle = true;
    }
    if (_blackPlayer.hasCastlingRightsQueenSide()) {
        sstr << 'q';
        someoneCanCastle = true;
    }
    if (!someoneCanCastle) {
        sstr << '-';
    }
    sstr << ' ';
    if (_hasEnPassantTarget) {
        std::string pos;
        Position::internalToAlgebraic(_enPassantTarget, pos);
        sstr << pos;
    }
    else {
        sstr << '-';
    }
    sstr << ' ' << _halfMoveClock << ' ' << _fullMoveNumber;
    _fen = sstr.str();
}

bool Board::isCheckmate() {
    if (!_isCheck) {
        return false;
    }
    return (_legalMoves.size() == 0);
}

bool Board::isStalemate() {
    if (_isCheck) {
        return false;
    }
    return (_legalMoves.size() == 0);
}

void Board::_doubleCheckFen() {
}

std::shared_ptr<Piece> Board::pieceAt(const Position & position) const {
    return _pieces[Position::internalToIndex(position)];
}

std::vector<std::shared_ptr<Piece>> Board::getAllAttackerOfTypeOnSquare(PieceType pieceType, Color color, Position target) {
    std::vector<std::shared_ptr<Piece>> attackers;
    for (auto piece : _pieces) {
        if (piece->pieceType() != pieceType || piece->color() != color) {
            continue;
        }
        if (piece->isAttackingSquare(target)) {
            attackers.push_back(piece);
        }
    }

    return attackers;
}

}
}

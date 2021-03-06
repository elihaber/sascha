#include <sstream>
#include "Gameplay/Board.h"
#include "Gameplay/Pieces/Piece.h"
#include "Gameplay/Pieces/BlankPiece.h"
#include "Gameplay/Move.h"
#include "Globals.h"
#include "Gameplay/Position.h"

using Sascha::Gameplay::Pieces::BlankPiece;
using Sascha::Gameplay::Pieces::Piece;

namespace Sascha {
namespace Gameplay {

void Board::setUpFromFen(const std::string & fen) {
    MAINLOG("Setting up board from FEN: " << fen)

    _pieces.clear();
    _gameHistory.clear();
    _gameHistory.addStartupFen(fen);
    
    Piece::CreateBlankPiece(shared_from_this()); // TODO: This should not be necessary -- the Piece class should automatically create the BlankPiece object.
    _pieces.clear();
    for (size_t i = 0; i < 64; ++i) {
        Position pos;
        Position::indexToInternal(i, pos);
        _pieces.push_back(Pieces::Piece::GetBlankPiece());
    }
    
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

    setCastlingRights(Color::WHITE, CastleSide::KINGSIDE, false);
    setCastlingRights(Color::WHITE, CastleSide::QUEENSIDE, false);
    setCastlingRights(Color::BLACK, CastleSide::KINGSIDE, false);
    setCastlingRights(Color::BLACK, CastleSide::QUEENSIDE, false);

    while (currChar != ' ') {
        if (currChar == '-') {
            // do nothing
        }
        else if (currChar == 'k') {
            setCastlingRights(Color::BLACK, CastleSide::KINGSIDE, true);
        }
        else if (currChar == 'q') {
            setCastlingRights(Color::BLACK, CastleSide::QUEENSIDE, true);
        }
        else if (currChar == 'K') {
            setCastlingRights(Color::WHITE, CastleSide::KINGSIDE, true);
        }
        else if (currChar == 'Q') {
            setCastlingRights(Color::WHITE, CastleSide::QUEENSIDE, true);
        }
        currChar = fen[++pos];
    }

    while (currChar == ' ') {
        currChar = fen[++pos];
    }

    if (currChar == '-') {
        _hasEnPassantTarget = false;
    }
    else {
        _hasEnPassantTarget = true;
        Position::uciToInternal(fen.substr(pos, pos + 2), _enPassantTarget);
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

void Board::handleMoveForFullAnalysis(const std::shared_ptr<Move> & move) {
    MAINLOG("Handling move for full analysis " << move->algebraicFormat())
    _handleMove(move);
    _calculateLegalMoves();
}

void Board::handleMoveForSingleAnalysis(const std::shared_ptr<Move> & move) {
    MAINLOG("Handling move for single analysis " << move->algebraicFormat())
    _handleMove(move);
    _calculateHasLegalMove();
}

void Board::_handleMove(const std::shared_ptr<Move> & move) {
    Position startPos = move->source();
    Position endPos = move->target();
    int startIndex = Position::internalToIndex(startPos);
    int endIndex = Position::internalToIndex(endPos);

    auto & movedPiece = _pieces[startIndex];

    move->setRuinedCastling(CastleSide::KINGSIDE, false);
    move->setRuinedCastling(CastleSide::QUEENSIDE, false);

    if (move->isCastle() || move->pieceType() == PieceType::KING) {
        for (int i = 0; i < 2; ++i) {
            int arrayPos = colorToInt(move->color()) * 2 + i;
            if (castlingRights(move->color(), intToCastleSide(i))) {
                setCastlingRights(move->color(), intToCastleSide(i), false);
                move->setRuinedCastling(intToCastleSide(i), true);
            }
        }
    }
    else if (move->pieceType() == PieceType::ROOK) {
        int baseRow = (move->color() == Color::WHITE ? 0 : 7);
        if (move->source().row == baseRow && (move->source().col == 0 || move->source().col == 7)) {
            auto castleSide = (move->source().col == 0 ? CastleSide::QUEENSIDE : CastleSide::KINGSIDE);
            if (castlingRights(move->color(), castleSide)) {
                setCastlingRights(move->color(), castleSide, false);
                move->setRuinedCastling(castleSide, true);
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
        if (abs(endPos.row - startPos.row) == 2) {
            _hasEnPassantTarget = true;
            _enPassantTarget = Position(endPos.col, (startPos.row == 1 ? startPos.row + 1 : startPos.row - 1));
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
    }
    
    _whosTurnToGo = oppositeColor(_whosTurnToGo);

    if (move->isPromotion()) {
        if (move->isCapture()) {
            _gameHistory.addCapturedPiece(_pieces[endIndex]);
        }
        _pieces[endIndex] = Pieces::Piece::createPiece(move->promotionResult(), movedPiece->color(), endPos.col, endPos.row, shared_from_this());
        _gameHistory.addPromotedPawn(_pieces[startIndex]);
        _pieces[startIndex] = Pieces::Piece::GetBlankPiece();
    }
    else if (move->isCastle()) {
        Position rookStartPos(0, startPos.row);
        Position rookEndPos(0, startPos.row);

        if (endPos.col > startPos.col) {
            rookStartPos.col = 7;
            rookEndPos.col = 5;
        }
        else {
            rookStartPos.col = 0;
            rookEndPos.col = 3;
        }

        // King
        _pieces[Position::internalToIndex(endPos)] = _pieces[Position::internalToIndex(startPos)];
        _pieces[Position::internalToIndex(startPos)] = Pieces::Piece::GetBlankPiece();
        _pieces[Position::internalToIndex(endPos)]->setPosition(endPos.col, endPos.row);

        // Rook
        _pieces[Position::internalToIndex(rookEndPos)] = _pieces[Position::internalToIndex(rookStartPos)];
        _pieces[Position::internalToIndex(rookStartPos)] = Pieces::Piece::GetBlankPiece();
        _pieces[Position::internalToIndex(rookEndPos)]->setPosition(rookEndPos.col, rookEndPos.row);
    }
    else {
        if (move->isCapture()) {
            _gameHistory.addCapturedPiece(_pieces[endIndex]);
        }
        else if (move->isEnPassant()) {
            Position enPassantPawnActualPos(_enPassantTarget.col, (move->color() == Color::WHITE ? _enPassantTarget.row - 1 : _enPassantTarget.row + 1));
            _gameHistory.addCapturedPiece(_pieces[Position::internalToIndex(enPassantPawnActualPos)]);
            _pieces[Position::internalToIndex(enPassantPawnActualPos)] = Pieces::Piece::GetBlankPiece();
        }
        _pieces[endIndex] = _pieces[startIndex];
        _pieces[endIndex]->setPosition(endPos.col, endPos.row);
        _pieces[startIndex] = Pieces::Piece::GetBlankPiece();
    }

    _isCheck = _scanForCheck();

    _calculateFen();
    MAINLOG("FEN: " << _fen)

    _gameHistory.addMove(move, _fen);
}

void Board::undoFullAnalysisMove(const std::shared_ptr<Move> & move) {
    MAINLOG("Undoing full analysis move " << move->algebraicFormat())
    _undoMove(move);
    _calculateLegalMoves();
}

void Board::undoSingleAnalysisMove(const std::shared_ptr<Move> & move) {
    MAINLOG("Undoing single analysis move " << move->algebraicFormat())
    _undoMove(move);
    _calculateHasLegalMove();
}

void Board::_undoMove(const std::shared_ptr<Move> & move) {
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
        lastCapturedPiece = Pieces::Piece::GetBlankPiece();
    }
    std::shared_ptr<Piece> lastPromotedPawn;
    if (lastMove->isPromotion()) {
        if (!_gameHistory.lastPromotedPawn(lastPromotedPawn)) {
            MAINLOG("ERROR: No last promoted pawn but last move was a promotion");
            throw std::runtime_error("ERROR: No last promoted pawn but last move was a promotion");
        }
    }
    else {
        lastPromotedPawn = Pieces::Piece::GetBlankPiece();
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

    MAINLOG("Undoing move " << lastMove->algebraicFormat())

    _calculateFen();
    std::string fenBeforeUndo = _fen;

    Position startPos = lastMove->source();
    Position endPos = lastMove->target();
    int startIndex = Position::internalToIndex(startPos);
    int endIndex = Position::internalToIndex(endPos);

    if (lastMove->isPromotion()) {
        _pieces[startIndex] = lastPromotedPawn;
        if (lastMove->isCapture()) {
            _pieces[endIndex] = lastCapturedPiece;
        }
        else {
            _pieces[endIndex] = Pieces::Piece::GetBlankPiece();
        }
    }
    else if (lastMove->isCastle()) {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        int rookStartCol = (lastMove->castleSide() == CastleSide::KINGSIDE ? 7 : 0);
        int rookEndCol = (lastMove->castleSide() == CastleSide::KINGSIDE ? 5 : 3);
        int kingStartCol = 4;
        int kingEndCol = (lastMove->castleSide() == CastleSide::KINGSIDE ? 6 : 2);
        int emptyCol1 = (lastMove->castleSide() == CastleSide::KINGSIDE ? 5 : 3);
        int emptyCol2 = (lastMove->castleSide() == CastleSide::KINGSIDE ? 6 : 2);
        int row = startPos.row;

        // Move the king back and update the piece's internal position
        _pieces[Position::internalToIndex(Position(rookStartCol, row))] = _pieces[Position::internalToIndex(Position(rookEndCol, row))];
        _pieces[Position::internalToIndex(Position(rookStartCol, row))]->setPosition(rookStartCol, row);

        // Move the rook back and update the piece's internal position
        _pieces[Position::internalToIndex(Position(kingStartCol, row))] = _pieces[Position::internalToIndex(Position(kingEndCol, row))];
        _pieces[Position::internalToIndex(Position(kingStartCol, row))]->setPosition(kingStartCol, row);

        // Set the squares they vacated to blank pieces
        _pieces[Position::internalToIndex(Position(emptyCol1, row))] = Pieces::Piece::GetBlankPiece();
        _pieces[Position::internalToIndex(Position(emptyCol2, row))] = Pieces::Piece::GetBlankPiece();
    }
    else if (lastMove->isCapture()) {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        _pieces[endIndex] = lastCapturedPiece;
    }
    else if (lastMove->isEnPassant()) {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        Position locationOfCapturedPawn(endPos.col, (lastMove->color() == Color::WHITE ? endPos.row - 1 : endPos.row + 1));
        _pieces[Position::internalToIndex(locationOfCapturedPawn)] = lastCapturedPiece;
        _pieces[endIndex] = Pieces::Piece::GetBlankPiece();
    }
    else {
        _pieces[startIndex] = _pieces[endIndex];
        _pieces[startIndex]->setPosition(startPos.col, startPos.row);
        _pieces[endIndex] = Pieces::Piece::GetBlankPiece();
    }

    if (lastMove->enabledEnPassant()) {
        _hasEnPassantTarget = false;
        _enPassantTarget = Position(0, 0);
    }
    if (lastMove->ruinedEnPassant()) {
        _hasEnPassantTarget = true;
        _enPassantTarget = lastMove->ruinedEnPassantTarget();
    }

    if (lastMove->color() == Color::BLACK) {
        --_fullMoveNumber;
    }

    _whosTurnToGo = lastMove->color();
    _halfMoveClock = _prevHalfmoveClock; // TODO: This won't work. Need to add halfMoveClock to Move object like the other items that are undone.

    for (int i = 0; i < 2; ++i) {
        if (lastMove->ruinedCastling(i)) {
            setCastlingRights(lastMove->color(), intToCastleSide(i), true);
        }
    }

    _calculateFen();
    MAINLOG("FEN: " << _fen)

    if (_fen.compare(lastFen) != 0) {
        MAINLOG("ERROR ERROR ERROR !!!!! Fen does not match after undo")
        MAINLOG("Expected fen:")
        MAINLOG(lastFen)
        MAINLOG("Fen after undo:")
        MAINLOG(_fen)
        MAINLOG("Fen before undo:")
        MAINLOG(fenBeforeUndo)
        MAINLOG("Move was " << lastMove->algebraicFormat())
        throw std::runtime_error("ERROR: Fen does not match after undo");
    }

    _isCheck = _scanForCheck();
}

bool Board::_scanForCheck(bool otherPlayer) {
    // Find the king
    std::shared_ptr<Pieces::Piece> king;
    for (auto piece : _pieces) {
        if (piece->pieceType() == PieceType::KING && piece->color() == (otherPlayer ? oppositeColor(_whosTurnToGo) : _whosTurnToGo)) {
            king = piece;
            break;
        }
    }

    if (!king) {
        throw std::runtime_error("ERROR: Failed to find king");
    }
    return isSquareAttacked(king->position(), oppositeColor(king->color()));
}

bool Board::isSquareAttacked(const Position & square, Color attackerColor) const {
    for (auto piece : _pieces) {
        if (piece->pieceType() == PieceType::BLANK || piece->color() != attackerColor) {
            continue;
        }
        if (piece->isAttackingSquare(square)) {
            return true;
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

    MAINLOG("Handling move for legality test " << move->algebraicFormat())
    _handleMove(move);

    bool result = !_scanForCheck(true); // Check if the player that just moved is in check, which would make the move illegal.

    MAINLOG("Undoing legality test move " << move->algebraicFormat())
    _undoMove(move);

    return result;
}

void Board::_calculateLegalMoves() {
    _legalMoves.clear();
    for (const auto & piece : _pieces) {
        if (piece->pieceType() == PieceType::BLANK || piece->color() != _whosTurnToGo) {
            continue;
        }

        std::vector<std::shared_ptr<Move>> pieceMoves;
        piece->getPossibleMoves(pieceMoves);
        _legalMoves.insert(_legalMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }

    _hasLegalMove = (_legalMoves.size() > 0);

    MAINLOG_NNL("List of legal response moves (" << _legalMoves.size() << "):")
    for (const auto & move : _legalMoves) {
        MAINLOG_NNL(" " << move->algebraicFormat())
    }
    MAINLOG("")
}

void Board::_calculateHasLegalMove() {
    for (const auto & piece : _pieces) {
        if (piece->pieceType() == PieceType::BLANK || piece->color() != _whosTurnToGo) {
            continue;
        }

        if (piece->calculateHasLegalMove()) {
            _hasLegalMove = true;
            return;
        }

        _hasLegalMove = false;
    }
}

const std::vector<std::shared_ptr<Move>> & Board::getLegalMoves() const {
    return _legalMoves;
}

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
    if (castlingRights(Color::WHITE, CastleSide::KINGSIDE)) {
        sstr << 'K';
        someoneCanCastle = true;
    }
    if (castlingRights(Color::WHITE, CastleSide::QUEENSIDE)) {
        sstr << 'Q';
        someoneCanCastle = true;
    }
    if (castlingRights(Color::BLACK, CastleSide::KINGSIDE)) {
        sstr << 'k';
        someoneCanCastle = true;
    }
    if (castlingRights(Color::BLACK, CastleSide::QUEENSIDE)) {
        sstr << 'q';
        someoneCanCastle = true;
    }
    if (!someoneCanCastle) {
        sstr << '-';
    }
    sstr << ' ';
    if (_hasEnPassantTarget) {
        std::string pos;
        Position::internalToUci(_enPassantTarget, pos);
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
    return !_hasLegalMove;
}

bool Board::isStalemate() {
    if (_isCheck) {
        return false;
    }
    return !_hasLegalMove;
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

std::shared_ptr<Board> Board::clone() const {
    auto clone = std::make_shared<Board>();
    clone->addAllPieces(_pieces);
    clone->setWhosTurnToGo(_whosTurnToGo);
    clone->setHasEnPassantTarget(_hasEnPassantTarget);
    if (_hasEnPassantTarget) {
        clone->setEnPassantTarget(_enPassantTarget);
    }
    clone->setHalfMoveClock(_halfMoveClock);
    clone->setFullMoveNumber(_fullMoveNumber);
    clone->setIsCheck(_isCheck);
    clone->setFen(_fen);
    clone->setGameHistory(_gameHistory.clone());
    clone->addAllLegalMoves(_legalMoves);
    clone->setHasLegalMove(_hasLegalMove);
    clone->setCastlingRights(const_cast<bool*>(_castlingRights));
    clone->setCastled(const_cast<bool*>(_castled));
    clone->setPrevHalfmoveClock(_prevHalfmoveClock);

    return clone;
}

void Board::addAllPieces(std::vector<std::shared_ptr<Pieces::Piece>> pieces) {
    _pieces.clear();
    for (auto & piece : pieces) {
        if (piece->pieceType() == PieceType::BLANK) {
            _pieces.push_back(piece);
        }
        else {
            auto newPiece = piece->clone(shared_from_this());
            _pieces.push_back(newPiece);
        }
    }
}

}
}

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

void Board::handleMove(const std::shared_ptr<Move> & move) {
    MAINLOG("Handling move " << move->algebraicNotation())
    Position startPos = move->source();
    Position endPos = move->target();
    int startIndex = Position::internalToIndex(startPos);
    int endIndex = Position::internalToIndex(endPos);

    auto & movedPiece = _pieces[startIndex];

    _prevFen = _fen;

    _prevWhiteCastlingKing = _whitePlayer.hasCastlingRightsKingSide();
    _prevWhiteCastlingQueen = _whitePlayer.hasCastlingRightsQueenSide();
    _prevBlackCastlingKing = _blackPlayer.hasCastlingRightsKingSide();
    _prevBlackCastlingQueen = _blackPlayer.hasCastlingRightsQueenSide();

    _prevMoveIsCastle = move->isCastle();

    if (movedPiece->pieceType() == PieceType::KING && !movedPiece->hasMoved() || move->isCastle()) {
        if (movedPiece->color() == Color::WHITE) {
            _whitePlayer.setCastlingRightsKingSide(false);
            _whitePlayer.setCastlingRightsQueenSide(false);
        }
        else {
            _blackPlayer.setCastlingRightsKingSide(false);
            _blackPlayer.setCastlingRightsQueenSide(false);
        }
    }

    if (movedPiece->pieceType() == PieceType::ROOK && !movedPiece->hasMoved()) {
        if (movedPiece->color() == Color::WHITE) {
            if (movedPiece->position().col == 0) {
                _whitePlayer.setCastlingRightsQueenSide(false);
            }
            else {
                _whitePlayer.setCastlingRightsKingSide(false);
            }
        }
        else {
            if (movedPiece->position().col == 0) {
                _blackPlayer.setCastlingRightsQueenSide(false);
            }
            else {
                _blackPlayer.setCastlingRightsKingSide(false);
            }
        }
    }

    _prevHasEnPassantTarget = _hasEnPassantTarget;
    _prevEnPassantTarget = _enPassantTarget;

    _hasEnPassantTarget = false;
    bool pawnMove = false;
    
    if (movedPiece->pieceType() == PieceType::PAWN) {
        pawnMove = true;
        if (endPos.row - startPos.row == 2) {
            _hasEnPassantTarget = true;
            _enPassantTarget = Position(endPos.col, endPos.row - 1);
        }
        else if (startPos.row - endPos.row == 2) {
            _hasEnPassantTarget = true;
            _enPassantTarget = Position(endPos.col, endPos.row + 1);
        }
    }

    if (move->hasPromotion()) {
        _prevHasPromotion = true;
    }
    else {
        _prevHasPromotion = false;
    }

    bool capture = (_pieces[endIndex]->pieceType() != PieceType::BLANK);

    if (capture) {
        _prevHasCapturedPiece = true;
        Position::indexToInternal(endIndex, _prevCapturedPiecePosition);
        _prevCapturedPieceType = _pieces[endIndex]->pieceType();
        _prevCapturedPieceColor = _pieces[endIndex]->color();
        _prevCapturedPieceNumTimesMoved = _pieces[endIndex]->numTimesMoved();
    }
    else {
        _prevHasCapturedPiece = false;
    }

    _prevHalfmoveClock = _halfMoveClock;
    _prevFullMoveNumber = _fullMoveNumber;
    _prevWhosTurnToGo = _whosTurnToGo;
    _prevIsCheck = _isCheck;

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

    movedPiece->setHasMoved(true);
    movedPiece->incrementNumTimesMoved();
    movedPiece->setPosition(endPos.col, endPos.row);
    if (!move->hasPromotion()) {
        _pieces[endIndex] = movedPiece;
    }
    else {
        _pieces[endIndex] = Pieces::Piece::createPiece(move->promotionResult(), movedPiece->color(), endPos.col, endPos.row, shared_from_this());
        _pieces[endIndex]->setNumTimesMoved(movedPiece->numTimesMoved());
        _pieces[endIndex]->setHasMoved(true);
    }
    _pieces[startIndex] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());

    if (move->isCastle()) {
        Position rookStartPos(0, 0);
        Position rookEndPos(0, 0);
        if (startPos.row == 0 && endPos.col > startPos.col) {
            rookStartPos.col = 7;
            rookStartPos.row = 0;
            rookEndPos.col = 5;
            rookEndPos.row = 0;
        }
        else if (startPos.row == 0 && endPos.col > startPos.col) {
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
        _pieces[Position::internalToIndex(rookEndPos)] = _pieces[Position::internalToIndex(rookStartPos)];
        _pieces[Position::internalToIndex(rookStartPos)] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
        _pieces[Position::internalToIndex(rookEndPos)]->incrementNumTimesMoved();
    }

    _isCheck = scanForCheck();

    _calculateFen();
}

void Board::handleMoveAlgebraic(const std::string & moveAlgebraic) {
    auto move = std::make_shared<Move>(moveAlgebraic, _pieces[Position::algebraicToIndex(moveAlgebraic)]->pieceType());
    handleMove(move);
}

void Board::undoMove(const std::shared_ptr<Move> & move) {
    MAINLOG("Undoing move " << move->algebraicNotation())
    Position startPos = move->source();
    Position endPos = move->target();
    int startIndex = Position::internalToIndex(startPos);
    int endIndex = Position::internalToIndex(endPos);

    auto & movedPiece = _pieces[endIndex];
    movedPiece->setPosition(startPos.col, startPos.row);
    movedPiece->decrementNumTimesMoved();
    if (!_prevHasPromotion) {
        _pieces[startIndex] = movedPiece;
    } else {
        _pieces[startIndex] = Pieces::Piece::createPiece(PieceType::PAWN, movedPiece->color(), startPos.col, startPos.row, shared_from_this());
        _pieces[startIndex]->setNumTimesMoved(movedPiece->numTimesMoved());
        _pieces[startIndex]->setHasMoved(true);
    }
    if (movedPiece->numTimesMoved() == 0) {
        movedPiece->setHasMoved(false);
    }
    if (_prevHasCapturedPiece) {
        _pieces[endIndex] = Pieces::Piece::createPiece(_prevCapturedPieceType, _prevCapturedPieceColor, endPos.col, endPos.row, shared_from_this());
        _pieces[endIndex]->setNumTimesMoved(_prevCapturedPieceNumTimesMoved);
        _pieces[endIndex]->setHasMoved(_prevCapturedPieceNumTimesMoved > 0);
    } else {
        _pieces[endIndex] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, endPos.col, endPos.row, shared_from_this());
    }

    if (move->isCastle()) {
        Position rookStartPos(0, 0);
        Position rookEndPos(0, 0);
        if (startPos.row == 0 && endPos.col > startPos.col) {
            rookStartPos.col = 7;
            rookStartPos.row = 0;
            rookEndPos.col = 5;
            rookEndPos.row = 0;
        }
        else if (startPos.row == 0 && endPos.col > startPos.col) {
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
        _pieces[Position::internalToIndex(rookStartPos)] = _pieces[Position::internalToIndex(rookEndPos)];
        _pieces[Position::internalToIndex(rookEndPos)] = Pieces::Piece::createPiece(PieceType::BLANK, Color::WHITE, startPos.col, startPos.row, shared_from_this());
        _pieces[Position::internalToIndex(rookStartPos)]->decrementNumTimesMoved();
    }

    _fullMoveNumber = _prevFullMoveNumber;
    _halfMoveClock = _prevHalfmoveClock;
    _whosTurnToGo = _prevWhosTurnToGo;
    _isCheck = _prevIsCheck;
    _hasEnPassantTarget = _prevHasEnPassantTarget;
    _enPassantTarget = _prevEnPassantTarget;

    _whitePlayer.setCastlingRightsKingSide(_prevWhiteCastlingKing);
    _whitePlayer.setCastlingRightsQueenSide(_prevWhiteCastlingQueen);
    _blackPlayer.setCastlingRightsKingSide(_prevBlackCastlingKing);
    _blackPlayer.setCastlingRightsQueenSide(_prevBlackCastlingQueen);

    _calculateFen();
    if (_fen.compare(_prevFen) != 0) {
        MAINLOG("ERROR ERROR ERROR !!!!! Fen does not match after undo")
        MAINLOG("Prev fen:")
        MAINLOG(_prevFen)
        MAINLOG("Fen after undo:")
        MAINLOG(_fen)
        MAINLOG("Move was " << move->algebraicNotation())
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
    MAINLOG("Testing move for legality: " << move->algebraicNotation())
    handleMove(move);
//    MAINLOG("Player who is to move: " << (int)_whosTurnToGo)
    bool result = !scanForCheck(true);
    undoMove(move);
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

void Board::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const {
    for (const auto & piece : _pieces) {
        if (piece->color() != _whosTurnToGo) {
            continue;
        }

        std::vector<std::shared_ptr<Move>> pieceMoves;
        piece->getPossibleMoves(pieceMoves);
        possibleMoves.insert(possibleMoves.end(), pieceMoves.begin(), pieceMoves.end());
    }
}

void Board::_calculateFen() {
    std::stringstream sstr;
    for (int row = 7; row <= 0; --row) {
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
    std::vector<std::shared_ptr<Move>> moves;
    getPossibleMoves(moves);
    return (moves.size() == 0);
}
}
}

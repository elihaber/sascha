#include "Gameplay/Pieces/AllPieces.h"
#include "Gameplay/Board.h"
#include "Gameplay/Move.h"
#include "Gameplay/Position.h"
#include "Globals.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

using Sascha::Gameplay::Move;
using Sascha::Gameplay::Board;
using namespace Sascha::Gameplay::Pieces;

std::shared_ptr<Piece> Piece::GenericBlankPiece;

std::shared_ptr<Piece> Piece::createPiece(PieceType pieceType, Color color, int col, int row, std::shared_ptr<Board> board) {
    if (pieceType == PieceType::PAWN) {
        return std::make_shared<Pawn>(color, col, row, board);
    }
    if (pieceType == PieceType::ROOK) {
        return std::make_shared<Rook>(color, col, row, board);
    }
    if (pieceType == PieceType::BISHOP) {
        return std::make_shared<Bishop>(color, col, row, board);
    }
    if (pieceType == PieceType::KNIGHT) {
        return std::make_shared<Knight>(color, col, row, board);
    }
    if (pieceType == PieceType::QUEEN) {
        return std::make_shared<Queen>(color, col, row, board);
    }
    if (pieceType == PieceType::KING) {
        return std::make_shared<King>(color, col, row, board);
    }
    if (pieceType == PieceType::BLANK) {
        return std::make_shared<BlankPiece>(color, col, row, board);
    }
    throw std::runtime_error("Incorrect type of piece to create");
}

}
}
}

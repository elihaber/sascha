#include "Gameplay/Pieces/BlankPiece.h"
#include "Gameplay/Board.h"
#include "Gameplay/Position.h"
#include "Gameplay/Move.h"

namespace Sascha {
namespace Gameplay {
namespace Pieces {

void BlankPiece::getPossibleMoves(std::vector<std::shared_ptr<Move>> & possibleMoves) const { }

bool BlankPiece::calculateHasLegalMove() const { return false; }

bool BlankPiece::isAttackingSquare(const Position & square) const {
    return false;
}

}
}
}

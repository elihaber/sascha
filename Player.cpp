#include <functional>
#include "Player.h"

namespace Sascha {
namespace Gameplay {

using Pieces::Piece;

void Player::addPiece(Piece & piece) {
    _pieces.push_back(std::reference_wrapper<Piece>(piece));
}

}
}

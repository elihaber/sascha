#include <functional>
#include "Player.h"

void Player::addPiece(Piece & piece) {
    _pieces.push_back(std::reference_wrapper<Piece>(piece));
}
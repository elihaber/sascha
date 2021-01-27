#pragma once

#include <vector>
#include "Enums.h"
#include "Piece.h"

namespace Sascha {
namespace Gameplay {

using Pieces::Piece;

class Player {
public:
    void addPiece(Piece & piece);
    void setCastlingRightsKingSide(bool val) { _castlingRightsKingSide = val; }
    void setCastlingRightsQueenSide(bool val) { _castlingRightsQueenSide = val; }
    bool hasCastlingRightsKingSide() const { return _castlingRightsKingSide; }
    bool hasCastlingRightsQueenSide() const { return _castlingRightsQueenSide; }
private:
    Color _color;
    std::vector<std::reference_wrapper<Piece>> _pieces;
    bool _hasCastled;
    bool _castlingRightsKingSide;
    bool _castlingRightsQueenSide;
    bool _isComputer;
    bool _isOponent;
};

}
}

#pragma once

namespace Sascha {

enum class Color { WHITE, BLACK };

enum class PieceType { BLANK, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

enum class CastleSide { NONE, QUEENSIDE, KINGSIDE, BOTH };

static Color oppositeColor(Color color) { return (color == Color::WHITE) ? Color::BLACK : Color::WHITE; }

static std::string colorToString(Color color) { return (color == Color::WHITE) ? "WHITE" : "BLACK"; }
}

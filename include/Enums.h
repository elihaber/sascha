#pragma once

namespace Sascha {

enum class Color { WHITE, BLACK };

static int colorToInt(Color val) { return (int)val; }
static Color intToColor(int val) { if (val < 0 || val > 1) throw std::runtime_error(std::string("Invalid color value: ") + std::to_string(val)); return (Color)val; }
static std::string colorToString(Color color) { return (color == Color::WHITE) ? "WHITE" : "BLACK"; }
static Color oppositeColor(Color color) { return (color == Color::WHITE) ? Color::BLACK : Color::WHITE; }

enum class PieceType { BLANK, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

enum class CastleSide { QUEENSIDE, KINGSIDE };

static int castleSideToInt(CastleSide val) { return (int)val; }
static CastleSide intToCastleSide(int val) { if (val < 0 || val > 1) throw std::runtime_error(std::string("Invalid castle side value: ") + std::to_string(val)); return (CastleSide)val; }

enum class Algorithm { NONE, RANDOM, MINIMAX };

}

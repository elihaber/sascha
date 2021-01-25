#pragma once

enum class Color { WHITE, BLACK };

enum class PieceType { BLANK, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

static Color oppositeColor(Color color) { return (color == Color::WHITE) ? Color::BLACK : Color::WHITE; }
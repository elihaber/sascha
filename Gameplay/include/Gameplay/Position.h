#pragma once

#include <string>
#include <vector>
#include "Enums.h"
#include "Globals.h"

namespace Sascha {
namespace Gameplay {

class Position {
public:
    int col;
    int row;

    Position() : col(-1), row(-1) { }
    Position(int aCol, int aRow) : col(aCol), row(aRow) { }

    static void uciToInternal(const std::string & uci, Position & pos) {
        pos.col = (char)uci[0] - 'a';
        pos.row = (char)uci[1] - '1';
    }

    static void internalToUci(const Position & pos, std::string & uci) {
        uci = ('a' + (char)pos.col);
        uci += ('1' + (char)pos.row);
    }

    static int internalToIndex(const Position & pos) {
        return pos.row * 8 + pos.col;
    }

    static void indexToInternal(int index, Position & pos) {
        pos.col = index % 8;
        pos.row = index / 8;
    }

    static int uciToIndex(const std::string & uci) {
        Position pos;
        uciToInternal(uci, pos);
        return internalToIndex(pos);
    }

    static void indexToUci(int index, std::string & uci) {
        Position pos;
        indexToInternal(index, pos);
        internalToUci(pos, uci);
    }

    static bool areSquaresTouching(const Position & pos1, const Position & pos2) {
        return (abs(pos1.col - pos2.col) <= 1 &&  abs(pos1.row - pos2.row) <= 1 &&
                !(pos1.col == pos2.col && pos1.row == pos2.row));
    }

    static bool areSquaresOnSameColumn(const Position & pos1, const Position & pos2) {
        return (pos1.col == pos2.col);
    }

    static bool areSquaresOnSameRow(const Position & pos1, const Position & pos2) {
        return (pos1.row == pos2.row);
    }

    static bool areSquaresOnSameDiagonalUp(const Position & pos1, const Position & pos2) {
        return (pos1.col - pos2.col == pos1.row - pos2.row);
    }

    static bool areSquaresOnSameDiagonalDown(const Position & pos1, const Position & pos2) {
        return ((pos1.col - pos2.col) == -(pos1.row - pos2.row));
    }

    static bool areSquaresKnightsMoveFromEachOther(const Position & pos1, const Position & pos2) {
        return ((abs(pos1.col - pos2.col) == 2 &&  abs(pos1.row - pos2.row) == 1) ||
                (abs(pos1.col - pos2.col) == 1 &&  abs(pos1.row - pos2.row) == 2));
    }

    static std::vector<Position> getSquaresWherePawnCanAttack(const Position & pos, Color attackerColor) {
        std::vector<Position> positions;

        if (attackerColor == Color::BLACK) {
            int row = pos.row + 1;
            if (row <= 7) {
                int col = pos.col - 1;
                if (col >= 0) {
                    positions.push_back(Position(col, row));
                }
                col = pos.col + 1;
                if (col <= 7) {
                    positions.push_back(Position(col, row));
                }
            }
        }
        else {
            int row = pos.row - 1;
            if (row >= 0) {
                int col = pos.col - 1;
                if (col >= 0) {
                    positions.push_back(Position(col, row));
                }
                col = pos.col + 1;
                if (col <= 7) {
                    positions.push_back(Position(col, row));
                }
            }
        }

        return positions;
    }

    bool operator ==(const Position & other) const {
        return (col == other.col && row == other.row);
    }
};

}
}

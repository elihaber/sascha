#include "Engine/Evaluators/RandomEvaluator.h"
#include "Gameplay/Board.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

void RandomEvaluator::calculateBestMove() {
    _isDone = false;
    auto moves = _board->getLegalMoves();
    auto index = rand() % moves.size();
    _calculatedMove = moves[index];
    _isDone = true;
}

}
}
}

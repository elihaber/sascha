#include "Engine/Evaluators/RandomEvaluator.h"
#include "Gameplay/Board.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

std::shared_ptr<Sascha::Gameplay::Move> RandomEvaluator::getBestMove() {
    auto moves = _board->getLegalMoves();
    auto index = rand() % moves.size();
    return moves[index];
}

}
}
}

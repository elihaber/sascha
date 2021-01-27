#include "Engine/Evaluators/RandomEvaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

int RandomEvaluator::getBestMoveIndex(std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & moves) {
    return rand() % moves.size();
}

}
}
}

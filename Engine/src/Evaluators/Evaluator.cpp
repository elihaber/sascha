#include "Engine/Evaluators/Evaluator.h"
#include "Engine/Evaluators/RandomEvaluator.h"
#include "Engine/Evaluators/TotalPieceValueEvaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

std::shared_ptr<Evaluator> Evaluator::createEvaluator() {
    return std::make_shared<TotalPieceValueEvaluator>();
}

}
}
}

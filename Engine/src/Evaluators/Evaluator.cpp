#include "Engine/Evaluators/Evaluator.h"
#include "Engine/Evaluators/RandomEvaluator.h"
#include "Engine/Evaluators/MinimaxSingleThreadEvaluator.h"
#include "Engine/Evaluators/MinimaxMultiThreadEvaluator.h"
#include "Globals.h"
#include "Engine/Options.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

std::shared_ptr<Evaluator> Evaluator::createEvaluator(std::shared_ptr<Engine::Options> options) {
    if (options->getAlgorithm() == Algorithm::MINIMAX) {
        return std::make_shared<MinimaxMultiThreadEvaluator>(options);
    }
    else if (options->getAlgorithm() == Algorithm::RANDOM) {
        return std::make_shared<RandomEvaluator>(options);
    }
    else {
        MAINLOG("ERROR: Algorithm is not set to a valid value")
        throw std::runtime_error("ERROR: Algorithm is not set to a valid value");
    }
}

}
}
}

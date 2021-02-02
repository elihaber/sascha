#pragma once

#include <memory>
#include <vector>

#include "Evaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
    class Options;
namespace Evaluators {

class RandomEvaluator : public Evaluator {
public:
    RandomEvaluator(std::shared_ptr<Sascha::Engine::Options> options) : Evaluator(options) { }

    virtual void calculateBestMove() override;

};
    
}
}
}

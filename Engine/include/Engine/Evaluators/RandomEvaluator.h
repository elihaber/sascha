#pragma once

#include <memory>
#include <vector>

#include "Evaluator.h"

namespace Sascha {
namespace Gameplay { class Board; class Move; }
namespace Engine {
namespace Evaluators {

class RandomEvaluator : public Evaluator {
public:
    RandomEvaluator() : Evaluator() { }

    virtual int getBestMoveIndex(std::vector<std::shared_ptr<Sascha::Gameplay::Move>> & moves) override;

};
    
}
}
}
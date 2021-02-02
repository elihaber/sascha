#pragma once

#include "Enums.h"

namespace Sascha {
namespace Engine {

class Options : public std::enable_shared_from_this<Options>  {
public:
    Options() : _hashSize(1), _ownBook(false), _algorithm(Algorithm::MINIMAX), _searchDepth(4), _initialMoveCount(4) { }

    void setHashSize(int val) { _hashSize = val; }
    int getHashSize() const { return _hashSize; }
    void setOwnBook(bool val) { _ownBook = val; }
    bool getOwnBook() const { return _ownBook; }
    void setAlgorithm(Algorithm val) { _algorithm = val; }
    Algorithm getAlgorithm() const { return _algorithm; }
    void setSearchDepth(int val) { _searchDepth = val; }
    int getSearchDepth() const { return _searchDepth; }
    void setInitialMoveCount(int val) { _initialMoveCount = val; }
    int getInitialMoveCount() const { return _initialMoveCount; }

private:
    int _hashSize;
    bool _ownBook;
    Algorithm _algorithm;
    int _searchDepth;;
    int _initialMoveCount;
};

}
}

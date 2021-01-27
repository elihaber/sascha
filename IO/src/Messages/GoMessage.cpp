#include <iterator>
#include <sstream>
#include <algorithm>
#include "IO/Messages/GoMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool GoMessage::parseInputLine(const std::string & line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };

    //Get keywords and positions.
    std::vector<std::string> keywords = { "searchmoves", "ponder", "wtime", "btime", "winc", "binc", "movestogo", "depth", "nodes", "mate", "movetime", "infinite" };
    std::vector<std::string> keytokens;
    std::vector<int> keypositions;
    for (int i = 0; i < tokens.size(); ++i) {
        if (std::find(keywords.begin(), keywords.end(), tokens[i]) != keywords.end()) {
            keytokens.push_back(tokens[i]);
            keypositions.push_back(i);
        }
    }

    for (int i = 0; i < keytokens.size(); ++i) {
        std::string key = keytokens[i];
        int keypos = keypositions[i];
        int nextkeypos = tokens.size();
        if (i < keytokens.size() - 1) {
            nextkeypos = keypositions[i + 1];
        }

        if (key.compare("searchmoves") == 0) {
            setSearchmovesFlag(true);
            for (int j = keypos + 1; j < nextkeypos; ++j) {
                addMove(tokens[j]);
            }
        }

        if (key.compare("ponder") == 0) {
            setPonderFlag(true);
        }

        if (key.compare("wtime") == 0) {
            setWtimeFlag(true);
            try {
                setWtime(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                return false;
            }
        }

        if (key.compare("btime") == 0) {
            setBtimeFlag(true);
            try {
                setBtime(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                MAINLOG("Exception in btime while converting " << tokens[keypos + i])
                return false;
            }
        }

        if (key.compare("winc") == 0) {
            setWincFlag(true);
            try {
                setWinc(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                MAINLOG("Exception in winc while converting " << tokens[keypos + i])
                return false;
            }
        }

        if (key.compare("binc") == 0) {
            setBincFlag(true);
            try {
                setBinc(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                MAINLOG("Exception in binc while converting " << tokens[keypos + i])
                return false;
            }
        }

        if (key.compare("movestogo") == 0) {
            setMovestogoFlag(true);
            try {
                setMovestogo(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                return false;
            }
        }

        if (key.compare("depth") == 0) {
            setDepthFlag(true);
            try {
                setDepth(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                return false;
            }
        }

        if (key.compare("nodes") == 0) {
            setNodesFlag(true);
            try {
                setNodes(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                return false;
            }
        }

        if (key.compare("mate") == 0) {
            setMateFlag(true);
            try {
                setMate(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                return false;
            }
        }

        if (key.compare("movetime") == 0) {
            setMovetimeFlag(true);
            try {
                setMovetime(std::stoi(tokens[keypos + 1]));
            }
            catch (std::exception &) {
                return false;
            }
        }

        if (key.compare("infinite") == 0) {
            setInfiniteFlag(true);
        }
    }

    return true;
}

}
}
}

#include <sstream>
#include "IO/Messages/OptionMessage.h"
#include "Globals.h"

namespace Sascha {
namespace IO {
namespace Messages {

bool OptionMessage::formatMessage(std::stringstream & line) {
    line << "option";

    if (name().empty()) {
        MAINLOG("OPTION message with no name")
        return false;
    }
    line << " name " << name();

    if (optionType().empty()) {
        MAINLOG("OPTION message with no optioneType")
        return false;
    }
    line << " type " << optionType();

    if (defaultFlag()) {
        if (defaultVal().empty()) {
            MAINLOG("OPTION message with default flag but no default")
            return false;
        }
        line << " default " << defaultVal();
    }

    if (minFlag()) {
        line << " min " << min();
    }

    if (maxFlag()) {
        line << " max " << max();
    }

    if (varFlag()) {
        if (vars().empty()) {
            MAINLOG("OPTION message with var flag but no vars")
            return false;
        }
        line << " var";
        for (int i = 0; i < vars().size(); ++i) {
            line << " " << vars()[i];
        }
    }

    return true;
}

}
}
}

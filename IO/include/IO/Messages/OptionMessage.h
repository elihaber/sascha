#include "IO/Messages/Message.h"

namespace Sascha {
namespace IO {
namespace Messages {

class OptionMessage : public Message {
public:
    OptionMessage() : Message(MessageType::OPTION), _defaultFlag(false), _minFlag(false), _maxFlag(false), _varFlag(false), _nameFlag(false) { }
    virtual bool parseInputLine(const std::string & line) override { return false; }
    virtual bool formatMessage(std::stringstream & line) override;
    bool defaultFlag() const { return _defaultFlag; }
    bool minFlag() const { return _minFlag; }
    bool maxFlag() const { return _maxFlag; }
    bool varFlag() const { return _varFlag; }
    bool nameFlag() const { return _nameFlag; }
    std::string optionType() const { return _optionType; }
    std::string defaultVal() const { return _defaultVal; }
    std::string name() const { return _name; }
    int min() const { return _min; }
    int max() const { return _max; }
    std::vector<std::string> vars() const { return _vars; }
    void setDefaultFlag(bool val) { _defaultFlag = val; }
    void setMinFlag(bool val) { _minFlag = val; }
    void setMaxFlag(bool val) { _maxFlag = val; }
    void setVarFlag(bool val) { _varFlag = val; }
    void setNameFlag(bool val) { _nameFlag = val; }
    void setOptionType(const std::string & val) { _optionType = val; }
    void setDefaultVal(const std::string & val) { _defaultVal = val; }
    void setName(const std::string & val) { _name = val; }
    void setMin(int val) { _min = val; }
    void setMax(int val) { _max = val; }
    void addVar(const std::string & val) { _vars.push_back(val); }

private:
    bool _defaultFlag;
    bool _minFlag;
    bool _maxFlag;
    bool _varFlag;
    bool _nameFlag;
    std::string _optionType;
    std::string _defaultVal;
    std::string _name;
    int _min;
    int _max;
    std::vector<std::string> _vars;
};

}
}
}

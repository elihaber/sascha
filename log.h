#include <string>
#include <fstream>

namespace log
{
    extern const std::string path ;
    extern std::ofstream out ;
    void flush() ;
}

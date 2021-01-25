#include "log.h"
#include <ctime>

namespace log
{
    void flush() { out.flush() ; }

    namespace // detail
    {
        std::string time_stamp()
        {
            const auto now = std::time(nullptr) ;
            char cstr[256] {};
            return std::strftime( cstr, sizeof(cstr), "%Y%m%d_%H%M%S", std::localtime(&now) ) ? cstr : "" ;
        }

        std::string path_to_session_log_file()
        {
            static const std::string log_dir = "C:/Users/eliha/Documents/dev/sascha/" ;
            static const std::string log_file_name = "sascha_log.txt" ;
            return log_dir + time_stamp() + '_' + log_file_name ;
        }
    }

   const std::string path = path_to_session_log_file() ;
   std::ofstream out = std::ofstream( path );
}

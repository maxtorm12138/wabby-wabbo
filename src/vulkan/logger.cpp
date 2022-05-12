#include "logger.hpp"

namespace wabby::vulkan
{

wabby::util::logger g_logger = [](){
    wabby::util::logger lg;
    lg.add_console_sink();
    return lg;
}();

}
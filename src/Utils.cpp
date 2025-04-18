#include "Utils.hpp"
#include "Steam.hpp"
#include <stdarg.h>

void sff_debug_printf(const char* fmt, ...)
{
    #ifdef SFF_DEBUG
    va_list arg_list;
    va_start(arg_list, fmt);
    vprintf(fmt, arg_list);
    va_end(arg_list);
    #endif
}
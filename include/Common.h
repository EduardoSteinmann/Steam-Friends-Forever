#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef __cplusplus
extern "C" 
{
#endif

#define INITIAL_WINDOW_WIDTH 1280
#define INITIAL_WINDOW_HEIGHT 800
#define MAX_QUERY 20
#define SFF_DEBUG

void sff_debug_printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
#ifdef __cplusplus
extern "C" 
{
#endif

#define CLAY_IMPLEMENTATION
#define SDL_MAIN_HANDLED
#include "clay.h"
#include "clay_renderer_raylib.c"
#include <stdio.h>

int app_driver()
{
    printf("Hello World");
    return 0;
}

#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
extern "C" 
{
#endif

#define CLAY_IMPLEMENTATION
#define SDL_MAIN_HANDLED
#include "clay.h"
#include "clay_renderer_raylib.c"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "AppDriver.h"
#include "Common.h"

void sff_clay_error_handler(Clay_ErrorData error_text)
{
    sff_debug_printf("CLAY REPORTED AN ERROR: %s", error_text.errorText);
}

int app_driver()
{
    Clay_Raylib_Initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Steam-Friends-Forever", FLAG_WINDOW_RESIZABLE);

    size_t clay_required_memory_amount = Clay_MinMemorySize();

    char* clay_backing_buffer = (char*)malloc(clay_required_memory_amount);

    if (clay_backing_buffer == NULL)
    {
        sff_debug_print("ERROR WHEN ALLOCATING MEMORY REQUIRED FOR CLAY: %s", strerror(errno));
        return errno;
    }

    Clay_Arena clay_arena = (Clay_Arena){ .memory = clay_backing_buffer, .capacity = clay_required_memory_amount };

    Clay_Initialize
    (
        clay_arena, 
        (Clay_Dimensions)
        { 
            .width = GetScreenWidth(), .height = GetScreenHeight() 
        },
        (Clay_ErrorHandler)
        {
            .errorHandlerFunction = sff_clay_error_handler,
            .userData = NULL
        }
    );

    while (!WindowShouldClose())
    {
        Clay_BeginLayout();

        Clay_RenderCommandArray clay_render_commands = Clay_EndLayout();

        BeginDrawing();
        Clay_Raylib_Render(clay_render_commands, NULL);
        EndDrawing();
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
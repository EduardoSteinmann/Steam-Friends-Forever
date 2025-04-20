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

typedef enum UIState
{
    START_SCREEN,
    LOADING,
    DISPLAY_USER
} UIState;

static void start_screen()
{
    CLAY({ .id = CLAY_ID("OuterContainer"),
        .backgroundColor = {27, 40, 56, 255},
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = { 
                .width = GetScreenWidth(), 
                .height = GetScreenHeight()
            },
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16
        }
    })
    {
        
    };
}

static void display_user()
{

}

static void loading_screen()
{

}

void search_bar(bool active)
{
    CLAY( 
        { 
            .id = CLAY_ID("SearchBar"),
            .backgroundColor = { 50, 50, 50, 255 },
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = { 
                    //width might be wrong
                    .width = CLAY_SIZING_PERCENT(50), 
                    .height = GetScreenHeight() / 2 
                },
                .padding = CLAY_PADDING_ALL(8),
                .childGap = 8
            } 
        }
    ){};

    if (!active)
    {
        return;
    }
    
    static char searchQuery[MAX_QUERY] = {0};

    int key = GetCharPressed();

    while (key > 0) {
        int len = strlen(searchQuery);
        if (len < MAX_QUERY - 1) {
            searchQuery[len] = (char)key;
            searchQuery[len + 1] = '\0';
            
        }  
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = strlen(searchQuery);
        if (len > 0) {
            searchQuery[len - 1] = '\0';
        }
    } 
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
        const char* clip = GetClipboardText();
        if (clip && strlen(clip) + strlen(searchQuery) < MAX_QUERY) {
            strcat(searchQuery, clip);
        }
    }
}

int app_driver()
{
    UIState interface_state = START_SCREEN;

    Clay_Raylib_Initialize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Steam-Friends-Forever", FLAG_WINDOW_RESIZABLE);

    size_t clay_required_memory_amount = Clay_MinMemorySize();

    char* clay_backing_buffer = (char*)malloc(clay_required_memory_amount);

    if (clay_backing_buffer == NULL)
    {
        sff_debug_printf("ERROR WHEN ALLOCATING MEMORY REQUIRED FOR CLAY: %s", strerror(errno));
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
        
        Clay_SetLayoutDimensions((Clay_Dimensions)
        { 
            .width = GetScreenWidth(), 
            .height = GetScreenHeight() 
        });
        
        Vector2 mouse_position = GetMousePosition();

        if (interface_state == START_SCREEN)
        {
            //Call ui for start
            start_screen();
        }
        else if (interface_state == DISPLAY_USER)
        {
            //Call ui for displaying
            display_user();
        }
        else if (interface_state == LOADING)
        {
            //Show loading screen
            loading_screen();
        }

        Clay_RenderCommandArray clay_render_commands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(clay_render_commands, NULL);
        EndDrawing();
    }

    Clay_Raylib_Close();

    return 0;
}

#ifdef __cplusplus
}
#endif
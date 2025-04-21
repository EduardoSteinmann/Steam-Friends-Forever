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


void search_bar(bool active)
{
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

static void start_screen(Clay_ImageElementConfig image)
{
    CLAY({ .id = CLAY_ID("OuterContainer"),
        .backgroundColor = {27, 40, 56, 255},
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = { 
                CLAY_SIZING_GROW(0),
                CLAY_SIZING_GROW(0)
            },
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16,
            .childAlignment = {.y = CLAY_ALIGN_Y_CENTER, .x = CLAY_ALIGN_X_CENTER}
        }
    })
    {
        CLAY({.id = CLAY_ID("Logo"),
            .layout = {
                .sizing = {.width = 128, .height = 128},
                .padding = CLAY_PADDING_ALL(16),

            },
            .image = {.imageData = &image , .sourceDimensions = { 128,128}}
        }){}
        CLAY({.id = CLAY_ID("SearchBar"),
            .backgroundColor = {199, 213, 224, 255},
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {
                    .height = 32,
                    .width = GetScreenWidth()/2,
                },

            },
            .cornerRadius = 16,

        }
        ){}
    };
    //auto *image_to_render =
}

static void display_user()
{
    //auto font = GetFontDefault();
    CLAY({ .id = CLAY_ID("OuterContainer"),
        .backgroundColor = {27, 40, 56, 255},
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
            .sizing = {
                CLAY_SIZING_GROW(0),
                CLAY_SIZING_GROW(0)
            },
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16,
            .childAlignment = {.y = CLAY_ALIGN_Y_TOP, .x = CLAY_ALIGN_X_CENTER}
        }
    })
    {
        CLAY({.id = CLAY_ID("BackButton"),
            .backgroundColor = {199, 213, 224, 255},
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {
                    .height = 32,
                    .width = 32,
                },
                .padding = CLAY_PADDING_ALL(4),
                 .childGap = 16,
                .childAlignment = {.y = CLAY_ALIGN_Y_TOP, .x = CLAY_ALIGN_X_CENTER}
            },
            .cornerRadius = 8,

        })
        {
            CLAY_TEXT(CLAY_STRING("Back") , CLAY_TEXT_CONFIG({.fontId = FONT_DEFAULT, .fontSize = 16, .textColor = {0,0,0,255}}));
        }
        CLAY({.id = CLAY_ID("SearchBar"),
            .backgroundColor = {199, 213, 224, 255},
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {
                    .height = 32,
                    .width = GetScreenWidth()/2,
                },

            },
            .cornerRadius = 16,

        }
        ){}


    }
}

static void loading_screen()
{

}



int app_driver()
{
    UIState interface_state = DISPLAY_USER;
    //load logo
    Image Logo = LoadImage("C:/Users/Kasra/CLionProjects/Steam-Friends-Forever/assets/Steam-Friends-Forever-Logo.png");
    Clay_ImageElementConfig image_config = (Clay_ImageElementConfig){
        .imageData = &Logo, .sourceDimensions = {1024, 1024}
    };

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
        //get mouse position (needed for node click and searchbar click)
        //Clay_SetPointerState((Clay_Vector2) {GetMouseX() , GetMouseY()} , );

        if (interface_state == START_SCREEN)
        {
            //Call ui for start
            start_screen(image_config);
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
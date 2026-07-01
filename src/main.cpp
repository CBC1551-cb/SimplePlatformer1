#include "platformer_lib.h"

#include "input.h"

#include "game.h"

#include "platform.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#ifdef _WIN32
#include "win32_platform.cpp"

#endif

#include "gl_renderer.cpp"

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Game DLL stuff
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
// functions pointer to update_game in game.cpp
typedef decltype(update_game) update_game_type;
static update_game_type* update_game_ptr;
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Game Cross PLatform functions
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
void reload_game_dll(BumpAllocator* transientStorage);

int main(){
    BumpAllocator transientStorage = make_bump_allocator(MB(50));
    BumpAllocator persistentStorage = make_bump_allocator(MB(50));

    gameStates = (GameStates*)bump_allocate(&persistentStorage, sizeof(GameStates));
    if(!gameStates){
        SP_ERR("Failed to allocate GameStates");
        return -1;
    }

    input = (Input*)bump_allocate(&persistentStorage, sizeof(Input));
    if(!input){
        SP_ERR("Failed to allocate Input");
        return -1;
    }

    renderData = (RenderData*)bump_allocate(&persistentStorage, sizeof(RenderData));
    if(!renderData){
        SP_ERR("Failed to allocate RenderData");
        return -1;
    }

    #ifdef _WIN32
        EnableVirtualTerminalProcessing();
    #endif

    platform_create_window(1200, 720, L"Simple Platformer");
    input->screenSizeX = 1200;
    input->screenSizeY = 720;

    gl_init(&transientStorage);

    reload_game_dll(&transientStorage);

    running = true;
    while (running)
    {
        // updating game
        platform_update_window();
        update_game(gameStates, renderData, input);
        gl_render();

        platform_swap_buffers();
        reload_game_dll(&transientStorage);

        transientStorage.used = 0;
    }
    return 0;
}

void update_game(GameStates* gameStatesIn, RenderData* renderDataIn, Input* inputIn){
    update_game_ptr(gameStatesIn, renderDataIn, inputIn);
}

void reload_game_dll(BumpAllocator* transientStorage){
    static void* gameDLL;
    static long long lastEditTimestampGameDLL;

    long long currentTimestampGameDLL = get_timestamp("game.dll");
    if(currentTimestampGameDLL > lastEditTimestampGameDLL){
        if(gameDLL){
            bool freeResult = platform_free_dynamic_library(gameDLL);
            SP_ASSERT(freeResult, "Failed to free game.dll");
            gameDLL = nullptr;
            SP_TRACE("Freed game.dll");
        }

        while(!copy_file("game.dll", "game_load.dll", transientStorage)){
            Sleep(10);
        }
        SP_TRACE("Copied game.dll into game_load.dll");

        gameDLL = platform_load_dynamic_library("game_load.dll");
        SP_ASSERT(gameDLL, "failed to load game_load.dll")

        update_game_ptr = (update_game_type*)platform_load_dynamic_function(gameDLL, "update_game");
        SP_ASSERT(update_game_ptr, "Failed to load update_game function");
        lastEditTimestampGameDLL = currentTimestampGameDLL;
    }
}
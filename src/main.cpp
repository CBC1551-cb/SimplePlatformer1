#include "platformer_lib.h"

#include "input.h"

#include "game.cpp"

#include "platform.h"

#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#ifdef _WIN32
#include "win32_platform.cpp"

#endif

#include "gl_renderer.cpp"

int main(){
    BumpAllocator transientStorage = make_bump_allocator(MB(50));

    #ifdef _WIN32
        EnableVirtualTerminalProcessing();
    #endif

    platform_create_window(1200, 720, L"Simple Platformer");
    input.screenSizeX = 1200;
    input.screenSizeY = 720;

    gl_init(&transientStorage);
    running = true;
    while (running)
    {
        // updating game
        platform_update_window();
        update_game();
        gl_render();

        platform_swap_buffers();
    }
    return 0;
}
#include "platformer_lib.h"
#include "platform.h"

#define APIENTRY
#include "glcorearb.h"
#include "gl_renderer.h"

#ifdef _WIN32
#include "win32_platform.cpp"

#endif

int main(){
    #ifdef _WIN32
        EnableVirtualTerminalProcessing();
    #endif
    platform_create_window(1200, 720, L"Simple Platformer");
    while (running == true)
    {
        // updating game
        platform_update_window();
    }
    return 0;
}
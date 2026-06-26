#pragma once
#include "platform.h"
#include "platformer_lib.h"
#include "gl_renderer.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "wglext.h"
    bool EnableVirtualTerminalProcessing(){
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE){
                return false;
            }
            DWORD dwMode = 0;
            if (!GetConsoleMode(hOut, &dwMode)){
                return false;
            }
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if(!SetConsoleMode(hOut, dwMode)){
                return false;
            }
            return true;
    }

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Windows Globals
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

static HWND window;
static HDC dc;

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//                                      Windows Platform Implementations
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
LRESULT CALLBACK windows_window_callback(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
    LRESULT result = 0;
    switch(msg){
        case WM_DESTROY:{
            PostQuitMessage(0);
            running = false;
            return 0;
        }
        case WM_QUIT:{
            PostQuitMessage(0);
            running = false;
            return 0;
        }
        case WM_SIZE:{
            RECT rect = {};
            GetClientRect(window, &rect);
            input.screenSizeX = rect.right - rect.left;
            input.screenSizeY = rect.bottom - rect.top;
            break;
        }
        default:{
            result = DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    return result;
}
bool platform_create_window(int width, int height, const WCHAR* title){
    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASS wc = {};
    wc.hInstance = instance;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);               // Calls the default cursor
    wc.lpszClassName = (const char*)"SimplePlatformer";                 // Creates a unique ID for the window, THIS IS NOT THE TITLE
    wc.lpfnWndProc = windows_window_callback;                         // Input into the window

    if(!RegisterClass(&wc)){
        DWORD err = GetLastError();
        return false;
    }

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
    {
        // fake window creation to set pixel format
        window = CreateWindowExW(0, L"SimplePlatformer", // This references lpszClassName from wc
                                         title, //This is the actual window's name
                                         dwStyle,
                                         100,
                                         100,
                                         width,
                                         height,
                                         NULL,  // Parent
                                         NULL,  // Menu
                                         instance,
                                         NULL); //lpParam
        if(window == NULL){
            SP_ASSERT(false, "Failed to create window");
            return false;
        }

        HDC fakeDC = GetDC(window);
        if(!fakeDC){
            SP_ASSERT(false, "Failed to get HDC");
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.cDepthBits = 24;

        int pixelFormat  = ChoosePixelFormat(fakeDC, &pfd);
        if(!pixelFormat){
            SP_ASSERT(false, "Failed to choose pixel format");
            return false;
        }

        if(!SetPixelFormat(fakeDC, pixelFormat, &pfd)){
            SP_ASSERT(false, "Failed to set pixel format");
            return false;
        }

        HGLRC fakeRC = wglCreateContext(fakeDC);
        if(!fakeRC){
            SP_ASSERT(false, "Failed to create render context");
            return false;
        }

        if(!wglMakeCurrent(fakeDC, fakeRC)){
            SP_ASSERT(false, "Failed to make current");
            return false;
        }

        wglChoosePixelFormatARB = 
            (PFNWGLCHOOSEPIXELFORMATARBPROC)platform_load_gl_function("wglChoosePixelFormatARB");
        wglCreateContextAttribsARB = 
            (PFNWGLCREATECONTEXTATTRIBSARBPROC)platform_load_gl_function("wglCreateContextAttribsARB");

        if(!wglCreateContextAttribsARB || !wglChoosePixelFormatARB){
            SP_ASSERT(false, "Failed to load OpenGL functions");
            return false;
        }

        // Clean up the make stuff
        wglMakeCurrent(fakeDC, 0);
        wglDeleteContext(fakeRC);
        ReleaseDC(window, fakeDC);

        // Can't reuse the same (Device)Context,
        // because we already called "SetPixelFormat"

        DestroyWindow(window);

    }

    // Actual OpenGL initialization
    {
        // Add in the border size of the window
        {
            RECT borderRect = {};
            AdjustWindowRectEx(&borderRect, dwStyle, 0, 0);

            width += borderRect.right - borderRect.left;
            height += borderRect.bottom - borderRect.top;
        }

        window = CreateWindowExW(0, L"SimplePlatformer", // This references lpszClassName from wc
                                         title, //This is the actual window's name
                                         dwStyle,
                                         100,
                                         100,
                                         width,
                                         height,
                                         NULL,  // Parent
                                         NULL,  // Menu
                                         instance,
                                         NULL); //lpParam
        if(window == NULL){
            SP_ASSERT(false, "Failed to create window");
            return false;
        }

        dc = GetDC(window);
        if(!dc){
            SP_ASSERT(false, "Failed to get HDC");
            return false;
        }

        const int pixelAttribs[] ={
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SWAP_METHOD_ARB,    WGL_SWAP_COPY_ARB,
            WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     32,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            0 // Terminate this area with 0, otherwise OpenGL will throw an Error
        };
        
        UINT numPixelFormats;
        int pixelFormat = 0;
        if(!wglChoosePixelFormatARB(dc, pixelAttribs, 0, 1, &pixelFormat, &numPixelFormats)){
            SP_ASSERT(0, "Failed to wglChoosePixelFormatARB");
            return false;
        }

        PIXELFORMATDESCRIPTOR pfd = {0};
        DescribePixelFormat(dc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        if(!SetPixelFormat(dc, pixelFormat, &pfd)){
            SP_ASSERT(false, "Failed to SetPixelFormat");
            return true;
        }

        const int contextAttribs[] = 
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 3,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
            0 // Terminate the array
        };

        HGLRC rc = wglCreateContextAttribsARB(dc, 0, contextAttribs);
        if(!rc){
            SP_ASSERT(false, "Failed to create Render Context for OpenGL");
            return false;
        }

        if(!wglMakeCurrent(dc, rc)){
            SP_ASSERT(false, "Failed to wglMakeCurrent");
            return false;
        }
    }

    ShowWindow(window, SW_SHOW);

    return true;
}
void platform_update_window(){
    MSG msg;

    while(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)){
        TranslateMessage(&msg);     
        DispatchMessage(&msg);      // Calls the callback specified when creating the window
    }
}

void* platform_load_gl_function(char* function){
    PROC proc = wglGetProcAddress(function);

    if(!proc){
        static HMODULE openglDLL = LoadLibraryW(L"opengl32.dll");
        proc = GetProcAddress(openglDLL, function);

        if(!proc){
            SP_ASSERT(false, "Failed to load gl function %s", function)
        }
    }

    return(void*)proc;
}

void platform_swap_buffers(){
    SwapBuffers(dc);
}
#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>

// NOTE: This is acting like a conditional variable but it is very inefficient
static volatile i32 childDone = false;

_THREAD_PROC(child)
{
    // Sleep for 1 second. to simulate expensive work.
    Sleep(1000);

    Logger::LogInfo("child thread end!\n");
    childDone = true;

    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("main function start!\n");

    thread t{};
    t.start(child, NULL);

    // spin-lock until the child is done
    // IMPORTANT: NOTE: Grossly inefficient since the main thread is wasting CPU
    // cycles just checking this while loop continuously.
    while(!childDone)
    {
    }

    Logger::LogInfoUnformatted("main function end!\n");

    pause();
    FreeConsole();
    return 0;
}

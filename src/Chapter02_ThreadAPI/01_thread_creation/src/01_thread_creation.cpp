#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    Logger::LogInfoUnformatted("Hello from a thread!\n");
    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    TCreate(t1, ThreadProc);
    Logger::LogInfoUnformatted("Hello From the Main Function!\n");

    // pause();
    FreeConsole();
    return 0;
}

#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>

static volatile i32 Counter = 0;

DWORD WINAPI 
ThreadProc(LPVOID Param)
{
    const char *P = (const char *)Param;
    Win32Logger::LogInfo("Began Thread %s\n", P);

    for (i32 i = 0; i < 1e7; ++i)
    {
        ++Counter;
    }
    
    Win32Logger::LogInfo("Ended Thread %s\n", P);
    
    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Win32Logger::LogInfoUnformatted("Started The Main Function!");

    TCreate(t1, ThreadProc);
    TCreate(t2, ThreadProc);

    WaitForSingleObject(t1.handle, INFINITE);
    WaitForSingleObject(t2.handle, INFINITE);

    Win32Logger::LogInfo(
        "The Counter Value at the end of both the threads is %d.\n", Counter);

    pause();
    FreeConsole();
    return 0;
}

#include <windows.h>

#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <defines.h>

#define USE_SYNCHRONIZATION 1

static volatile i32 Counter = 0;
CRITICAL_SECTION CriticalSection;

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    const char *P = (const char *)Param;
    Logger::LogInfo("Began Thread %s\n", P);

    for (i32 i = 0; i < 1e7; ++i)
    {
#if USE_SYNCHRONIZATION
        EnterCriticalSection(&CriticalSection);
        ++Counter;
        LeaveCriticalSection(&CriticalSection);
#else
        ++Counter;
#endif
    }

    Logger::LogInfo("Ended Thread %s\n", P);

    return TRUE;
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
        i32 CmdShow)
{
    InitializeCriticalSection(&CriticalSection);

    thread_handle t1 = {.arg = (void *)"t1"};
    t1.handle = CreateThread(NULL, 0, ThreadProc, t1.arg, 0, &t1.id);
    thread_handle t2 = {.arg = (void *)"t2"};
    t2.handle = CreateThread(NULL, 0, ThreadProc, t2.arg, 0, &t2.id);

    WaitForSingleObject(t1.handle, INFINITE);
    WaitForSingleObject(t2.handle, INFINITE);

    Logger::LogInfo("Counter value at the end of both threads is: %d\n",
                    Counter);

    DeleteCriticalSection(&CriticalSection);

    pause();
    FreeConsole();
    return 0;
}

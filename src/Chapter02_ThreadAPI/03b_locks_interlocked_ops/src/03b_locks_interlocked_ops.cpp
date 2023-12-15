#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>

#define USE_SYNCHRONIZATION 1
#define USE_INTERLOCKED_INCREMENT 0

static volatile i32 Counter = 0;

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    const char *P = (const char *)Param;
    Logger::LogInfo("Began Thread %s\n", P);
    
    for (i32 i = 0; i < 1e7; ++i)
    {
#if USE_SYNCHRONIZATION
#if USE_INTERLOCKED_INCREMENT
        InterlockedIncrement((LONG volatile *)&Counter);
#else
        i32 CurrentCounter = Counter;
        i32 NewCounter = CurrentCounter + 1;
        u32 OriginalCounterValue = InterlockedCompareExchange((LONG volatile *)&Counter,
                                                              NewCounter, CurrentCounter);
        if (OriginalCounterValue != CurrentCounter)
        {
            // NOTE: Retry the same loop iteration
            --i;
        }
#endif
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
    thread_handle t1 = {.arg = (void *)"t1"};
    t1.handle = CreateThread(NULL, 0, ThreadProc, t1.arg, 0, &t1.id);
    thread_handle t2 = {.arg = (void *)"t2"};
    t2.handle = CreateThread(NULL, 0, ThreadProc, t2.arg, 0, &t2.id);
    
    WaitForSingleObject(t1.handle, INFINITE);
    WaitForSingleObject(t2.handle, INFINITE);
    
    Logger::LogInfo("Counter value at the end of both threads is: %d\n", Counter);
    
    pause();
    FreeConsole();
    return 0;
}

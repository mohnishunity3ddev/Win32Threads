#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>

static volatile i32 Counter = 0;
static volatile b32 Ready = false;

#define USE_SYNCHRONIZATION 1

#if USE_SYNCHRONIZATION
static HANDLE Mutex = INVALID_HANDLE_VALUE;
static CONDITION_VARIABLE ConditionVar;
static CRITICAL_SECTION CriticalSection;
#endif

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    const char *P = (const char *)Param;
    Logger::LogInfo("Began Thread %s\n", P);

    EnterCriticalSection(&CriticalSection);

    while(!Ready)
    {
        SleepConditionVariableCS(&ConditionVar, &CriticalSection, INFINITE);
    }

    LeaveCriticalSection(&CriticalSection);

    WakeConditionVariable(&ConditionVar);

    Logger::LogInfo("Ended Thread %s\n", P);
    
    return TRUE;
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
#if USE_SYNCHRONIZATION
    // Mutex = CreateMutex(NULL, FALSE, NULL);
    // if(Mutex == INVALID_HANDLE_VALUE)
    // {
    //     Logger::LogFatalUnformatted("Could not create the Mutex Object.\n");
    //     return -1;
    // }
    // ASSERT(Mutex != INVALID_HANDLE_VALUE);
    
    InitializeCriticalSection(&CriticalSection);
    
    InitializeConditionVariable(&ConditionVar);
#endif
    
    thread_handle t1 = {.arg = (void *)"t1"};
    t1.handle = CreateThread(NULL, 0, ThreadProc, t1.arg, 0, &t1.id);
    thread_handle t2 = {.arg = (void *)"t2"};
    t2.handle = CreateThread(NULL, 0, ThreadProc, t2.arg, 0, &t2.id);
    
    WaitForSingleObject(t1.handle, INFINITE);
    WaitForSingleObject(t2.handle, INFINITE);
    
    Logger::LogInfo("Counter value at the end of both threads is: %d\n", Counter);
    
    DeleteCriticalSection(&CriticalSection);

    pause();
    FreeConsole();
    return 0;
}

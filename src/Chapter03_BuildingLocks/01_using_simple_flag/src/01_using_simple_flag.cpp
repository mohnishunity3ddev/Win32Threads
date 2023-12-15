#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>

static volatile i32 Counter = 0;

#define USE_SYNCHRONIZATION 1

struct lock
{
    i32 flag;
};

#if USE_SYNCHRONIZATION
static lock Mutex = {};
#endif

void
Init(lock *Mutex)
{
    // NOTE: Lock is available to be claimed.
    Mutex->flag = 0;
}

// IMPORTANT: NOTE:
// This does not provide Mutual Exclusion. Imagine a situation here,
// 1. flag is 0.
// 2. Thread 1 checks for flag == 1 and as it is setting the flag to 1, OS Switches to Thread 2.
// 3. Thread 2 checks for flag == 1 and sets flag = 1. OS now switches to thread 1
// 4. Thread 1 sets the flag as 1.
// This shows both enter their critical section.
void
Lock(lock *Mutex)
{
    // NOTE: Spin Wait
    while(Mutex->flag == 1)
    {
    }

    Mutex->flag = 1;
}

void
Unlock(lock *Mutex)
{
    Mutex->flag = 0;
}

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    const char *P = (const char *)Param;
    Logger::LogInfo("Began Thread %s\n", P);

    for (i32 i = 0; i < 1e5; ++i)
    {
#if USE_SYNCHRONIZATION
        // NOTE: Take Ownership of the Mutex Object
        Lock(&Mutex);
        ++Counter;
        Unlock(&Mutex);
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
#if USE_SYNCHRONIZATION
    Init(&Mutex);
#endif

    thread_handle t1 = {.arg = (void *)"t1"};
    t1.handle = CreateThread(NULL, 0, ThreadProc, t1.arg, 0, &t1.id);
    thread_handle t2 = {.arg = (void *)"t2"};
    t2.handle = CreateThread(NULL, 0, ThreadProc, t2.arg, 0, &t2.id);

    WaitForSingleObject(t1.handle, INFINITE);
    WaitForSingleObject(t2.handle, INFINITE);

    Unlock(&Mutex);

    Logger::LogInfo("Counter value at the end of both threads is: %d\n", Counter);

    pause();
    FreeConsole();
    return 0;
}

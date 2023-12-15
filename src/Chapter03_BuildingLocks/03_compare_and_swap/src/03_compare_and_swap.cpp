#include <windows.h>

#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <defines.h>

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
// This is what cmpxchg assembly instruction or the InterlockedCompareExchange
// in Win32 Does. This also has to be atomic. Here, it wont work because we cant
// guarantee this function to be atomic.
i32
CompareAndSwap(i32 *Ptr, i32 Expected, i32 New)
{
    i32 Original = *Ptr;
    if(Original == Expected)
    {
        *Ptr = New;
    }

    return Original;
}

void
Lock(lock *Mutex)
{
    // NOTE: Spin Wait
    // Here we are saying check the mutex flag, The Expected value should be 0
    // meaning it is free to hold, hold it by setting the flag to 1 if that's
    // the case. If the lock was already held, spin-lock here(meaning if it was
    // not expected). This does not deadlock as testAndSwap was doing. 
    while ((CompareAndSwap(&Mutex->flag, 0, 1) == 1))
    {
    }
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

    Logger::LogInfo("Counter value at the end of both threads is: %d\n",
                    Counter);

    pause();
    FreeConsole();
    return 0;
}

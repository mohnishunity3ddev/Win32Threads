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

// IMPORTANT: NOTE: Basic Spin Lock.
// This is what hardware support for atomicity does. The assembly instruction is
// xchg in win32 which is what Interlocked Operations do. Here, this is not
// going to be an atomic operation so this also does not work unless we get the
// hardware support to do this function atomically i.e. all 3 instructions are
// run or none are run at all.
i32
TestAndSet(i32 *OldPtr, i32 New)
{
    i32 Old = *OldPtr;
    *OldPtr = New;
    return Old;
}

void
Lock(lock *Mutex)
{
    // NOTE: Spin Wait
    while ((TestAndSet(&Mutex->flag, 1) == 1))
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

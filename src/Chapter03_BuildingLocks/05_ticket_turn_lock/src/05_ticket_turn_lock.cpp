#include <windows.h>

#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <defines.h>

static volatile i32 Counter = 0;

#define USE_SYNCHRONIZATION 1

struct lock
{
    i32 ticket;
    i32 turn;
};

#if USE_SYNCHRONIZATION
static lock Mutex = {};
#endif

void
Init(lock *Mutex)
{
    // NOTE: Lock is available to be claimed.
    Mutex->ticket = 0;
    Mutex->turn = 0;
}

// NOTE: Also an atomic hardware primitive.
i32
FetchAndAdd(i32* Ptr)
{
    i32 Old = *Ptr;
    *Ptr = Old + 1;
    return Old;
}

void
Lock(lock *Mutex)
{
    i32 MyTurn = FetchAndAdd(&Mutex->ticket);
    while (Mutex->turn != MyTurn)
    {
        // Yield the CPU to some other thread since the lock is held instead of
        // spinning here and waste CPU cycles. Basically, DESchedule itself. Or
        // Yield the CPU to someone else.
        SwitchToThread();
    }
}

void
Unlock(lock *Mutex)
{
    // NOTE: This ensures that the ticket number increases, so other threads
    // also have a fair chance to execute their critical section. This ensures
    // fairness in lock-acquisition.
    Mutex->ticket = Mutex->ticket + 1;
}

DWORD WINAPI ThreadProc(LPVOID Param)
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

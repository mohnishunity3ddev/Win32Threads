#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>

static volatile b32 childDone = false;
static CRITICAL_SECTION criticalSection;
static CONDITION_VARIABLE condVar;

// IMPORTANT: NOTE:
// This is a demo of conditional variable working.
// Here, the main thread is waiting on the child thread to complete fully before
// continuing.
// The main thread checks to see if the child is done. if it is not, then sleeps
// on the conditional variable waiting for it to get signaled and then sleeps.
// also, releasing the lock on it. The OS makes a queue for the condition
// variable of all threads that are waiting for it to get signaled. in the child
// thread, when it is done, it wakes up all the threads that are waiting on this
// condition variable.
//
// NOTE: Explanation of why we need the critical section "locks", lets assume we
// don't use the locks here. what will happen? - in the absence of locks, let's
// say the main threads calls the waitForchild() function, it checks if the
// child is done, which it won't since main ran first. let's say it finds the
// childIsDone, and then immediately the child thread runs, it sets the
// childIsDone bool, it signals the condition variable and triggers wake for all
// the threads waiting on it, since there aren't any at this point, it won't
// wake up anyone. Here, the OS now schedules the main thread which saw before
// the child is not done, then it triggers a sleep on the condition variable,
// and it will keep sleeping since the wake signal was already signaled by the
// child thread before, so main will sleep on forever.
//
// NOTE: Explanation of why we need the childIsDone variable for syncing here.
// let's assume there isn't any right now. let's say the OS schedules the child
// thread first, it goes through it's routine and signals the conditon var and
// wakes up any thread waiting on it, which there isn't any at this point. Then
// OS schedules the main thread, which enters its critical section, and sleeps
// itself waiting for the condition var to get signaled. And here, it will again
// sleep on forever since the child had already signaled and it won't be doing
// so again.

_THREAD_PROC(child)
{
    // NOTE: simulate expensive work.
    Logger::LogInfoUnformatted("child began!\n");
    Sleep(5000);

    EnterCriticalSection(&criticalSection);
    childDone = true;
    Logger::LogInfoUnformatted("child end!\n");
    // Signal the condition variable and wake all threads waiting on it.
    WakeConditionVariable(&condVar);
    LeaveCriticalSection(&criticalSection);

    ExitThread(0);
}

void
waitForChild()
{
    EnterCriticalSection(&criticalSection);
    while (!childDone)
    {
        Logger::LogInfoUnformatted("main sleeping!\n");
        // NOTE: Sleep the thread and wait on condVar to get signaled.
        SleepConditionVariableCS(&condVar, &criticalSection, INFINITE);
    }
    LeaveCriticalSection(&criticalSection);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("main began!\n");

    InitializeConditionVariable(&condVar);
    InitializeCriticalSection(&criticalSection);

    thread t{};
    t.start(child, NULL);

    waitForChild();

    Logger::LogInfoUnformatted("main end!\n");

    pause();
    FreeConsole();
    return 0;
}

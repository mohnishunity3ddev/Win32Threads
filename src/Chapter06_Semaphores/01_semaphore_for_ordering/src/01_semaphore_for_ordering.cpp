#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>

// NOTE: Semaphores can be used as ordering operations. Here, we want to make
// sure the child thread runs first followed by the parent.
HANDLE hSemaphore = NULL;

_THREAD_PROC(childProc)
{
    Logger::LogInfoUnformatted("child!\n");

    Sleep(500);
    // NOTE: second argument here is the amount we want to increment the
    // semaphore count by(which is 1 in this case). third argument is pointer to
    // the variable which stores the previous count of the semaphore(which is
    // null in this case since we don't care about that).
    ASSERT(ReleaseSemaphore(hSemaphore, 1, NULL));

    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
        i32 CmdShow)
{
    // NOTE: The initial count value is 0, the max it can go is 1.
    hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);

    thread child{};
    child.start(childProc, nullptr);

    Logger::LogInfoUnformatted("Waiting for child started!\n");

    // IMPORTANT: NOTE:
    // There are two cases to consider. First, let us assume that the parent
    // creates the child but the child has not run yet (i.e., it is sitting in a
    // ready queue but not running). In this case the parent will call sem
    // wait() before the child has called sem post(); we’d like the parent to
    // wait for the child to run. The only way this will happen is if the value
    // of the semaphore is not greater than 0; hence, 0 is the initial value.
    // The parent runs, decrements the semaphore (to -1), then waits (sleeping).
    // When the child finally runs, it will call sem post(), increment the value
    // of the semaphore to 0, and wake the parent, which will then return from
    // sem wait() and finish the program.
    // The second case occurs when the child runs to completion before the
    // parent gets a chance to call sem wait(). In this case, the child will
    // first call sem post(), thus incrementing the value of the semaphore from
    // 0 to 1. When the parent then gets a chance to run, it will call sem
    // wait() and find the value of the semaphore to be 1; the parent will thus
    // decrement the value (to 0) and return from sem wait() without waiting,
    // also achieving the desired effect.
    DWORD result = WaitForSingleObject(hSemaphore, INFINITE);
    ASSERT(result == WAIT_OBJECT_0);

    Logger::LogInfoUnformatted("parent!\n");

    pause();
    FreeConsole();
    CloseHandle(hSemaphore);
    return 0;
}

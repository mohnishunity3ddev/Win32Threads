#include <windows.h>

#include <defines.h>
#include <Win32Logger.h>

// IMPORTANT: NOTE:
// Joinable Threads - This means that threads that are created are joined. All
// the thread's resources are released after the joining completes and the
// result of the thread has been passed on to the caller thread.
//
// Detached threads - do not need to be joined. They dont care about the result.
// They do the work and go away silently without joining the result. They
// release their resources as soon as they are terminated.
//
// We will write code here to make two threads both of which will be joinable
// and their job will be to sqaure the number passed to them and return the
// result to the main thread by joining.

struct thread_info
{
    i32 ThreadID;
    f64 InputNumber;
    f64 Result;
};

DWORD WINAPI
ThreadProc(LPVOID lpParameter)
{
    thread_info *Info = (thread_info *)lpParameter;
    
    for(i32 i = 0; i < Info->InputNumber; ++i)
    {
        LogInfo("Thread %d is computing the square of %f\n", Info->ThreadID,
                Info->InputNumber);
        Sleep(1000);
    }

    Info->Result = Info->InputNumber*Info->InputNumber;
    ExitThread(0);
}

HANDLE Thread1 = NULL;
HANDLE Thread2 = NULL;

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
          i32 CmdShow)
{
    thread_info *threadInfo1 = new thread_info();
    threadInfo1->InputNumber = 3.0;
    threadInfo1->Result = 0.0;
    threadInfo1->ThreadID = 1;
    thread_info *threadInfo2 = new thread_info();
    threadInfo2->InputNumber = 10.0;
    threadInfo2->Result = 0.0;
    threadInfo2->ThreadID = 2;
    
    Thread1 = CreateThread(NULL, 0, ThreadProc, (void *)threadInfo1, 0, NULL);
    Thread2 = CreateThread(NULL, 0, ThreadProc, (void *)threadInfo2, 0, NULL);
    LogDebug("Created two threads inside the main function\n");

    WaitForSingleObject(Thread1, INFINITE);
    CloseHandle(Thread1);
    Thread1 = NULL;
    LogWarn("The Result of Thread %d for computing the square of %f is %f.\n",
            threadInfo1->ThreadID, threadInfo1->InputNumber,
            threadInfo1->Result);
    
    WaitForSingleObject(Thread2, INFINITE);
    CloseHandle(Thread2);
    Thread2 = NULL;
    LogWarn("The Result of Thread %d for computing the square of %f is %f.\n",
            threadInfo2->ThreadID, threadInfo2->InputNumber,
            threadInfo2->Result);

    FreeConsole();
    return 0;
}

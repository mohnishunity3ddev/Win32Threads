#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>

DWORD WINAPI
ThreadFunction(LPVOID lpParameter)
{
    i32 a = 0;
    while(a < 10)
    {
        Logger::LogInfo("I am in thread %s with Id %u\n", (const char *)lpParameter,
                GetCurrentThreadId());
        
        if (a == 3)
        {
            // IMPORTANT: NOTE: ExitThread(0) will force quit the thread.
            Logger::LogUnformatted("Exiting Prematurely from thread!\n");
            ExitThread(0);
        }
        
        // NOTE: Sleep for 1000 miliseconds or 1 second.
        Sleep(1000);
        a++;
    }
    return 0;
}

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
          i32 CmdShow)
{
    TCreate(h1, ThreadFunction);
    
    // IMPORTANT: NOTE:
    // This will terminate the main thread. But the thing is that the child
    // threads will continue to execute. If this main thread naturally exits by
    // returning 0, then all child threads will end even if they were executing.
    // But exiting the main thread like this will not end any child threads like
    // the one above to end.
    // ExitThread(0);
    
    // IMPORTANT: NOTE:
    // Race Condition on thread creation:
    // We do not know when this log instruction will be executed after the
    // thread creation above or the first instruction inside the thread will be
    // executed. It depends on the CPU which thread is scheduled first and the
    // first instruction in that thread will be executed first.
    Logger::LogInfoUnformatted("Hello, World From Main Function!\n");
    
    // IMPORTANT: NOTE:
    // if we don't pause the main function, the main will go on, and the thread
    // h1 we created will stop executing. but if we WaitForh1 to complete in
    // this main function, the main function effectively will be paused. because
    // in the thread we have a never ending while loop.
    pause();
    WaitForSingleObject(h1.handle, 0xFFFFFFFF);
    CloseHandle(h1.handle); // <- Wait for the thread to complete before
                            // anything after this in the main function.
    Console::FreeConsoles();
    
    // LogInfoUnformatted("Main Function Terminated\n");
    return 0;
}

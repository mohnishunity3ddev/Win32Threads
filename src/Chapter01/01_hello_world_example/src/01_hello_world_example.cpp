#include <windows.h>

#include <defines.h>
#include <Win32Logger.h>

DWORD WINAPI
ThreadFunction(LPVOID lpParameter)
{
    i32 a = 0;
    while(a < 10)
    {
        LogInfo("I am in thread %s with Id %u\n", (const char *)lpParameter,
                GetCurrentThreadId());
        
        if (a == 3)
        {
            // IMPORTANT: NOTE: ExitThread(0) will force quit the thread.
            LogUnformatted("Exiting Prematurely from thread!\n");
            ExitThread(0);
        }
        
        // NOTE: Sleep for 1000 miliseconds or 1 second.
        Sleep(1000);
        a++;
    }
    return 0;
}

struct thread_handle
{
    HANDLE handle;
    DWORD id;
    const char *message;
};

#define TCreate(Name)                                                          \
    thread_handle Name;                                                        \
    const char *message = #Name;                                               \
    {                                                                          \
        Name.message = #Name;                                                  \
        CreateThread(&Name);                                                   \
    }

#define TFree(Name)                                                            \
    WaitForSingleObject(Name.handle, INFINITE);                                \
    CloseHandle(Name.handle)

void
CreateThread(thread_handle *Handle)
{
    Handle->handle = CreateThread(NULL, 0, ThreadFunction,
                                  (void *)Handle->message, 0, &Handle->id);
    
    if(Handle->handle == NULL)
    {
        LogErrorUnformatted("Could not create the thread!");
    }
}

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
          i32 CmdShow)
{
    TCreate(h1);
    
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
    LogInfoUnformatted("Hello, World From Main Function!\n");
    
    // IMPORTANT: NOTE:
    // if we don't pause the main function, the main will go on, and the thread
    // h1 we created will stop executing. but if we WaitForh1 to complete in
    // this main function, the main function effectively will be paused. because
    // in the thread we have a never ending while loop.
    pause();
    TFree(h1); // <- Wait for the thread to complete before anything after this in the main function.
    Console::FreeConsoles();
    
    // LogInfoUnformatted("Main Function Terminated\n");
    return 0;
}

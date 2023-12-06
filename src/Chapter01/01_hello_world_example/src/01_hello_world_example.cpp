#include <windows.h>

#include <defines.h>
#include <Win32Logger.h>

DWORD WINAPI
ThreadFunction(LPVOID lpParameter)
{
    while(1)
    {
        LogInfo("I am in thread %s with Id %u\n", (const char *)lpParameter,
                GetCurrentThreadId());
        Sleep(1000);
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
    {                                                                          \
        const char *message = #Name;                                           \
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
    
    LogInfoUnformatted("Hello, World From Main Function!\n");

    // IMPORTANT: NOTE:
    // if we don't pause the main function, the main will go on, and the thread
    // h1 we created will stop executing. but if we WaitForh1 to complete in
    // this main function, the main function effectively will be paused. because
    // in the thread we have a never ending while loop.
    
    // pause();
    // TFree(h1); // <- Wait for the thread to complete before anything after this in the main function.
    Console::FreeConsoles();

    // LogInfoUnformatted("Main Function Terminated\n");
    return 0;
}

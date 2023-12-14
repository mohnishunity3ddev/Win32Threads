#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>

static volatile i32 Counter = 0;

#define USE_SYNCHRONIZATION 1

#if USE_SYNCHRONIZATION
static HANDLE Mutex = INVALID_HANDLE_VALUE;
#endif

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    const char *P = (const char *)Param;
    Logger::LogInfo("Began Thread %s\n", P);
    
    for (i32 i = 0; i < 1e6; ++i)
    {
#if USE_SYNCHRONIZATION
        // NOTE: Take Ownership of the Mutex Object
        DWORD WaitResult = WaitForSingleObject(Mutex, INFINITE);
        
        switch(WaitResult)
        {
            case WAIT_OBJECT_0:
            {
                ++Counter;
                
                if(!ReleaseMutex(Mutex))
                {
                    Logger::LogFatalUnformatted( "Unable to release the mutex object!");
                }
            } break;
            
            case WAIT_ABANDONED:
            {
                return FALSE;
            } break;
            
            INVALID_DEFAULT
        }
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
    Mutex = CreateMutex(NULL, FALSE, NULL);
    if(Mutex == INVALID_HANDLE_VALUE)
    {
        Logger::LogFatalUnformatted("Could not create the Mutex Object.\n");
        return -1;
    }
    ASSERT(Mutex != INVALID_HANDLE_VALUE);
#endif
    
    thread_handle t1 = {.arg = (void *)"t1"};
    t1.handle = CreateThread(NULL, 0, ThreadProc, t1.arg, 0, &t1.id);
    
    thread_handle t2 = {.arg = (void *)"t2"};
    t2.handle = CreateThread(NULL, 0, ThreadProc, t2.arg, 0, &t2.id);
    
    WaitForSingleObject(t1.handle, INFINITE);
    WaitForSingleObject(t2.handle, INFINITE);
    
    Logger::LogInfo("Counter value at the end of both threads is: %d\n", Counter);
    
    pause();
    FreeConsole();
    return 0;
}

#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>

#include <memory>

struct thread_result
{
    i32 resultCode;
    char string[256];
};

struct thread_args
{
    char Name[128];
    i32 a;
    f32 b;
    thread_result result;
};

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    thread_args *P = (thread_args *)Param;
    
    Logger::LogInfo("Received Data: a = %d, b = %f, Name = %s\n", P->a, P->b,
                    P->Name);

    StringCopy(P->result.string, "I have received the data you sent!");
    P->result.resultCode = 204;
    
    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("Hello, World from the main function!\n");
    
    size_t size = sizeof(thread_args);
    
    thread_args *Args = (thread_args *)malloc(sizeof(thread_args));
    Args->a = 221;
    Args->b = 2.324;
    const char *Name = "Mohnish Sharma";
    size_t Len = StringLength(Name);
    StringCopy(Args->Name, Name);
    
    thread_handle T1 = {.arg = (void *)Args};
    T1.handle = CreateThread(NULL, 0, ThreadProc, T1.arg, 0, &T1.id);
    
    WaitForSingleObject(T1.handle, INFINITE);

    Logger::LogInfo("Received -> Result Code: %d, Result String: %s\n",
                    Args->result.resultCode, Args->result.string);

    CloseHandle(T1.handle);
    free(Args);
    
    // pause();
    FreeConsole();
    return 0;
}

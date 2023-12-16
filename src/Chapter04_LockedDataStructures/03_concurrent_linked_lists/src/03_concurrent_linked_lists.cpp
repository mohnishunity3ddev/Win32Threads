#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/mutex.h>
#include <Containers/containers.h>

struct args
{
    concurrent_list *list;
    i32 count;
    b32 odd;
};

DWORD WINAPI
thread_proc(LPVOID param)
{
    args *arg = (args *)param;

    for(i32 i = arg->count - 1; i >= 0 ; --i)
    {
        if(arg->odd)
        {
            arg->list->insert(2*i + 1);
        }
        else
        {
            arg->list->insert(2*i);
        }
    }

    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("Inside main function!\n");
    concurrent_list list{};

    args arg0 = {.list = &list, .count = 1000, .odd = true};
    args arg1 = {.list = &list, .count = 1000, .odd = false};
    HANDLE t0 = CreateThread(NULL, 0, thread_proc, (void *)&arg0, 0, NULL);
    HANDLE t1 = CreateThread(NULL, 0, thread_proc, (void *)&arg1, 0, NULL);

    WaitForSingleObject(t0, INFINITE);
    WaitForSingleObject(t1, INFINITE);

    Logger::LogInfo("both threads have ended. list count is: %d; should be %d!\n", list.getCount(), (arg0.count + arg1.count));
    list.print();

    //list.cleanup();
    pause();
    FreeConsole();
    return 0;
}

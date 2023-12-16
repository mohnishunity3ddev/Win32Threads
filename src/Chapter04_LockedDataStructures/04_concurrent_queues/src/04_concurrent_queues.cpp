#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Containers/containers.h>

struct args
{
    concurrent_queue *queue;
    i32 count;
    b32 add;
};

DWORD WINAPI
thread_proc(LPVOID param)
{
    args *arg = (args *)param;

    for(i32 i = 0; i < arg->count; ++i)
    {
        if(arg->add)
        {
            arg->queue->enqueue(i);
            Logger::LogInfo("Enqueueing %d.\n", i);
        }
        else
        {
            i32 dequeuedVal;
            if(arg->queue->dequeue(&dequeuedVal) == -1)
            {
                Logger::LogInfo("trying to dequeue failed since queue is empty!\n");
            }
            else
            {
                Logger::LogInfo("dequeued %d.\n", dequeuedVal);
            }
        }
    }

    ExitThread(0);
}


i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
        i32 CmdShow)
{
    Logger::LogInfoUnformatted("Inside main function!\n");
    concurrent_queue queue{};

    args arg0 = {.queue = &queue, .count = 1000, .add = true};
    args arg1 = {.queue = &queue, .count = 1000, .add = false};
    HANDLE t0 = CreateThread(NULL, 0, thread_proc, (void *)&arg0, 0, NULL);
    // NOTE: Sleep before creating the dequeue thread to give the enqueue thread
    // some time to add items which can be dequeued by this following thread.
    Sleep(5);
    HANDLE t1 = CreateThread(NULL, 0, thread_proc, (void *)&arg1, 0, NULL);
    if (t0 == NULL || t1 == NULL)
    {
        Logger::LogFatalUnformatted("Can't continue since thread creation failed!\n");
        return -1;
    }
    
    WaitForSingleObject(t0, INFINITE);
    WaitForSingleObject(t1, INFINITE);

    Logger::LogInfo("both threads have ended. queue count is: %d; should be 0!\n", queue.count());

    CloseHandle(t0);
    CloseHandle(t1);

    pause();
    FreeConsole();
    return 0;
}

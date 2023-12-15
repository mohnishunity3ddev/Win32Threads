#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <memory>

HANDLE Mutex = NULL;

static volatile i32 Counter = 0;

class counter
{
  private:
    i32 Value;
    HANDLE Lock;

  public:
    void Init()
    {
        this->Value = 0;
        this->Lock = CreateMutex(NULL, FALSE, NULL);
    }

    void Increment()
    {
        DWORD WaitResult = WaitForSingleObject(this->Lock, INFINITE);
        if(WaitResult != WAIT_OBJECT_0)
        {
            ASSERT(!"Should not be here!");
            return;
        }

        ++this->Value;

        if(!ReleaseMutex(this->Lock))
        {
            ASSERT(!"Should not be here!");
        }
    }

    void Decrement()
    {
        DWORD WaitResult = WaitForSingleObject(this->Lock, INFINITE);
        if (WaitResult != WAIT_OBJECT_0)
        {
            ASSERT(!"Should not be here!");
            return;
        }

        --this->Value;

        if (!ReleaseMutex(this->Lock))
        {
            ASSERT(!"Should not be here!");
        }
    }

    i32 Get()
    {
        DWORD WaitResult = WaitForSingleObject(this->Lock, INFINITE);
        if (WaitResult != WAIT_OBJECT_0)
        {
            ASSERT(!"Should not be here!");
            return -1;
        }

        i32 Result = this->Value;

        if (!ReleaseMutex(this->Lock))
        {
            ASSERT(!"Should not be here!");
        }

        return Result;
    }
};

struct thread_args
{
    counter *Counter;
    b32 Increment;
};

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    thread_args *Arg = (thread_args *)Param;
    if(Arg == nullptr)
    {
        ASSERT(!"Something wrong here!");
    }

    for(i32 Index = 0; Index < 1e6; ++Index)
    {
#if 1
        if(Arg->Increment)
        {
            Arg->Counter->Increment();
        }
        else
        {
            Arg->Counter->Decrement();
        }
#else
        Arg->Counter->Increment();
#endif
    }

    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Mutex = CreateMutex(NULL, FALSE, NULL);
    if(Mutex == NULL)
    {
        Logger::LogFatal("Unable to create Mutex!\n");
        return -1;
    }

    counter *Counter = (counter *)malloc(sizeof(counter));
    Counter->Init();
    thread_args *Arg1 = (thread_args *)malloc(sizeof(thread_args));
    Arg1->Counter = Counter;
    Arg1->Increment = true;
    thread_args *Arg2 = (thread_args *)malloc(sizeof(thread_args));
    Arg2->Counter = Counter;
    Arg2->Increment = false;

    Logger::LogInfo("Main Function: Starting the threads!\n");

    HANDLE t1 = CreateThread(NULL, 0, ThreadProc, (void *)Arg1, 0, NULL);
    HANDLE t2 = CreateThread(NULL, 0, ThreadProc, (void *)Arg2, 0, NULL);

    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);

    Logger::LogInfo("Both Threads are done. Value of Counter is: %d!\n", Counter->Get());

    pause();
    FreeConsole();
    return 0;
}

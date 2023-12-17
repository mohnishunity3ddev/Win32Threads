#include <windows.h>

#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <defines.h>
#include <memory>
#include <Thread/thread_api.h>

static volatile i32 Counter = 0;
#define NUMCPUS 6
#define NumThreads 20

class counter
{
  private:
    i32 globalValue;
    mutex globalLock;
    i32 localValues[NUMCPUS];
    mutex localLocks[NUMCPUS];
    i32 updateFrequency;

  public:
    counter(i32 updateFreq)
        : globalValue(0), updateFrequency(updateFreq), globalLock{},
          localValues{}, localLocks{}
    {
        globalLock.init();

        for(i32 i = 0; i < NUMCPUS; ++i)
        {
            localLocks[i].init();
        }
    }

    ~counter() { cleanup(); }

    void
    cleanup()
    {
        globalValue = 0;
        updateFrequency = 0;
        globalLock.cleanup();

        for (i32 i = 0; i < NUMCPUS; ++i)
        {
            localValues[i] = 0;
            localLocks[i].cleanup();
        }
    }

    counter(const counter &other) = delete;
    counter(counter &&other) = delete;
    counter &operator=(const counter &other) = delete;
    counter &operator=(counter &&other) = delete;

    void
    update(DWORD threadId, i32 amount)
    {
        u32 threadIndex = threadId % NUMCPUS;

        this->localLocks[threadIndex].lock();
        this->localValues[threadIndex] += amount;

        if(this->localValues[threadIndex] >= this->updateFrequency)
        {
            this->globalLock.lock();
            this->globalValue += this->localValues[threadIndex];
            this->globalLock.unlock();

            this->localValues[threadIndex] = 0;
        }

        this->localLocks[threadIndex].unlock();
    }

    i32
    get()
    {
        this->globalLock.lock();
        i32 result = this->globalValue;
        this->globalLock.unlock();

        return result;
    }
};

struct thread_args
{
    counter *counter;

    DWORD threadId;
    i32 amount;
};

DWORD WINAPI
ThreadProc(LPVOID Param)
{
    thread_args *Arg = (thread_args *)Param;
    ASSERT(Arg != nullptr && Arg->counter != nullptr);

    for(i32 i = 0; i < 1024*1024; ++i)
    {
        Arg->counter->update(Arg->threadId, Arg->amount);
    }

    Logger::LogInfo("T%d has completed!\n", (Arg->threadId % NUMCPUS));
    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
        i32 CmdShow)
{
    {
        counter Counter{1024};

        HANDLE threads[NumThreads];
        thread_args args[NumThreads];

        Logger::LogInfoUnformatted("Main Function: Starting the threads!\n");
        for(i32 threadIndex = 0;
            threadIndex < NumThreads;
            ++threadIndex)
        {
            args[threadIndex] = {.counter = &Counter, .amount = 1};
            threads[threadIndex] = CreateThread(NULL, 0, ThreadProc, (void *)&args[threadIndex], 0, &args[threadIndex].threadId);

            ASSERT(threads[threadIndex] != NULL);
        }
        Logger::LogInfoUnformatted("Initialized all threads! Waiting!\n");

        WaitForMultipleObjects(NumThreads, threads, TRUE, INFINITE);

        Logger::LogInfo("All threads are done. Value of counter is: %d; should be: %u!\n",
                        Counter.get(), (1024*1024*NumThreads));

        for(i32 threadIndex = 0;
            threadIndex < NumThreads;
            ++threadIndex)
        {
            CloseHandle(threads[threadIndex]);
            threads[threadIndex] = NULL;
        }
    }

    pause();
    FreeConsole();
    return 0;
}

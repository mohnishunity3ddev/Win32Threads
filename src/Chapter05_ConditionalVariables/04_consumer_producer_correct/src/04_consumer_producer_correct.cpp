#include <windows.h>

#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>
#include <defines.h>
#include <stdlib.h>

// NOTE: This is simple single buffer producer consumer example. Buffer length
// is 1.

// NOTE: queueNotFull is signal to the producers that the queue is not full, so
// it should go ahead to put values inside the buffer.
// queueNotEmpty is signal to the consumers that there is something in the queue
// that you can safely allowed to consume.
CONDITION_VARIABLE queueNotFull, queueNotEmpty;
CRITICAL_SECTION lock;

volatile b32 stopRequested = false;
volatile i32 count = 0;
volatile i32 buffer = 0;

void
put(i32 value)
{
    ASSERT(count == 0);
    count = 1;
    buffer = value;
}

i32
get()
{
    ASSERT(count == 1);
    count = 0;
    return buffer;
}

_THREAD_PROC(producerThreadProc)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("Producer %d began!\n", id);
    while (true)
    {
        Sleep(rand() % 200);
        EnterCriticalSection(&lock);

        while(!stopRequested && count == 1)
        {
            SleepConditionVariableCS(&queueNotFull, &lock, INFINITE);
        }

        if(stopRequested)
        {
            Logger::LogInfo("producer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        i32 value = rand() % 20;
        put(value);
        Logger::LogInfo("Producer %d filled up the buffer with value: %d\n", id, value);
        WakeConditionVariable(&queueNotEmpty);

        LeaveCriticalSection(&lock);
    }

    Logger::LogInfo("Producer %d end!\n", id);
    ExitThread(0);
}

_THREAD_PROC(consumerThreadProc)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("Consumer %d began!\n", id);
    while (true)
    {
        Sleep(rand() % 500);

        EnterCriticalSection(&lock);

        while(!stopRequested && count == 0)
        {
            SleepConditionVariableCS(&queueNotEmpty, &lock, INFINITE);
        }

        if(stopRequested)
        {
            Logger::LogInfo("consumer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        i32 consumedVal = get();
        Logger::LogInfo("consumer %d consumed %d\n", id, consumedVal);

        WakeConditionVariable(&queueNotFull);
        LeaveCriticalSection(&lock);
    }

    Logger::LogInfo("Consumer %d end!\n", id);
    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
        i32 CmdShow)
{
    Logger::LogInfoUnformatted("main function!\n");

    InitializeConditionVariable(&queueNotFull);
    InitializeConditionVariable(&queueNotEmpty);
    InitializeCriticalSection(&lock);

    thread tConsumer1{}, tConsumer2{}, tProducer{};

    i32 c0 = 0;
    i32 c1 = 1;
    i32 p0 = 0;
    tProducer.start(producerThreadProc, (void *)&c0);
    tConsumer1.start(consumerThreadProc, (void *)&c1);
    tConsumer2.start(consumerThreadProc, (void *)&p0);

    pause();
    EnterCriticalSection(&lock);
    stopRequested = true;
    LeaveCriticalSection(&lock);

    WaitForSingleObject(tConsumer1.getHandle(), INFINITE);
    WaitForSingleObject(tConsumer2.getHandle(), INFINITE);
    WaitForSingleObject(tProducer.getHandle(), INFINITE);

    DeleteCriticalSection(&lock);

    pause();
    FreeConsole();
    return 0;
}

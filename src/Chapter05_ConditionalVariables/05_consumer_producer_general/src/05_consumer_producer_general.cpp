#include <windows.h>
#include <wincrypt.h> // for CryptGenRandom

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>
#include <Rand/random_win32.h>

#define BUFFER_SIZE 128
#define nProducers 3
#define nConsumers 5

volatile i32 buffer[BUFFER_SIZE];
volatile i32 fillIndex = 0;
volatile i32 useIndex = 0;
volatile i32 count = 0;
rand_win32 rng{};

CONDITION_VARIABLE queueNotFull, queueNotEmpty;
CRITICAL_SECTION lock;

volatile b32 stopRequested = false;

void
put(i32 value)
{
    buffer[fillIndex] = value;
    fillIndex = ((fillIndex + 1) % BUFFER_SIZE);
    count++;
}

i32
get()
{
    i32 result = buffer[useIndex];
    useIndex = ((useIndex + 1) % BUFFER_SIZE);
    count--;
    return result;
}

b32
isQueueFull()
{
    b32 result = count >= BUFFER_SIZE;
    return result;
}

_THREAD_PROC(producerThreadProc)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("producer %d began!\n", id);
    while (true)
    {
        Sleep(rng.next() % 200);
        EnterCriticalSection(&lock);

        while(!stopRequested && isQueueFull())
        {
            SleepConditionVariableCS(&queueNotFull, &lock, INFINITE);
        }

        if(stopRequested)
        {
            Logger::LogInfo("producer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        i32 value = rng.next();
        put(value);
        Logger::LogInfo("producer %d filled up the buffer with value: %d\n", id, value);
        WakeConditionVariable(&queueNotEmpty);

        LeaveCriticalSection(&lock);
    }

    Logger::LogInfo("producer %d end!\n", id);
    ExitThread(0);
}

_THREAD_PROC(consumerThreadProc)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("consumer %d began!\n", id);
    while (true)
    {
        Sleep(rng.next() % 500);

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

    Logger::LogInfo("consumer %d end!\n", id);
    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
        i32 CmdShow)
{
    Logger::LogInfoUnformatted("main function start!\n");

    InitializeConditionVariable(&queueNotFull);
    InitializeConditionVariable(&queueNotEmpty);
    InitializeCriticalSection(&lock);

    i32 producerIds[nProducers];
    HANDLE hProducers[nProducers]{};
    DWORD tempId;
    for (i32 i = 0; i < nProducers; ++i)
    {
        producerIds[i] = i;
        hProducers[i] = CreateThread(NULL, 0, producerThreadProc,
                                     (void *)&producerIds[i], 0, &tempId);
    }
    i32 consumerIds[nConsumers];
    HANDLE hConsumers[nConsumers]{};
    for(i32 i = 0; i < nConsumers; ++i)
    {
        consumerIds[i] = i;
        hConsumers[i] = CreateThread(NULL, 0, consumerThreadProc,
                                     (void *)&consumerIds[i], 0, &tempId);
    }

    pause();
    EnterCriticalSection(&lock);
    stopRequested = true;
    LeaveCriticalSection(&lock);

    WaitForMultipleObjects(nProducers, hProducers, TRUE, INFINITE);
    WaitForMultipleObjects(nConsumers, hConsumers, TRUE, INFINITE);

    DeleteCriticalSection(&lock);

    pause();
    FreeConsole();
    return 0;
}

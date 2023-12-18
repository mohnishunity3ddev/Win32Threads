#include <windows.h>
#include <wincrypt.h> // for CryptGenRandom

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>
#include <Rand/random_win32.h>

#define BUFFER_SIZE 16
#define nProducers 4
#define nConsumers 9

volatile i32 buffer[BUFFER_SIZE];
volatile i32 fillIndex = 0;
volatile i32 useIndex = 0;
volatile i32 count = 0;
rand_win32 rng{};

HANDLE hSemaphoreNotFull, hSemaphoreNotEmpty;
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

        // NOTE: The Buffer is not full, the producer should run to put some
        // values in it until it is full. This semaphore is needed to
        // synchronize producer and consumer threads.
        DWORD waitResult = WaitForSingleObject(hSemaphoreNotFull, INFINITE);
        ASSERT(waitResult == WAIT_OBJECT_0);

        // IMPORTANT: NOTE:
        // This lock here is needed. to provide synchronization between two or
        // more producer threads. Since they are writing to the same
        // buffer(shared resource).
        // IMPORTANT: NOTE: What happens if this lock was placed before waiting
        // for the semaphore to get signaled?
        // there will be a deadlock in this case. Let's say there is one
        // consumer c0 thread and one producer p0 thread. c0 acquires the lock,
        // and starts waiting for the producer to fill one up. thus going to
        // sleep WITHOUT UNLOCKING! p0 waits on the NotEmpty semaphore from
        // where it returns signaled since its already signaled at the start.
        // And then before adding the item, it tries to grab the lock which it
        // can't since c0 is already holding it. So consumer grabs the lock and
        // starts waiting on the producer to fill the queue, but producer can't
        // do that since the lock it wants is being held by the consumer. Hence,
        // DEADLOCK.
        EnterCriticalSection(&lock);

        if(stopRequested)
        {
            Logger::LogInfo("producer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        i32 value = rng.next();
        put(value);
        Logger::LogInfo("producer %d filled up the buffer with value: %d; "
                        "items available: %d.\n",
                        id, value, count);

        // NOTE: Signal the NotFull Semaphore and signal to the consumers that
        // the buffer is not empty. So it should be safe to consume out of it.
        b32 signalResult = ReleaseSemaphore(hSemaphoreNotEmpty, 1, NULL);
        ASSERT(signalResult);

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
        Sleep(rng.next() % 100);

        // NOTE: Wait on the NotEmpty to get signaled because if the buffer is
        // not empty, it means there are elements available inside the buffer
        // that the consumer can consume.
        DWORD waitResult = WaitForSingleObject(hSemaphoreNotEmpty, INFINITE);
        ASSERT(waitResult == WAIT_OBJECT_0);

        EnterCriticalSection(&lock);

        if(stopRequested)
        {
            Logger::LogInfo("consumer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        i32 consumedVal = get();
        Logger::LogInfo("consumer %d consumed %d; items left: %d.\n", id,
                        consumedVal, count);

        // NOTE: Signal to the producers that the consumers consumed one element
        // out of the buffer so it is not full, so it should put atleast one
        // value to the buffer by the producer.
        b32 signalResult = ReleaseSemaphore(hSemaphoreNotFull, 1, NULL);
        ASSERT(signalResult);

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

    // NotEmpty semaphore gets signaled by the producer after filling up one
    // item in the buffer. The consumer waits on it before running.
    // We initialize it to zero, so that the first time any consumer thread
    // waits on it, it makes the semaphore count negative and sleeps. This
    // allows the producer to run first.
    hSemaphoreNotEmpty = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);
    // NotFull semaphore gets signaled by the consumer to tell producer that "I
    // consumed one value out of the buffer, so the buffer is not full and so it
    // is now safe to put values inside of it".
    // We initialize it to the value of the buffer size, so that in the
    // beginning the producers are allowed to run to put values inside the
    // buffer so that consumer threads can run.
    hSemaphoreNotFull = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
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

    // NOTE: This should be done in case there are some threads waiting on these
    // semaphores.
    ReleaseSemaphore(hSemaphoreNotEmpty, BUFFER_SIZE, NULL);
    ReleaseSemaphore(hSemaphoreNotFull, BUFFER_SIZE, NULL);

    WaitForMultipleObjects(nProducers, hProducers, TRUE, INFINITE);
    WaitForMultipleObjects(nConsumers, hConsumers, TRUE, INFINITE);

    for (i32 i = 0; i < nProducers; ++i)
    {
        CloseHandle(hProducers[i]);
        hProducers[i] = NULL;
    }
    for (i32 i = 0; i < nConsumers; ++i)
    {
        CloseHandle(hConsumers[i]);
        hConsumers[i] = NULL;
    }

    DeleteCriticalSection(&lock);

    pause();
    FreeConsole();
    return 0;
}

#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>
#include <stdlib.h>

// NOTE: This is simple single buffer producer consumer example. Buffer length is 1.

CONDITION_VARIABLE condVar;
CRITICAL_SECTION lock;
volatile b32 stopRequested = false;
volatile i32 count = 0;
volatile i32 buffer = 0;

// IMPORTANT: NOTE:
// This is to say that instead of the if statement we should use a while loop
// for the condition of whether the queue is empty or not. Let's assume there is
// one producer p0 and two consumers c0 and c1. c0 runs, sees the buffer empty,
// it has nothing to consume, so it sleeps. OS schedules p0 which produces the
// entry in the buffer and wakes up c0. c0 wakes up BUT DOES NOT RUN, IT IS IN
// THE READY STATE at this moment. OS schedules c1, it runs, consumes the
// buffer(buffer is empty now) and releases the lock. The problem is that OS
// could schedule c0 to run from its waking state. since this is an if
// statement, it will not check the count variable here and goes ahead and tries
// to consume from the buffer which it obviously can not since c1 already snuck
// in and consumed it. The way to correct this is to use a while loop here so
// that c1 checks the count var again and seeing that it is still zero, sleeps
// until it is woken up again by the producer.
#define ERROR_USE_WHILE_HERE

// IMPORTANT: NOTE:
// The important thing to note here is that we are only using one condition
// variable for both the consumer and producer threads. We should actually use
// two condition variables one to denote queueNotEmpty and one for queueNotFull.
// producers wait on queueNotFull condition to get signaled and wake all
// consumer threads waiting on queueNotEmpty condition.
// The problem of not using two but one condition(as in this example) is
// explained as follows:
// lets say there are two consumers c0 and c1. One producer p0.
// both c0 and c1 run first until they sleep on queueNotEmpty to get signaled.
// producer goes ahead fills the buffer and signals the condVar here and wakes
// only one of the thread waiting on it. Let's say c0 gets woken up(In ready
// state) but OS has not scheduled it yet. c1 is still sleeping. p0 tries to
// fill up the buffer again in its second iteration of its while loop, sees the
// buffer is full and sleeps on the condVar. c0 runs, it grabs the buffer value
// wakes the thread waiting on the condVar here. Let's say c1 got woken up
// (there is the problem here - producer should have woken up since the buffer
// is empty but instead another consumers got awakened). anywho, c0 continues
// with its second while loop iteration, sees the buffer empty and then sleeps.
// p0 is already sleeping. Here, c1 runs since it is the one that was sleected
// to be woken up, it again sees the buffer as empty and again sleeps. As you
// can see, all three threads are sleeping and nobody will be woken up.
// So, consumers should signal to wake only producer threads and producer should
// signal only consumer threads to wake up. That is why we need two condition
// variables here instead of just the one used here.
#define ERROR_USE_QUEUE_EMPTY_CONDITION_HERE
#define ERROR_USE_QUEUE_FILLED_CONDITION_HERE


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
        EnterCriticalSection(&lock);

#ifdef ERROR_USE_WHILE_HERE
        // NOTE: Buffer is full!
        if (!stopRequested && count == 1)
        {
            // Sleep producer, releasing the lock, and wait for consumer to get
            // something and signal the condition variable after which this
            // thread will resume and re-acquire the lock.
 #ifdef ERROR_USE_QUEUE_FILLED_CONDITION_HERE
            SleepConditionVariableCS(&condVar, &lock, INFINITE);
#endif
        }
#endif

        if (stopRequested)
        {
            Logger::LogInfo("producer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        // NOTE: fill the buffer up with a random number between 0 and 20.
        put(rand() % 20);
        // NOTE: since the producer has put 1 item in the buffer, wake a consumer
        // which was waiting on this buffer to be filled.

#ifdef ERROR_USE_QUEUE_EMPTY_CONDITION_HERE
        // NOTE: wake a consumer thread waiting for this condition var to get
        // signaled and then start itself. Let's say there are 3 threads waiting
        // for this to get signaled. WakeConditionVariable will only wake up one
        // out of those 3 threads, not all of them. If you want to wake all the
        // threads, use WakeAllConditionVariable.
        WakeConditionVariable(&condVar);
#endif

        LeaveCriticalSection(&lock);
    }

    ExitThread(0);
}

_THREAD_PROC(consumerThreadProc)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("Consumer %d began!\n", id);
    while (true)
    {
        EnterCriticalSection(&lock);

        // NOTE: if there is nothing in the buffer to consume, sleep and give
        // time to producer to put some items in the buffer.
#ifdef ERROR_USE_WHILE_HERE
        if (!stopRequested && count == 0)
        {
            // NOTE: Sleep since there is nothing to consume, and leave the
            // critical section(lock).
#ifdef ERROR_USE_QUEUE_EMPTY_CONDITION_HERE
            SleepConditionVariableCS(&condVar, &lock, INFINITE);
#endif
        }
#endif
        if (stopRequested)
        {
            Logger::LogInfo("consumer %d terminated!\n", id);
            LeaveCriticalSection(&lock);
            break;
        }

        i32 consumedVal = get();
        Logger::LogInfo("consumer consumed %d", consumedVal);

#ifdef ERROR_USE_QUEUE_FILLED_CONDITION_HERE
        // NOTE: This one should wake a producer thread telling it the buffer is
        // not empty right now, so fill it.
        WakeConditionVariable(&condVar);
#endif

        LeaveCriticalSection(&lock);
    }

    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("main function\n!");

    InitializeCriticalSection(&lock);
    InitializeConditionVariable(&condVar);

    thread tConsumer1{}, tConsumer2{}, tProducer{};

    tProducer.start(producerThreadProc, (void *)1);
    tConsumer1.start(consumerThreadProc, (void *)1);
    tConsumer2.start(consumerThreadProc, (void *)2);

    pause();
    EnterCriticalSection(&lock);
    stopRequested = true;
    LeaveCriticalSection(&lock);

    WaitForSingleObject(tConsumer1.getHandle(), INFINITE);
    WaitForSingleObject(tConsumer2.getHandle(), INFINITE);
    WaitForSingleObject(tProducer.getHandle(), INFINITE);

    DeleteCriticalSection(&lock);

    FreeConsole();
    return 0;
}

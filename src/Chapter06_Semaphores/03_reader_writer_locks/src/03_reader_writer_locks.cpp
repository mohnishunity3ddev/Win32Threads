#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>
#include <Rand/random_win32.h>

// NOTE: Simple reader_writer example.
// The goal here is that readers acquire readLock, which allows multiple reader
// threads to read concurrently but does not allow writers to write.
// AND only one writer can write to the shared_value.

#define nReaders 15
#define nWriters 4

SRWLOCK srwLock;

b32 dataAvailable;
//
CONDITION_VARIABLE readAllowedCondition;

static volatile b32 stopRequested = false;
static volatile i32 sharedValue = 0;
rand_win32 rng{};

_THREAD_PROC(writerThread)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("writer %d began!\n", id);

    while(true)
    {
        // NOTE: This is the read/write lock, no other thread can have this lock
        // until this writer thread releases it.
        AcquireSRWLockExclusive(&srwLock);

        if (stopRequested)
        {
            Logger::LogInfo("writer thread %d terminated!\n", id);
            ReleaseSRWLockExclusive(&srwLock);
            break;
        }

        ++sharedValue;

        dataAvailable = true;
        WakeAllConditionVariable(&readAllowedCondition);

        Logger::LogInfo("WRITER thread %d writes value %d.\n", id, sharedValue);

        ReleaseSRWLockExclusive(&srwLock);
        Sleep(rng.next() % 1000);
    }

    ExitThread(0);
}

_THREAD_PROC(readerThread)
{
    i32 id = *((i32 *)param);
    while (true)
    {
        // NOTE: This allows multiple readers to run concurrently but does not
        // allow writer threads to take this lock. This ensures that if there
        // are readers reading, no modification can be made with the shared
        // resource by the writer.
        AcquireSRWLockShared(&srwLock);
        if (stopRequested)
        {
            Logger::LogInfo("reader thread %d terminated!\n", id);
            ReleaseSRWLockShared(&srwLock);
            break;
        }

        while(!dataAvailable)
        {
            SleepConditionVariableSRW(&readAllowedCondition, &srwLock, INFINITE, 0);
        }

        Logger::LogInfo("reader thread %d reads value %d\n", id, sharedValue);

        ReleaseSRWLockShared(&srwLock);
        Sleep(rng.next() % 200);
    }

    ExitThread(0);
}

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("inside main!\n");
    InitializeSRWLock(&srwLock);
    InitializeConditionVariable(&readAllowedCondition);
    
    i32 readerIds[nReaders];
    HANDLE hReaders[nReaders]{};
    DWORD tempId;
    for (i32 i = 0; i < nReaders; ++i)
    {
        readerIds[i] = i;
        hReaders[i] = CreateThread(NULL, 0, readerThread, (void *)&readerIds[i], 0, &tempId);
    }

    i32 writerIds[nWriters];
    HANDLE hWriters[nWriters]{};
    for(i32 i = 0; i < nWriters; ++i)
    {
        writerIds[i] = i;
        hWriters[i] = CreateThread(NULL, 0, writerThread, (void *)&writerIds[i], 0, &tempId);
    }

    pause();
    AcquireSRWLockShared(&srwLock);
    stopRequested = true;
    ReleaseSRWLockShared(&srwLock);
    dataAvailable = true;
    WakeAllConditionVariable(&readAllowedCondition);

    WaitForMultipleObjects(nReaders, hReaders, TRUE, INFINITE);
    WaitForMultipleObjects(nWriters, hWriters, TRUE, INFINITE);

    pause();
    FreeConsole();
    return 0;
}

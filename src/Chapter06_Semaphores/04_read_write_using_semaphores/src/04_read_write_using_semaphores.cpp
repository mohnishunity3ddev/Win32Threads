#include <windows.h>

#include <defines.h>
#include <Logger/Win32Logger.h>
#include <String/sh_string.h>
#include <Thread/thread_api.h>
#include <Rand/random_win32.h>

#define nReaders 20
#define nWriters 4

class rwlock
{
  public:
    rwlock()
    {
        lock = CreateSemaphore(NULL, 1, 1, NULL);
        writeLock = CreateSemaphore(NULL, 1, 1, NULL);
        this->numReadersActive = 0;
    }
    ~rwlock()
    {
        CloseHandle(lock);
        CloseHandle(writeLock);
    }

    void
    acquireReadLock()
    {
        ASSERT(WaitForSingleObject(lock, INFINITE) == WAIT_OBJECT_0);
        numReadersActive++;
        if(numReadersActive == 1)
        {
            ASSERT(WaitForSingleObject(writeLock, INFINITE) == WAIT_OBJECT_0);
        }
        ASSERT(ReleaseSemaphore(lock, 1, NULL));
    }

    void
    releaseReadLock()
    {
        ASSERT(WaitForSingleObject(lock, INFINITE) == WAIT_OBJECT_0);
        if(numReadersActive == 1)
        {
            ASSERT(ReleaseSemaphore(writeLock, 1, NULL));
        }
        numReadersActive--;
        ASSERT(ReleaseSemaphore(lock, 1, NULL));
    }

    void
    acquireWriteLock()
    {
        ASSERT(WaitForSingleObject(writeLock, INFINITE) == WAIT_OBJECT_0);
    }

    void
    releaseWriteLock()
    {
        ASSERT(ReleaseSemaphore(writeLock, 1, NULL));
    }

  private:
    HANDLE lock; // binary semaphore for lock.
    HANDLE writeLock; // semaphore used by writers
    i32 numReadersActive;
};

b32 dataAvailable;
HANDLE readAllowedCondition;

static volatile b32 stopRequested = false;
static volatile i32 sharedValue = 0;
rand_win32 rng{};
rwlock rwLock{};

_THREAD_PROC(writerThread)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("writer %d began!\n", id);

    while(true)
    {
        // NOTE: This is the read/write lock, no other thread can have this lock
        // until this writer thread releases it.
        rwLock.acquireWriteLock();

        if (stopRequested)
        {
            Logger::LogInfo("writer thread %d terminated!\n", id);
            rwLock.releaseWriteLock();
            break;
        }

        ++sharedValue;

        if(!dataAvailable)
        {
            dataAvailable = true;
            // NOTE: Wake up all the reader threads waiting on this condition.
            ASSERT(ReleaseSemaphore(readAllowedCondition, nReaders, NULL));
        }

        Logger::LogInfo("---------WRITER thread %d writes value %d.----------------\n", id, sharedValue);

        rwLock.releaseWriteLock();
        // Sleep(1);
    }

    ExitThread(0);
}

_THREAD_PROC(readerThread)
{
    i32 id = *((i32 *)param);
    Logger::LogInfo("reader %d began!\n", id);

    while (true)
    {
        // NOTE: This allows multiple readers to run concurrently but does not
        // allow writer threads to take this lock. This ensures that if there
        // are readers reading, no modification can be made with the shared
        // resource by the writer.
        rwLock.acquireReadLock();

        if (stopRequested)
        {
            Logger::LogInfo("reader thread %d terminated!\n", id);
            rwLock.releaseReadLock();
            break;
        }

        while(!dataAvailable)
        {
            // NOTE: THIS is so that the writer is able to lock.
            rwLock.releaseReadLock();
            ASSERT(WaitForSingleObject(readAllowedCondition, INFINITE) == WAIT_OBJECT_0);
            rwLock.acquireReadLock();
        }

        Logger::LogInfo("reader thread %d reads value %d\n", id, sharedValue);

        rwLock.releaseReadLock();
        Sleep(1);
    }

    ExitThread(0);
}

#include <cstdio>

i32 CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine,
          i32 CmdShow)
{
    Logger::LogInfoUnformatted("inside main!\n");
    readAllowedCondition = CreateSemaphore(NULL, 0, nReaders, NULL);

    i32 readerIds[nReaders];
    HANDLE hReaders[nReaders]{};
    DWORD tempId;
    for (i32 i = 0; i < nReaders; ++i)
    {
        readerIds[i] = i;

        wchar_t threadName[256];
        const wchar_t *format = L"Reader %d";
        swprintf_s(threadName, sizeof(threadName) / sizeof(wchar_t), format, i);

        hReaders[i] = CreateThread(NULL, 0, readerThread, (void *)&readerIds[i], 0, &tempId);
        SetThreadDescription(hReaders[i], threadName);
    }

    Sleep(100);

    i32 writerIds[nWriters];
    HANDLE hWriters[nWriters]{};
    for(i32 i = 0; i < nWriters; ++i)
    {
        writerIds[i] = i;

        wchar_t threadName[256];
        const wchar_t *format = L"Writer %d";
        swprintf_s(threadName, sizeof(threadName) / sizeof(wchar_t), format, i);

        hWriters[i] = CreateThread(NULL, 0, writerThread, (void *)&writerIds[i], 0, &tempId);
        SetThreadDescription(hWriters[i], threadName);
    }

    pause();
    rwLock.acquireWriteLock();
    stopRequested = true;
    rwLock.releaseWriteLock();

    WaitForMultipleObjects(nReaders, hReaders, TRUE, INFINITE);
    WaitForMultipleObjects(nWriters, hWriters, TRUE, INFINITE);

    pause();
    FreeConsole();
    return 0;
}

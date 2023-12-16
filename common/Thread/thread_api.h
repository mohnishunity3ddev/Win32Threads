#if !defined(THREAD_API_H)

#include "defines.h"
#include <windows.h>

#define _THREAD_PROC(name) DWORD WINAPI name(LPVOID param)
typedef _THREAD_PROC(threadProc);

class thread
{
  private:
    HANDLE _handle;
    DWORD _id;

  public:
    thread() : _handle(NULL), _id(-1) {}
    ~thread()
    {
         CloseHandle(_handle);
        _handle = NULL;
    }

    DELETE_MOVE_COPY(thread);

    HANDLE getHandle() { return _handle; }
    void start(threadProc *proc, void *args);
    void wait(DWORD timeout);
};

class mutex
{
  private:
    HANDLE _handle;

  public:
    mutex();
    ~mutex();

    DELETE_MOVE_COPY(mutex);

    void lock();
    void unlock();
    void cleanup();
};

#define THREAD_API_H
#endif // THREAD_API_H
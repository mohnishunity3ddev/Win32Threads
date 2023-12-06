#if !defined(WIN32THREAD_H)

#include "defines.h"

class win32_thread
{
  public:
    win32_thread();
    ~win32_thread() { CloseHandle(ThreadHandle); }
    HANDLE get() { return ThreadHandle; }
    DWORD getID() { return ThreadID; }
  
  private:
    HANDLE ThreadHandle;
    DWORD ThreadID;
};

#define WIN32THREAD_H
#endif // WIN32THREAD_H
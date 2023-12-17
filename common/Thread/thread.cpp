#include "thread_api.h"

void
thread::wait(DWORD timeout)
{
    WaitForSingleObject(_handle, timeout);
}

void
thread::start(threadProc *proc, void *args)
{
    _handle = CreateThread(NULL, 0, proc, args, 0, &_id);
}

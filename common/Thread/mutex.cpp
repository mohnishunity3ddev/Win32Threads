#include "thread_api.h"

void
mutex::cleanup()
{
    unlock();
    CloseHandle(_handle);
    _handle = NULL;
}

mutex::~mutex()
{
    cleanup();
}

void
mutex::lock()
{
    DWORD result = WaitForSingleObject(_handle, INFINITE);
    ASSERT(result == WAIT_OBJECT_0);
}

void
mutex::unlock()
{
    ReleaseMutex(this->_handle);
}

void
mutex::init()
{
    _handle = CreateMutex(NULL, FALSE, NULL);
    ASSERT(_handle != NULL);
}

// content of mutex.cpp goes here

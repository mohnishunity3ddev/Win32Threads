#if !defined(MUTEX_H)

#include "defines.h"
#include <windows.h>

class mutex
{
  private:
    HANDLE _handle;

  public:
    mutex();
    ~mutex();

    mutex(const mutex& other) = delete;
    mutex(mutex &&other) = delete;
    mutex& operator=(const mutex& other) = delete;
    mutex& operator=(mutex&& other) = delete;

    void lock();
    void unlock();
    void cleanup();

};

#define MUTEX_H
#endif // MUTEX_H
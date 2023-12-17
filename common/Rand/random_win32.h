#if !defined(RANDOM_WIN32_H)
#include <windows.h>
#include <wincrypt.h>

#include "defines.h"

class rand_win32
{
  private:
    HCRYPTPROV hCryptProv;
    CRITICAL_SECTION criticalSection;

  public:
    rand_win32();
    ~rand_win32();

    DELETE_MOVE_COPY(rand_win32);

    DWORD next();
};

#define RANDOM_WIN32_H
#endif // RANDOM_WIN32_H
#include "random_win32.h"

rand_win32::rand_win32()
{
    ASSERT(CryptAcquireContext(&hCryptProv, nullptr, nullptr, PROV_RSA_FULL,
                               CRYPT_VERIFYCONTEXT));
    InitializeCriticalSection(&this->criticalSection);
}

rand_win32::~rand_win32()
{
    CryptReleaseContext(hCryptProv, 0); // Release the CSP
    LeaveCriticalSection(&criticalSection);
    DeleteCriticalSection(&criticalSection);
}

DWORD
rand_win32::next()
{
    EnterCriticalSection(&this->criticalSection);

    DWORD randomValue;
    ASSERT(CryptGenRandom(hCryptProv, sizeof(randomValue),
                          (BYTE *)(&randomValue)));

    randomValue %= 65536;
    LeaveCriticalSection(&this->criticalSection);

    return randomValue;
}

#if !defined(DEFINES_H)

#include "windows.h"
#include <stdint.h>

typedef int8_t b8;
typedef int16_t b16;
typedef int32_t b32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef float f32;
typedef double f64;


#include "Console/Win32Console.h"
#include "Logger/Win32Logger.h"

#define OFFSETOF(Type, Member) (size_t)&(((Type *)0)->Member)

#define ASSERT(Expression)                                                     \
    if (!(Expression))                                                         \
    {                                                                          \
        *((int volatile *)0) = 0;                                              \
    }
#define INVALID_DEFAULT                                                        \
    default:                                                                   \
    {                                                                          \
        ASSERT(!"Invalid Default Case");                                       \
    }
#define INVALID_CODEPATH ASSERT(!"Invalid Code Path")

#define pause()                                                                \
    {                                                                          \
        Logger::LogUnformatted("Press Enter to continue...\n");           \
        HANDLE Console = Console::GetInputConsole();                           \
        FlushConsoleInputBuffer(Console);                                      \
                                                                               \
        if (Console != INVALID_HANDLE_VALUE)                                   \
        {                                                                      \
            INPUT_RECORD Input;                                                \
            DWORD BytesRead;                                                   \
                                                                               \
            while (1)                                                          \
            {                                                                  \
                auto Result = ReadConsole(                                     \
                    Console, &Input, sizeof(INPUT_RECORD), &BytesRead, NULL);  \
                if (Result)                                                    \
                {                                                              \
                    break;                                                     \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }

struct thread_handle
{
    HANDLE handle;
    DWORD id;
    void *arg;
};

#define CREATE_THREAD(Handle, Func)                                            \
    {                                                                          \
        thread_handle *pHandle = &Handle;                                      \
        pHandle->handle = CreateThread(                                        \
            NULL, 0, Func, (void *)pHandle->arg, 0, &pHandle->id);         \
                                                                               \
        if (pHandle->handle == NULL)                                           \
        {                                                                      \
            Logger::LogErrorUnformatted("Could not create the thread!");  \
        }                                                                      \
    }

#define TCreate(Handle, ThreadProc)                                            \
    thread_handle Handle = {};                                                 \
    {                                                                          \
        Handle.arg = (void *)#Handle;                                          \
        CREATE_THREAD(Handle, ThreadProc);                                     \
    }

#define TFree(Thread)                                                          \
    WaitForSingleObject(Thread.handle, INFINITE);                              \
    CloseHandle(Thread.handle)

#define DEFINES_H
#endif // DEFINES_H
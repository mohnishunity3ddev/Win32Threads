#if !defined(DEFINES_H)

#include "windows.h"
#include <stdint.h>
#include <stdio.h>

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

#define pause()                                                                \
    {                                                                          \
        Win32Logger::LogUnformatted("Press Enter to continue...\n");           \
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

#define DEFINES_H
#endif // DEFINES_H
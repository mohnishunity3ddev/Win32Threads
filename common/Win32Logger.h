#if !defined(WIN32LOGGER_H)

#include "defines.h"
#include <Windows.h>
#include <stdio.h>

enum LogType
{
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    LogType_Fatal,
    LogType_Error,
    LogType_Warn,
    LogType_Info,
    LogType_Debug,
    LogType_Trace,
    LogType_ValidationLayerInfo,
    LogType_DebugReportCallbackInfo,

    LogType_Normal,
    
    LogType_MaxCount
};

void
OutputDebugStringColor(LogType LogType, const char *message)
{
    static COLORREF Colors[] = {RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(242, 188, 47),
                                RGB(40, 130, 255), RGB(128, 128, 128), RGB(96, 96, 96)};
    
    if (LogType != LogType::LogType_Normal)
    {
        COLORREF Color = Colors[4];
        switch(LogType)
        {
            case LogType_DebugReportCallbackInfo:
            {
                Color = Colors[6];
            }
            break;
            case LogType_ValidationLayerInfo:
            {
                Color = Colors[5];
            } break;
            case LogType_Fatal:
            case LogType_Error:
            {
                Color = Colors[0];
            } break;
            case LogType_Warn:
            {
                Color = Colors[3];
            } break;
            case LogType_Info:
            case LogType_Debug:
            case LogType_Trace:
            default:
            {
            } break;
        }
        
        char buffer[8192];
        sprintf_s(buffer, sizeof(buffer), "\x1B[38;2;%d;%d;%dm%s\x1B[0m", GetRValue(Color),
                GetGValue(Color), GetBValue(Color), message);
        OutputDebugStringA(buffer);
    }
    else
    {
        OutputDebugStringA(message);
    }
}

void
OutputToConsole(LogType LogType, const char *Message)
{
    HANDLE OutputConsole = Console::GetOutputConsole();
    
    static u8 Levels[] = {64, 4, 6, 2, 1, 8, 8, 8};
    if(LogType != LogType::LogType_Normal)
    {
        SetConsoleTextAttribute(OutputConsole, Levels[LogType]);
    }
    else
    {
        // Set the console text attribute to white text on a black background
        SetConsoleTextAttribute(
            OutputConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    
    OutputDebugStringColor(LogType, Message);
    
    u64 Length = strlen(Message);
    LPDWORD NumberWritten = 0;
    WriteConsoleA(OutputConsole, Message, (DWORD)Length, NumberWritten, 0);
}

void
Log_(LogType Type, const char *Format, va_list VarArgs)
{
    char Buffer[4096];
    
    i32 Length = vsnprintf(Buffer, ARRAYSIZE(Buffer), Format, VarArgs);
    
    if (Length > 0 && Length < ARRAYSIZE(Buffer))
    {
        OutputToConsole(Type, Buffer);
    }
}

#define LOG_FORMAT(Type, Format)                                               \
    va_list VarArgs;                                                           \
    va_start(VarArgs, Format);                                                 \
    Log_(Type, Format, VarArgs);                                               \
    va_end(VarArgs);
void
LogInfo(const char *Format, ...)
{
    LOG_FORMAT(LogType_Info, Format);
}
void
LogDebug(const char *Format, ...)
{
    LOG_FORMAT(LogType_Debug, Format);
}
void
LogWarn(const char *Format, ...)
{
    LOG_FORMAT(LogType_Warn, Format);
}
void
LogError(const char *Format, ...)
{
    LOG_FORMAT(LogType_Error, Format);
}
void
LogFatal(const char *Format, ...)
{
    LOG_FORMAT(LogType_Fatal, Format);
}
void
LogTrace(const char *Format, ...)
{
    LOG_FORMAT(LogType_Trace, Format);
}
void
Log(const char *Format, ...)
{
    LOG_FORMAT(LogType_Normal, Format);
}

void
LogInfoUnformatted(const char *Message)
{
    OutputToConsole(LogType_Info, Message);
}
void
LogDebugUnformatted(const char *Message)
{
    OutputToConsole(LogType_Debug, Message);
}
void
LogWarnUnformatted(const char *Message)
{
    OutputToConsole(LogType_Warn, Message);
}
void
LogErrorUnformatted(const char *Message)
{
    OutputToConsole(LogType_Error, Message);
}
void
LogFatalUnformatted(const char *Message)
{
    OutputToConsole(LogType_Fatal, Message);
}
void
LogTraceUnformatted(const char *Message)
{
    OutputToConsole(LogType_Trace, Message);
}
void
LogUnformatted(const char *Message)
{
    OutputToConsole(LogType_Normal, Message);
}

#define WIN32LOGGER_H
#endif // WIN32LOGGER_H
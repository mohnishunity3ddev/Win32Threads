#include "Win32Logger.h"

#include <windows.h>
#include "defines.h"
#include <stdio.h>

void
Win32Logger::OutputDebugStringColor(LogType LogType, const char *message)
{
    static COLORREF Colors[] = {RGB(255, 0, 0),    RGB(0, 255, 0),
                                RGB(0, 0, 255),    RGB(242, 188, 47),
                                RGB(40, 130, 255), RGB(128, 128, 128),
                                RGB(96, 96, 96)};
    
    if (LogType != LogType::LogType_Normal)
    {
        COLORREF Color = Colors[4];
        switch (LogType)
        {
        case LogType_DebugReportCallbackInfo:
        {
            Color = Colors[6];
        }
        break;
        case LogType_ValidationLayerInfo:
        {
            Color = Colors[5];
        }
        break;
        case LogType_Fatal:
        case LogType_Error:
        {
            Color = Colors[0];
        }
        break;
        case LogType_Warn:
        {
            Color = Colors[3];
        }
        break;
        case LogType_Info:
        case LogType_Debug:
        case LogType_Trace:
        default:
        {
        }
        break;
        }
        
        char buffer[8192];
        sprintf_s(buffer, sizeof(buffer), "\x1B[38;2;%d;%d;%dm%s\x1B[0m",
                  GetRValue(Color), GetGValue(Color), GetBValue(Color),
                  message);
        OutputDebugStringA(buffer);
    }
    else
    {
        OutputDebugStringA(message);
    }
}

void
Win32Logger::OutputToConsole(LogType LogType, const char *Message)
{
    HANDLE OutputConsole = Console::GetOutputConsole();
    
    static u8 Levels[] = {64, 4, 6, 2, 1, 8, 8, 8};
    if (LogType != LogType::LogType_Normal)
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
Win32Logger::Log_(LogType Type, const char *Format, va_list VarArgs)
{
    char Buffer[4096];
    
    i32 Length = vsnprintf(Buffer, ARRAYSIZE(Buffer), Format, VarArgs);
    
    if (Length > 0 && Length < ARRAYSIZE(Buffer))
    {
        OutputToConsole(Type, Buffer);
    }
}

void
Win32Logger::LogInfo(const char *Format, ...)
{
    LOG_FORMAT(LogType_Info, Format);
}
void
Win32Logger::LogDebug(const char *Format, ...)
{
    LOG_FORMAT(LogType_Debug, Format);
}
void
Win32Logger::LogWarn(const char *Format, ...)
{
    LOG_FORMAT(LogType_Warn, Format);
}
void
Win32Logger::LogError(const char *Format, ...)
{
    LOG_FORMAT(LogType_Error, Format);
}
void
Win32Logger::LogFatal(const char *Format, ...)
{
    LOG_FORMAT(LogType_Fatal, Format);
}
void
Win32Logger::LogTrace(const char *Format, ...)
{
    LOG_FORMAT(LogType_Trace, Format);
}
void
Win32Logger::Log(const char *Format, ...)
{
    LOG_FORMAT(LogType_Normal, Format);
}

void
Win32Logger::LogInfoUnformatted(const char *Message)
{
    OutputToConsole(LogType_Info, Message);
}
void
Win32Logger::LogDebugUnformatted(const char *Message)
{
    OutputToConsole(LogType_Debug, Message);
}
void
Win32Logger::LogWarnUnformatted(const char *Message)
{
    OutputToConsole(LogType_Warn, Message);
}
void
Win32Logger::LogErrorUnformatted(const char *Message)
{
    OutputToConsole(LogType_Error, Message);
}
void
Win32Logger::LogFatalUnformatted(const char *Message)
{
    OutputToConsole(LogType_Fatal, Message);
}
void
Win32Logger::LogTraceUnformatted(const char *Message)
{
    OutputToConsole(LogType_Trace, Message);
}
void
Win32Logger::LogUnformatted(const char *Message)
{
    OutputToConsole(LogType_Normal, Message);
}

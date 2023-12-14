#if !defined(WIN32LOGGER_H)

#include <stdarg.h>

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

class Win32Logger
{
  public:
    static void OutputDebugStringColor(LogType LogType, const char *message);
    static void OutputToConsole(LogType LogType, const char *Message);
    static void Log_(LogType Type, const char *Format, va_list VarArgs);

    static void LogInfo(const char *Format, ...);
    static void LogDebug(const char *Format, ...);
    static void LogWarn(const char *Format, ...);
    static void LogError(const char *Format, ...);
    static void LogFatal(const char *Format, ...);
    static void LogTrace(const char *Format, ...);
    static void Log(const char *Format, ...);
    static void LogInfoUnformatted(const char *Message);
    static void LogDebugUnformatted(const char *Message);
    static void LogWarnUnformatted(const char *Message);
    static void LogErrorUnformatted(const char *Message);
    static void LogFatalUnformatted(const char *Message);
    static void LogTraceUnformatted(const char *Message);
    static void LogUnformatted(const char *Message);
};

#define LOG_FORMAT(Type, Format)                                               \
    va_list VarArgs;                                                           \
    va_start(VarArgs, Format);                                                 \
    Log_(Type, Format, VarArgs);                                               \
    va_end(VarArgs)

#define WIN32LOGGER_H
#endif // WIN32LOGGER_H
#include <windows.h>

#include <defines.h>
#include <Logger/win32Logger.h>

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
         i32 CmdShow)
{
    LogInfoUnformatted("Info\n");
    LogDebugUnformatted("Debug\n");
    LogWarnUnformatted("Warn\n");
    LogErrorUnformatted("Error\n");
    LogFatalUnformatted("Fatal\n");
    LogTraceUnformatted("Trace\n");
    LogUnformatted("Normal\n");
    LogTraceUnformatted("Trace\n");
    LogTraceUnformatted("Trace\n");
    LogTraceUnformatted("Mani\n");
    LogTraceUnformatted("Mani2\n");
    
    OutputDebugStringA("Hello from the second project!\n");
    
    FreeConsole();
    return 0;
}

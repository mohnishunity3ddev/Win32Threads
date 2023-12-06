#include <windows.h>

#include <defines.h>
#include <Logger/win32Logger.h>

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
          i32 CmdShow)
{
    LogInfoUnformatted("Hello, World!");
    FreeConsole();
    return 0;
}

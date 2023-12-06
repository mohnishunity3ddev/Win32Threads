#include <windows.h>

#include <defines.h>
#include <logger.h>

void
CreateThread()
{

}

i32
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
          i32 CmdShow)
{
    LogInfoUnformatted("Hello, World!\n");
    
    CreateThread();
    
    pause();
    Console::FreeConsoles();
    return 0;
}

#if !defined(CONSOLE_H)

#include "windows.h"


class Console
{
  public:
    static HANDLE GetOutputConsole();
    static HANDLE GetInputConsole();
    
    static void
    FreeConsoles()
    {
        FreeConsole();
    }
  
  private:
    static HANDLE OutputConsole;
    static HANDLE InputConsole;
};

#define CONSOLE_H
#endif // CONSOLE_H
#include "Console.h"

HANDLE Console::InputConsole = NULL;
HANDLE Console::OutputConsole = NULL;

HANDLE
Console::GetOutputConsole()
{
    if (OutputConsole == NULL)
    {
        OutputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (OutputConsole == NULL)
        {
            AllocConsole();
            OutputConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }
    
    return OutputConsole;
}

HANDLE
Console::GetInputConsole()
{
    if (InputConsole == NULL)
    {
        InputConsole = GetStdHandle(STD_INPUT_HANDLE);
        if (InputConsole == NULL)
        {
            AllocConsole();
            InputConsole = GetStdHandle(STD_INPUT_HANDLE);
        }
    }
    
    return InputConsole;
}

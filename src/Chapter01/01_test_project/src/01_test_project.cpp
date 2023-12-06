#include <iostream>
#include <test_class/test.h>
#include <Windows.h>

int WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine,
         int CmdShow)
{
    OutputDebugStringA("Hello, World!\n");
    OutputDebugStringA("Hello, World AGAIN!\n");
    OutputDebugStringA("Only works in vsStudio for now! :(\n");
    int x = 0;
}
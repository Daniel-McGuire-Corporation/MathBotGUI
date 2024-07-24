#include <windows.h>
#include <cwchar>         // For _snwprintf
#include <stdexcept>     // For std::exception
#include <string>        // For std::wstring
#include <locale>
#include <codecvt>       // For string conversion

#include "MathBotCLI.h"

// Convert std::string to std::wstring
std::wstring s2ws(const std::string& s)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(s);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            CreateWindowExA(
                0, "BUTTON", "Calculate", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                10, 10, 100, 30, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
            );
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) // Button ID
            {
                try
                {
                    double result = calc2(5.0, "+", 3.0);
                    char resultStr[256];
                    snprintf(resultStr, sizeof(resultStr), "5 + 3 = %f", result);
                    MessageBoxA(hwnd, resultStr, "Result", MB_OK);
                }
                catch (const std::exception &ex)
                {
                    MessageBoxA(hwnd, ex.what(), "Error", MB_OK | MB_ICONERROR);
                }
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance; // Suppress unused parameter warning
    (void)lpCmdLine;     // Suppress unused parameter warning

    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0, CLASS_NAME, "Sample Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

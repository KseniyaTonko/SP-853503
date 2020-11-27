#include "windows.h"
#include "windowsx.h"
#include "resource.h"

#define WM_LBUTTONDOWN    0x0201

bool draw = false;
int x = 0;
int y = 150;
HCURSOR hCursor;

    WNDCLASS wc = { };
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";


    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = (HCURSOR)LoadImage(NULL, L"cat.ani", IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCEW(IDI_ICON1));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Nice text",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    HMENU hMenu = CreateMenu();
    HMENU hPopMenu = CreatePopupMenu();

    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hPopMenu, L"Menu");
    AppendMenu(hPopMenu, MF_STRING, 1, L"Start");
    AppendMenu(hPopMenu, MF_STRING, 2, L"Stop");
    SetMenu(hwnd, hMenu);
    SetMenu(hwnd, hPopMenu);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {/*
    case WM_SETCURSOR:
        SetCursor(wc.hCursor);
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(wc.hCursor);
            return TRUE;
        }
        break;*/
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case 1:
        {
            draw = true;
            SetTimer(hwnd,             // хэндл главного окна
                1,            // идентификатор таймера
                2,                 // интервал
                (TIMERPROC)NULL);     // процедуры таймера нет
            break;
        }
        case 2:
            draw = false;
            KillTimer(hwnd, 1);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_TIMER: {
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if (draw) {
            x = (x + 1) % 600;
            TextOut(hdc, x, y, L"Hello, I'm text!", strlen("Hello, I'm text!"));
        }

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
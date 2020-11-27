#include "windows.h"
#include "windowsx.h"
#include <iostream>
#include <list>
#include "resource.h"

#define WM_LBUTTONDOWN    0x0201

#define ID_LISTBOX1 101
#define ID_LISTBOX2 102
#define ID_BUTTON_ADD 201
#define ID_BUTTON_CLEAR 202
#define ID_BUTTON_TORIGHT 203
#define ID_BUTTON_DELETE 204
#define ID_EDIT 301

HWND hListbox1, hListbox2, hEdit;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = (HCURSOR)LoadImage(NULL, L"cursor.ani", IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCEW(IDI_ICON5));

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"LISTBOXES",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 400,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    hListbox1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"listbox", L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL,
        40, 40, 150, 200, hwnd, (HMENU)ID_LISTBOX1, 0, 0);
    hListbox2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"listbox", L"",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL,
        250, 40, 150, 200, hwnd, (HMENU)ID_LISTBOX2, 0, 0);
    HWND hButton1 = CreateWindow(L"button", L"Add",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        0, 0, 60, 30, hwnd, (HMENU)ID_BUTTON_ADD, hInstance, NULL);
    HWND hButton2 = CreateWindow(L"button", L"Crear",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        60, 0, 60, 30, hwnd, (HMENU)ID_BUTTON_CLEAR, hInstance, NULL);
    HWND hButton3 = CreateWindow(L"button", L"ToRight",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 0, 60, 30, hwnd, (HMENU)ID_BUTTON_TORIGHT, hInstance, NULL);
    HWND hButton4 = CreateWindow(L"button", L"Delete",
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        180, 0, 60, 30, hwnd, (HMENU)ID_BUTTON_DELETE, hInstance, NULL);
    hEdit = CreateWindow(L"edit", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
        40, 250, 360, 100, hwnd, (HMENU)ID_EDIT, hInstance, NULL);

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
    {
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_EDIT:
        {
            InvalidateRect(hEdit, NULL, TRUE);
            return 0;
        }
        case ID_BUTTON_ADD: {
            TCHAR str[15];
            GetWindowText(hEdit, str, 15);
            if (SendMessage(hListbox1, LB_FINDSTRINGEXACT, 0, (LPARAM)str) == -1) {
                SendMessage(hListbox1, LB_ADDSTRING, 0, (LPARAM)str);
                InvalidateRect(hListbox1, NULL, TRUE);
            }
            SendMessage(hEdit, WM_SETTEXT, 0, LPARAM(""));
            InvalidateRect(hEdit, NULL, TRUE);
            return 0;
        }
        case ID_BUTTON_CLEAR: {
            SendMessage(hListbox1, LB_RESETCONTENT, 0, 0);
            SendMessage(hListbox2, LB_RESETCONTENT, 0, 0);
            InvalidateRect(hListbox1, NULL, TRUE);
            InvalidateRect(hListbox2, NULL, TRUE);
            return 0;
        }
        case ID_BUTTON_TORIGHT: {
            int count = SendMessage(hListbox1, LB_GETSELCOUNT, 0, 0);
            int* selItems = new int[count];
            SendMessage(hListbox1, LB_GETSELITEMS, (WPARAM)count, (LPARAM)selItems);
            TCHAR str[15];
            for (int i = 0; i < count; i++)
            {
                SendMessage(hListbox1, LB_GETTEXT, selItems[i], (LPARAM)str);
                if (SendMessage(hListbox2, LB_FINDSTRINGEXACT, 0, (LPARAM)str) == -1) {
                    SendMessage(hListbox2, LB_ADDSTRING, 0, (LPARAM)str);
                }
            }
            InvalidateRect(hListbox2, NULL, TRUE);
            return 0;
        }
        case ID_BUTTON_DELETE: {
            int count = SendMessage(hListbox1, LB_GETSELCOUNT, 0, 0);
            int* selItems = new int[count];
            SendMessage(hListbox1, LB_GETSELITEMS, (WPARAM)count, (LPARAM)selItems);
            TCHAR str[15];
            for (int i = count - 1; i >= 0; i--)
            {
                SendMessage(hListbox1, LB_GETTEXT, selItems[i], (LPARAM)str);
                int index;
                SendMessage(hListbox1, LB_DELETESTRING, selItems[i], 0);
                index = SendMessage(hListbox2, LB_FINDSTRINGEXACT, 0, (LPARAM)str);
                if (index > -1) {
                    SendMessage(hListbox2, LB_DELETESTRING, index, 0);
                }

            }
            count = SendMessage(hListbox2, LB_GETSELCOUNT, 0, 0);
            selItems = new int[count];
            SendMessage(hListbox2, LB_GETSELITEMS, (WPARAM)count, (LPARAM)selItems);
            for (int i = count - 1; i >= 0; i--)
            {
                SendMessage(hListbox2, LB_GETTEXT, selItems[i], (LPARAM)str);
                int index;
                SendMessage(hListbox2, LB_DELETESTRING, selItems[i], 0);
                index = SendMessage(hListbox1, LB_FINDSTRINGEXACT, 0, (LPARAM)str);
                if (index > -1) {
                    SendMessage(hListbox1, LB_DELETESTRING, index, 0);
                }

            }
            InvalidateRect(hListbox2, NULL, TRUE);
            InvalidateRect(hListbox2, NULL, TRUE);
            return 0;
        }
        default:
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
        }
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
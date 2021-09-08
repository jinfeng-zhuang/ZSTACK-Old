#include <Windows.h>
#include <zstack.h>

#define LOG_APPEND      (WM_USER + 1)

static HWND editHwnd;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance;
    RECT client_rect;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &client_rect);
        editHwnd = CreateWindowEx(
            0,
            TEXT("EDIT"),
            NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            client_rect.left, client_rect.top, client_rect.right - client_rect.left, client_rect.bottom - client_rect.top,
            hWnd,
            NULL,
            hInstance,
            NULL);
        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam))
        {
        case EN_MAXTEXT:
            if (lParam == (LONG)editHwnd) {
                printf("reach the limit\n");
                SetWindowText(editHwnd, L"");
            }
            break;
        }
        break;
    case LOG_APPEND:
        SendMessage(editHwnd, EM_SETSEL, 0, -1);
        SendMessage(editHwnd, EM_SETSEL, -1, -1);
        SendMessage(editHwnd, EM_REPLACESEL, 0, wParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int LogBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("LogBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

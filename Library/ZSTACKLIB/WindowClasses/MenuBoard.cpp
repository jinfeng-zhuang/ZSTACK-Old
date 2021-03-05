#include <Windows.h>
#include <commctrl.h>
#include <zstack.h>

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        //CreateWindow(L"static", L"XXX:", WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT, 0, 0, 100, 100, hWnd, NULL, hInstance, 0);
        //CreateWindow(L"button", L"XXX", WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTOCHECKBOX, 100, 0, 100, 100, hWnd, NULL, hInstance, 0);
        //CreateWindowEx(0, WC_TABCONTROL, 0, TCS_FIXEDWIDTH | WS_CHILD | WS_VISIBLE, 0, 0, 100, 100, hWnd, NULL, hInstance, 0);
        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int MenuBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("MenuBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

//
// 参考资料
// * https://docs.microsoft.com/en-us/windows/win32/controls/create-a-simple-list-box
//

#include <Windows.h>
#include <zstack.h>

HWND listbox;

struct database {
    wchar_t text[32];
} data[] = {
    {TEXT("清风")},
    {TEXT("明月") },
};

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance;
    int index;
    int i;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        listbox = CreateWindow(TEXT("LISTBOX"), NULL,
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_STANDARD | LBS_HASSTRINGS | LBS_SORT,
            10, 10, 100, 100, hWnd, NULL, hInstance, NULL);

        for (i = 0; i < ARRAYSIZE(data); i++) {
            index = SendMessage(listbox, LB_ADDSTRING, 0, (LPARAM)data[i].text);
            // LB_SETITEMDATA, UUID
        }
        SetFocus(listbox);
        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        // invalid command return -1, not valid if use LBS_SORT
        index = SendMessage(listbox, LB_GETCURSEL, 0, 0);
        // LB_GETITEMDATA, UUID
        printf("index %d\n", index);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int ListBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("ListBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

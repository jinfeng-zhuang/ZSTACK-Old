#include <Windows.h>
#include <commctrl.h>

static HWND ctrl;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT client;
	int evt;
	int sel;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &client);

	switch (uMsg)
	{
	case WM_CREATE:
		ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), NULL,
			WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_HASSTRINGS,
			client.left, client.top, client.right, client.bottom,
			hWnd, NULL, NULL, NULL);

		SendMessage(ctrl, LB_ADDSTRING, 0, (LPARAM)"AAAA");
		SendMessage(ctrl, LB_INSERTSTRING, 1, (LPARAM)"Inserted");
		SendMessage(ctrl, LB_ADDSTRING, 0, (LPARAM)"BBBB");
		SendMessage(ctrl, LB_ADDSTRING, 0, (LPARAM)"CCCC");
		SendMessage(ctrl, LB_ADDSTRING, 0, (LPARAM)"DDDD");

		SendMessage(ctrl, LB_SETCURSEL, 2, 0);
		break;
	case WM_SIZE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		evt = HIWORD(wParam);
		switch (evt) {
		case CBN_SELCHANGE:
			sel = SendMessage(ctrl, LB_GETCURSEL, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Init_ListBoxDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("ListBoxDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

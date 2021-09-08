#include <Windows.h>
#include <commctrl.h>
#include <string.h>

static HWND ctrl;
static HWND staticCtrl;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT client;
	int pos;
	char tmp[32];

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &client);

	switch (uMsg)
	{
	case WM_CREATE:
		staticCtrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), NULL,
			WS_CHILD | WS_VISIBLE,
			client.left, client.top, client.right, client.top + 20,
			hWnd, NULL, NULL, NULL);

		ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("msctls_trackbar32"), NULL,
			WS_CHILD | WS_VISIBLE,
			client.left, client.top + 40, client.right, client.bottom,
			hWnd, NULL, NULL, NULL);

		SendMessage(ctrl, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(ctrl, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)50); // 50%

		SendMessage(staticCtrl, WM_SETTEXT, 0, "0%");
		break;
	case WM_SIZE:
		break;
	case WM_HSCROLL:
		pos = SendMessage(ctrl, TBM_GETPOS, 0, 0);
		snprintf(tmp, sizeof(tmp), "%d%%", pos);
		SendMessage(staticCtrl, WM_SETTEXT, 0, tmp);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
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

int Init_SliderDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("SliderDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

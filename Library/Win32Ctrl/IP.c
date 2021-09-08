#include <Windows.h>
#include <commctrl.h>
#include <stdio.h>

static HWND ctrl;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT rc;
	unsigned int ip;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &rc);

	switch (uMsg)
	{
	case WM_CREATE:
		ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("SysIPAddress32"), NULL,
			WS_CHILD | WS_VISIBLE,
			rc.left, rc.top, rc.right, rc.bottom,
			hWnd, NULL, NULL, NULL);

		SendMessage(ctrl, IPM_SETADDRESS, 0, (LPARAM)MAKEIPADDRESS(127,0,0,1));
		break;
	case WM_SIZE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		break;
	case WM_NOTIFY:
		if (IPN_FIELDCHANGED == ((LPNMHDR)lParam)->code) {
			SendMessage(ctrl, IPM_GETADDRESS, 0, (LPARAM)&ip);
			printf("%#x\n", ip);
			fflush(stdout);
		}
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

int Init_IPDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("IPDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

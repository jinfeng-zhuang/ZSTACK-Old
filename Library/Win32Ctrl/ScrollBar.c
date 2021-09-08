#include <Windows.h>
#include <commctrl.h>

static HWND ctrl;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT client;
	int pos = 1;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &client);

	switch (uMsg)
	{
	case WM_CREATE:
		ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("SCROLLBAR"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | SBS_HORZ,
			//10, 10, 200, 10+20,
			client.left, client.top, client.right, client.bottom,
			hWnd, NULL, NULL, NULL);

		SetScrollRange(ctrl, SB_HORZ, 0, 100, FALSE);
		SetScrollPos(ctrl, SB_HORZ, 50, TRUE);
		break;

	case WM_SIZE:
		break;

	case WM_HSCROLL:
		pos = GetScrollPos(ctrl, SB_HORZ);

		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			pos--;
			break;
		case SB_LINEDOWN:
			pos++;
			break;
		case SB_PAGEUP:
			pos -= 10;
			break;
		case SB_PAGEDOWN:
			pos += 10;
			break;
		case SB_THUMBTRACK:
			pos = HIWORD(wParam);
			break;
		}

		printf("pos = %d\n", pos);

		SetScrollPos(ctrl, SB_HORZ, pos, TRUE);
		InvalidateRect(ctrl, NULL, FALSE);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//LineTo(hdc, client.right + 1000, client.bottom);
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

int Init_ScrollBarDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("ScrollBarDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

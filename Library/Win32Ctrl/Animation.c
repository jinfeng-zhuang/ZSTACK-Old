






//================================================
// Limited Class, REMOVE it, don't use it
//================================================

#include <Windows.h>
#include <commctrl.h>

static HWND ctrl;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT client;
	POINT pt;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &client);

	switch (uMsg)
	{
	case WM_CREATE:
		Animate_Create(hWnd, 0, WS_CHILD | WS_VISIBLE, hInstance);

#if 1
		pt.x = client.left;
		pt.y = client.bottom;
		ScreenToClient(hWnd, &pt);

		SetWindowPos(ctrl, 0, pt.x, pt.y + 20,
			240, 200,
			SWP_NOZORDER | SWP_DRAWFRAME);
#else

		SetWindowPos(ctrl, 0, 0, 0,
			240, 200,
			SWP_NOZORDER | SWP_DRAWFRAME);
#endif
		Animate_Open(ctrl, "C:\\Users\\jinfengz\\tmp\\240x200.avi");
		//Animate_Play(ctrl, 0, -1, -1);
		//ShowWindow(ctrl, SW_SHOW);
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
		break;
	case WM_DESTROY:
		Animate_Close(ctrl);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Init_AnimationDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("AnimationDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

#include <Windows.h>
#include <commctrl.h>

static HWND ctrl;
static HBITMAP hBitmap = NULL;

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
		ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("STATIC"), NULL,
			WS_CHILD | WS_VISIBLE | SS_BITMAP,
			client.left, client.top, client.right, client.bottom,
			hWnd, NULL, NULL, NULL);

		hBitmap = LoadImage(hInstance, "C:\\Users\\jinfengz\\test.bmp", IMAGE_BITMAP, client.right - client.left, client.bottom - client.top, LR_LOADFROMFILE);
		if (NULL == hBitmap) {
			MessageBox(hWnd, "BMP file not found", "", MB_OK);
		}
		else {
			SendMessage(ctrl, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
		}
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
		if (NULL != hBitmap) {
			DeleteObject((HBITMAP)hBitmap);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Init_PictureCtrlDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("PictureCtrlDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

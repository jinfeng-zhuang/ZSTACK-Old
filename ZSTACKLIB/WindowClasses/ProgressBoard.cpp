#include <Windows.h>
#include <zstack.h>

#define RBM_UPDATE   (WM_USER + 0)

static int progress;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	int i;
    HBRUSH hbrush;
    HBRUSH hbrush_origin;
    RECT client_rect;

	printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		break;
    case WM_PAINT:
        GetClientRect(hWnd, &client_rect);

        hdc = BeginPaint(hWnd, &ps);

        Rectangle(hdc, 0, 0, client_rect.right, client_rect.bottom);

        hbrush = CreateSolidBrush(RGB(0, 0, 255));
        hbrush_origin = (HBRUSH)SelectObject(hdc, hbrush);

        Rectangle(hdc, 0, 0, progress, client_rect.bottom);

        SelectObject(hdc, hbrush_origin);
        EndPaint(hWnd, &ps);
        break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case RBM_UPDATE:
        GetClientRect(hWnd, &client_rect);

        progress = LOWORD(lParam);
        if (progress < 0)
            progress = 0;
        if (progress > 100)
            progress = 100;
        progress = (float)progress * (client_rect.right - client_rect.left) / 100;
        
        InvalidateRect(hWnd, &client_rect, TRUE);
        break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int ProgressBoardRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("ProgressBoard");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

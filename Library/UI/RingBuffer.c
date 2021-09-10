#include <Windows.h>
#include <zstack/zstack.h>

static unsigned int progress_wp;
static unsigned int progress_rp;
static struct ringbuffer ring;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
    HBRUSH hbrush;
    HBRUSH hbrush_origin;
    RECT client_rect;

	//printf("Window Message: 0x%04X\n", uMsg);

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

        if (progress_wp >= progress_rp)
            Rectangle(hdc, progress_rp, 0, progress_wp, client_rect.bottom);
        else {
            Rectangle(hdc, progress_rp, 0, client_rect.right, client_rect.bottom);
            Rectangle(hdc, 0, 0, progress_wp, client_rect.bottom);
        }

        SelectObject(hdc, hbrush_origin);
        EndPaint(hWnd, &ps);
        break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
    case WM_USER:
		if (NULL == lParam)
			break;

        memcpy(&ring, lParam, sizeof(struct ringbuffer));

		if ((ring.wp >= ring.length) || (ring.rp >= ring.length))
			break;

        GetClientRect(hWnd, &client_rect);

        progress_wp = (float)ring.wp * 100 / ring.length;
        progress_rp = (float)ring.rp * 100 / ring.length;

        progress_wp = (float)progress_wp * (client_rect.right - client_rect.left) / 100;
        progress_rp = (float)progress_rp * (client_rect.right - client_rect.left) / 100;
        
        InvalidateRect(hWnd, &client_rect, TRUE);
        break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Class_RingBufferRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("RingBuffer");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

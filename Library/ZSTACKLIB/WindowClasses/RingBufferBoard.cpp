#include <Windows.h>
#include <zstack.h>

#define RBM_INIT        (WM_USER + 0)
#define RBM_UPDATE      (WM_USER + 1)

static int progress;
static unsigned int start;
static unsigned int end;
static unsigned int wp;
static unsigned int rp;
static unsigned int progress_wp;
static unsigned int progress_rp;
static unsigned int start_new, end_new, wp_new, rp_new;

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

        if (progress_wp > progress_rp)
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
    case RBM_UPDATE:
        // wParam = read, lParam = write
        rp = LOWORD(wParam);
        wp = LOWORD(lParam);

        if ((wp < start) || (wp > end))
            break;

        if ((rp < start) || (rp > end))
            break;

        start_new = 0;
        end_new = end - start;
        wp_new = wp - start;
        rp_new = rp - start;
        
        GetClientRect(hWnd, &client_rect);

        progress_wp = (unsigned int)wp_new * 100 / end_new;
        progress_rp = (unsigned int)rp_new * 100 / end_new;

        progress_wp = (unsigned int)progress_wp * (client_rect.right - client_rect.left) / 100;
        progress_rp = (unsigned int)progress_rp * (client_rect.right - client_rect.left) / 100;
        
        InvalidateRect(hWnd, &client_rect, TRUE);
        break;
    case RBM_INIT:
        // wParam = start, lParam = end
        start = LOWORD(wParam);
        end = LOWORD(lParam);
        break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int RingBufferBoardRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("RingBufferBoard");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

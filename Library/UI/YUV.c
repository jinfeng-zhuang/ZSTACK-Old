#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <commctrl.h>
#include <zstack/zstack.h>

static HWND ctrl;
struct YUV yuv;
static struct YUV_Wrapper *wrapper;
static HDC hdcCanvas;
static HBITMAP bitmapCanvas;
static BITMAP bitmap;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT rc;
	struct YUV tmp;
	int i;
	int j;
	unsigned char Y, U, V;
	unsigned int value;
	unsigned int rgba;
	float ratioW, ratioH;
	unsigned int *buffer;
	int width;
	int height;
	int x, y;
	
	GetClientRect(hWnd, &rc);

	switch (uMsg)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

		hdc = GetDC(hWnd);
		hdcCanvas = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_SIZE:
		if (bitmap.bmBits)
			free(bitmap.bmBits);

		bitmap.bmType = 0;
		bitmap.bmWidth = (rc.right - rc.left);
		bitmap.bmWidthBytes = bitmap.bmWidth * 4;
		bitmap.bmHeight = (rc.bottom - rc.top);
		bitmap.bmBitsPixel = 32;
		bitmap.bmPlanes = 1;
		bitmap.bmBits = malloc((rc.right - rc.left) * (rc.bottom - rc.top) * 4);
		break;
	case WM_PAINT:
		buffer = (unsigned int*)bitmap.bmBits;

		if (yuv.buffer && buffer) {
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			

			ratioW = (float)yuv.width / width;
			ratioH = (float)yuv.height / height;

			for (j = 0; j < height; j++) {
				for (i = 0; i < width; i++) {
					x = i * ratioW;
					y = j * ratioH;

					Y = wrapper->GetY(yuv.buffer, yuv.width, yuv.height, x, y);
					U = wrapper->GetU(yuv.buffer, yuv.width, yuv.height, x, y);
					V = wrapper->GetV(yuv.buffer, yuv.width, yuv.height, x, y);

					rgba = yuv2rgba((unsigned int)Y | (U << 8) | (V << 16));

					buffer[j * width + i] = rgba;
				}
			}
		}

		if (bitmapCanvas)
			DeleteObject(bitmapCanvas);
		bitmapCanvas = CreateBitmapIndirect(&bitmap); // copy from Source to HBITMAP

		if (bitmapCanvas)
			SelectObject(hdcCanvas, bitmapCanvas);

		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcCanvas, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_USER:
		memcpy(&tmp, lParam, sizeof(struct YUV));
		if ((tmp.format != yuv.format) || (tmp.width != yuv.width) || (tmp.height != yuv.height)) {
			if (yuv.buffer) {
				free(yuv.buffer);
			}

			memcpy(&yuv, lParam, sizeof(struct YUV));
			wrapper = YUV_Wrapper_Init(yuv.format);
			yuv.buffer = malloc(wrapper->FrameSize(yuv.width, yuv.height));
		}
		
		if (yuv.buffer && tmp.buffer) {
			memcpy(yuv.buffer, tmp.buffer, wrapper->FrameSize(yuv.width, yuv.height));
			InvalidateRect(hWnd, NULL, FALSE);
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

int Class_YUV_Register(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("YUV");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return TRUE;
	}

	return FALSE;
}

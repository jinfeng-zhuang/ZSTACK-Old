#pragma comment(lib, "Msimg32.lib")

#include <Windows.h>
#include <math.h>
#include <assert.h>

#include <zstack.h>

static HBITMAP hBitmap;
static HBITMAP hBitmapArray[10];

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
    HDC hdcMem;
    BITMAP bitmap;
    int width, height, depth, stride;
    unsigned char *bgra = NULL;
    int i;
    BLENDFUNCTION ftn = { 0 };

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	switch (uMsg)
	{
	case WM_CREATE:
        hdc = GetDC(hWnd);
        
        hBitmapArray[0] = CreateCompatibleBitmap(hdc, 500, 500);
    
        hdcMem = CreateCompatibleDC(hdc);
        
        SelectObject(hdcMem, hBitmapArray[0]);

        // draw with GDI interface, rectangle, line, ecllipse, text...
        // no no no, draw on hdcMem, not bitmap
        SetBkMode(hdcMem, TRANSPARENT);

        TextOut(hdcMem, 0, 0, TEXT("Hello World"), 11);

        DeleteDC(hdcMem);

        ReleaseDC(hWnd, hdc);

        GetObject(hBitmapArray[0], sizeof(BITMAP), &bitmap);
        
        width = bitmap.bmWidth;
        height = bitmap.bmHeight;
        depth = bitmap.bmBitsPixel;
        stride = bitmap.bmWidthBytes;

        bgra = (unsigned char *)malloc(width * height * depth >> 3);
        assert(NULL != bgra);
        memset(bgra, 0, width * height * depth >> 3);

        // draw, global color process, etc...
        GetBitmapBits(hBitmapArray[0], stride * height, bgra);

        for (i=0; i<stride * height; i+=4) {
            //bgra[i+0] = 100;
            //bgra[i+3] = 0x7F; // 50%
        }
        
        SetBitmapBits(hBitmapArray[0], stride * height, bgra);
        
        break;
	case WM_SIZE:
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        hdcMem = CreateCompatibleDC(hdc);

        // select correct image
        SelectObject(hdcMem, hBitmapArray[0]);
        
        ftn.BlendOp = AC_SRC_OVER;
        ftn.BlendFlags = 0;
        ftn.AlphaFormat = AC_SRC_ALPHA; 
        ftn.SourceConstantAlpha = 100; // global alpha value, always enabled
        AlphaBlend(hdc, 0, 0, 256, 256, hdcMem, 0, 0, 256, 256, ftn);

        DeleteDC(hdcMem);
   
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_DROPFILES:
		wchar_t filename[100];
		DragQueryFile((HDROP)wParam, 0, filename, 100);
		DragFinish((HDROP)wParam);
		wprintf(L"filename = %s\n", filename);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int BitmapBoardRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("BitmapBoard");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}


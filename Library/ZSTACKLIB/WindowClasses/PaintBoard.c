#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <math.h>
#include <gdiplus.h>
#include <assert.h>

#include <zstack.h>

using namespace Gdiplus;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    POINT center = { 100, 100 };
    static HDC hdcMem;
    int i, j;
    FILE* fp;
    int file_size;
    static unsigned char* yuv_buffer = NULL;
    unsigned char r, g, b;
    unsigned char y, u, v;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        fp = fopen("test_444.yuv", "rb"); // TODO 420
        assert(NULL != fp);

        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        yuv_buffer = (unsigned char*)malloc(file_size);
        assert(NULL != yuv_buffer);

        fread(yuv_buffer, 1, file_size, fp);

        fclose(fp);
        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        for (i = 0; i < 1080; i++) {
            for (j = 0; j < 1920; j++) {
                if (yuv_buffer) {
                    y = yuv_buffer[1920 * 1080 * 0 + i * 1920 + j];
                    v = yuv_buffer[1920 * 1080 * 1 + i * 1920 + j];
                    u = yuv_buffer[1920 * 1080 * 2 + i * 1920 + j];

#if 1
                    r = ((256 * y + (351 * v)) >> 8);
                    g = ((256 * y - (86 * u) - (179 * v)) >> 8);
                    b = ((256 * y + (444 * u)) >> 8);
#else
                    r = y + (1.370705 * v);
                    g = y - (0.337633 * u) - (0.698001 * v);
                    b = y + (1.732446 * u);
#endif
                    //SetPixel(hdc, j, i, RGB(yuv_buffer[i * 1920 + j], yuv_buffer[i * 1920 + j], yuv_buffer[i * 1920 + j]));
                    SetPixel(hdc, j, i, RGB(r, g, b));
                }
            }
        }
        
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int PaintBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("PaintBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

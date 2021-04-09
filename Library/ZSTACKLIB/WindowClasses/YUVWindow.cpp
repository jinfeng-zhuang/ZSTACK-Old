// TODO: bmp replace with paper, the effection of SetPixel is too low
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <zstack.h>
#include <math.h>

#define INSTANCE_MAX    (100)
#define DEFAULT_WIDTH   (720)
#define DEFAULT_HEIGHT  (288)
#define DEFAULT_FORMAT  (0) // 420

struct config {
    HWND hwnd;

    HBITMAP hPaper;
    HDC hDeck;
    HBRUSH hBrush;

    TCHAR filename[MAX_PATH];
    char filename_ansi[MAX_PATH];

    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int format; // enum YUVFormat

    int index;
    int total;

    unsigned char *data;
} config_array[INSTANCE_MAX];

static struct config *GetConfig(HWND hwnd)
{
    int i;

    for (i = 0; i < INSTANCE_MAX; i++) {
        if (hwnd == config_array[i].hwnd) {
            return &config_array[i];
        }
    }

    for (i = 0; i < INSTANCE_MAX; i++) {
        if (NULL == config_array[i].hwnd) {
            memset(&config_array[i], 0, sizeof(config_array[0]));
            config_array[i].hwnd = hwnd;
            return &config_array[i];
        }
    }

    return NULL;
}

static void Draw(HWND hwnd)
{
    unsigned int i, j;
    unsigned int color;
    unsigned char r,g,b;
    struct config *config;
    RECT rect;
    FILE *fp;

    GetClientRect(hwnd, &rect);

    config = GetConfig(hwnd);
    if (NULL == config)
        return;

    if (0 == config->filename[0]) {
        draw_text(config->hDeck, L"Left Click to select a YUV file", &rect, 0x000000, 0xFFFFFF, 18);
    }
    else {
        memset(config->filename_ansi, 0, MAX_PATH);
        WideCharToMultiByte(CP_ACP, 0, config->filename, -1, config->filename_ansi, MAX_PATH, NULL, NULL);

        fp = fopen(config->filename_ansi, "rb");
        if (NULL == fp) {
            return;
        }
        
        if (NULL == config->data) {
            return;
        }

        fseek(fp, config->width * config->height * 3 / 2 * config->index, SEEK_SET);
        fread(config->data, 1, config->width * config->height, fp);

        fclose(fp);

        //Rectangle(config->hDeck, 0, 0, config->width, config->height);
        
        for (i = 0; i < config->width; i++) {
            for (j=0; j <config->height; j++) {
                r = g = b = I420_Y(i, j, config->stride, config->height, config->data);
                color = r<<0 | g<<8 | b<<16;
                SetPixel(config->hDeck, i, j, color);
            }
        }

        draw_text(config->hDeck, L"Index/Total", &rect, 0x000000, 0xFFFFFF, 18);
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HBITMAP hPaperOld;
    RECT rect;
    OPENFILENAME ofn = {0};
    struct config *config;

    config = GetConfig(hWnd);
    if (config == NULL) {
        return 0;
    }

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    GetClientRect(hWnd, &rect);

    switch (uMsg)
    {
    case WM_CREATE:
        hdcWindow = GetDC(hWnd);
        config->hDeck = CreateCompatibleDC(hdcWindow);
        config->hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
        config->hBrush = CreateSolidBrush(RGB(0, 0, 0xFF));

        config->width = DEFAULT_WIDTH;
        config->height = DEFAULT_HEIGHT;
        config->stride = DEFAULT_WIDTH;
        config->format = 0;
        config->data = (unsigned char *)malloc(DEFAULT_WIDTH * DEFAULT_HEIGHT);

        SelectObject(config->hDeck, config->hPaper);
        ReleaseDC(hWnd, hdcWindow);
        break;
    case WM_SIZE:
        config->hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
        hPaperOld = (HBITMAP)SelectObject(config->hDeck, config->hPaper);
        DeleteObject(hPaperOld);
        break;
    case WM_PAINT:
        hdcWindow = BeginPaint(hWnd, &ps);
        Draw(hWnd);
        BitBlt(hdcWindow, 0, 0, rect.right - rect.left, rect.bottom - rect.top, config->hDeck, 0, 0, SRCCOPY);
        EndPaint(hWnd, &ps);
        break;
    case WM_USER:
        if (wParam == WIN_YUV_RESOLUTION) {
            config->width = (lParam >> 16) & 0xFFFF;
            config->stride = (lParam >> 16) & 0xFFFF; // TODO
            config->height = (lParam) & 0xFFFF;
        }
        else if (wParam == WIN_YUV_INDEX) {
            config->index = lParam;
        }
        else {
        }

        if (config->data) {
            free(config->data);
            config->data = (unsigned char *)malloc(config->width * config->height); // TODO consider YUV format
        }

        InvalidateRect(hWnd, &rect, FALSE);
        break;
    case WM_LBUTTONDOWN:
        memset(&ofn, 0, sizeof(ofn));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.lpstrFilter = TEXT("YUV File\0*.yuv\0");
        ofn.nFilterIndex = 1;
        ofn.lpstrFile = config->filename;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrInitialDir = NULL;
        ofn.lpstrTitle = TEXT("Select a YUV file");
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
        GetOpenFileName(&ofn);
        InvalidateRect(hWnd, &rect, FALSE);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int YUVWindow_Register(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("YUVWindow");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

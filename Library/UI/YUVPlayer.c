// TODO: bmp replace with paper, the effection of SetPixel is too low
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <zstack/zstack.h>
#include <math.h>

#define INSTANCE_MAX    (100)
#define DEFAULT_WIDTH   (100)
#define DEFAULT_HEIGHT  (100)
#define DEFAULT_FORMAT  (0) // 420

struct config {
    HWND hwnd;

    HWND hwnd_resolution;
    HWND hwnd_index;
    HWND hwnd_prev;
    HWND hwnd_next;
    HWND hwnd_play;

    HBITMAP hPaper;
    HDC hDeck;
    HBRUSH hBrush;

    TCHAR filename[MAX_PATH];
    char filename_ansi[MAX_PATH];
    char content_resolution[32];

    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int format; // enum YUVFormat

    int index;
    int index_max;
    int total;

    int source; // 0 - file, 1 - pointer

    unsigned char *data;

    int flag_hide;
    int flag_play;
    int flag_update;
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
    unsigned int width;
    unsigned int height;
    float x, y;

    GetClientRect(hwnd, &rect);

    width = rect.right - rect.left;
    height = rect.bottom - rect.top;

    config = GetConfig(hwnd);
    if (NULL == config)
        return;

    if (0 == config->source) {
        rect.top = height / 3;
        rect.left = width / 3;

        if (0 == config->filename[0]) {
            draw_text(config->hDeck, L"Select a file", &rect, 0x000000, 0xFFFFFF, 20);
        }
        else if (NULL == config->data) {
            draw_text(config->hDeck, L"Enter Resolution", &rect, 0x000000, 0xFFFFFF, 20);
        }
    }
    else {
        rect.top = height / 3;
        rect.left = width / 3;

        draw_text(config->hDeck, L"Merge Result", &rect, 0x000000, 0xFFFFFF, 30);
    }

    if (config->data == NULL)
        return;

    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            x = i * ((float)config->width / width);
            y = j * ((float)config->height / height);
            r = g = b = I420_Y((int)x, (int)y, config->stride, config->height, config->data);
            color = r<<0 | g<<8 | b<<16;
            SetPixel(config->hDeck, i, j, color);
        }
    }
}

static void index_update(struct config *config, int inc)
{
    if (NULL == config)
        return;

    config->index += inc;

    if (config->index >= config->index_max)
        config->index = config->index_max - 1;
    else if (config->index < 0)
        config->index = 0;
    else {
    }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    FILE *fp;
    HDC hdcWindow;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    HBITMAP hPaperOld;
    RECT rect;
    OPENFILENAME ofn = {0};
    struct config *config;
    char str_index[32];
    CREATESTRUCTA *style;
    YUV_INFO info;
    int ret;

    config = GetConfig(hWnd);
    if (config == NULL) {
        return 0;
    }

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
    GetClientRect(hWnd, &rect);

    switch (uMsg)
    {
    case WM_CREATE:
        style = (CREATESTRUCTA *)lParam;

        hdcWindow = GetDC(hWnd);
        config->hDeck = CreateCompatibleDC(hdcWindow);
        config->hPaper = CreateBitmap(rect.right - rect.left, rect.bottom - rect.top, 32, 1, NULL);
        config->hBrush = CreateSolidBrush(RGB(0, 0, 0xFF));

        SelectObject(config->hDeck, config->hPaper);
        ReleaseDC(hWnd, hdcWindow);

        if (0 == (style->style & YUV_STYLE_EXTERNAL)) {
            config->hwnd_resolution = CreateWindowEx(
                0,
                TEXT("EDIT"),
                NULL,
                WS_BORDER | WS_VISIBLE | WS_CHILD,
                0, 0, 100, 20,
                hWnd,
                NULL,
                hInstance,
                NULL);
            config->hwnd_prev = CreateWindowEx(
                0,
                TEXT("BUTTON"),
                TEXT("<"),
                WS_BORDER | WS_VISIBLE | WS_CHILD,
                100, 0, 50, 20,
                hWnd,
                NULL,
                hInstance,
                NULL);
            config->hwnd_index = CreateWindowEx(
                0,
                TEXT("EDIT"),
                NULL,
                WS_BORDER | WS_VISIBLE | WS_CHILD,
                150, 0, 50, 20,
                hWnd,
                NULL,
                hInstance,
                NULL);
            config->hwnd_next = CreateWindowEx(
                0,
                TEXT("BUTTON"),
                TEXT(">"),
                WS_BORDER | WS_VISIBLE | WS_CHILD,
                200, 0, 50, 20,
                hWnd,
                NULL,
                hInstance,
                NULL);
            config->hwnd_play = CreateWindowEx(
                0,
                TEXT("BUTTON"),
                TEXT(">>"),
                WS_BORDER | WS_VISIBLE | WS_CHILD,
                260, 0, 50, 20,
                hWnd,
                NULL,
                hInstance,
                NULL);
        }
        else {
            config->source = 1;
        }
        
        config->flag_play = 0;
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
        if (wParam == YUV_UPDATE) {
            memcpy(&info, (void *)lParam, sizeof(YUV_INFO));
            config->width = info.width;
            config->height = info.height;
            config->stride = config->width;
            if (config->data) {
                free(config->data);
            }
            config->data = (unsigned char *)malloc(info.width * info.height);
            if (config->data) {
                memcpy(config->data, info.data, config->width * config->height);
            }

            InvalidateRect(hWnd, &rect, FALSE);
        }
        else if (wParam == YUV_GET_INFO) {
            info.width = config->width;
            info.height = config->height;
            info.data = config->data;

            memcpy((void *)lParam, &info, sizeof(YUV_INFO));
        }
        else if (YUV_GET_FILENAME == wParam) {
            *((unsigned int *)lParam) = (unsigned int)(config->filename_ansi);
        }
        else {
        }
        
        break;
    case WM_COMMAND:
        if (config->hwnd_play == (HWND)lParam) {
            if (config->flag_play == 0) {
                SetTimer(hWnd, NULL, 300, NULL);
                SetWindowTextA(config->hwnd_play, "||");
            }
            else {
                KillTimer(hWnd, NULL);
                SetWindowTextA(config->hwnd_play, ">>");
            }
            config->flag_play = ~config->flag_play;
        }
        else if (config->hwnd_prev == (HWND)lParam) {
            index_update(config, -1);

            _snprintf(str_index, sizeof(str_index), "%d/%d", config->index, config->index_max);
            SetWindowTextA(config->hwnd_index, str_index);

            config->flag_update = 1;
        }
        else if (config->hwnd_next == (HWND)lParam) {
            index_update(config, 1);

            _snprintf(str_index, sizeof(str_index), "%d/%d", config->index, config->index_max);
            SetWindowTextA(config->hwnd_index, str_index);

            config->flag_update = 1;
        }
        else if (config->hwnd_resolution == (HWND)lParam) {
            if (EN_KILLFOCUS == HIWORD(wParam)) {
                GetWindowTextA(config->hwnd_resolution, config->content_resolution, sizeof(config->content_resolution));
                ret = sscanf(config->content_resolution, "%dx%d", &config->width, &config->height);

                if ((ret < 2) || (config->width * config->height <= 0)) {
                    break;
                }

                config->stride = config->width;

                config->index_max = (int)fsize(config->filename_ansi) / (config->width * config->height * 3 / 2);

                if (config->data) {
                    free(config->data);
                }
                config->data = (unsigned char *)malloc(config->width * config->height); // TODO consider YUV format

                _snprintf(str_index, sizeof(str_index), "%d/%d", config->index, config->index_max);
                SetWindowTextA(config->hwnd_index, str_index);

                config->flag_update = 1;
            }
        }
        else {
        }

        if (0 != config->filename_ansi[0]) {
            fp = fopen(config->filename_ansi, "rb");
            if (NULL == fp) {
                return 0;
            }

            fseek(fp, config->index * config->width * config->height * 3 / 2, SEEK_SET); // TODO: YUV FORMAT
            
            if (config->data) {
                fread(config->data, 1, config->width * config->height, fp);
            }

            fclose(fp);
        }

        if (config->flag_update) {
            config->flag_update = 0;
            InvalidateRect(hWnd, NULL, FALSE);

            SendMessage(GetParent(hWnd), WM_USER, YUV_UPDATE, (LPARAM)hWnd);
        }

        break;
    case WM_LBUTTONDBLCLK:
        if (config->source == 0) {
            memset(&ofn, 0, sizeof(ofn));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.lpstrFilter = TEXT("YUV File\0*.yuv\0");
            ofn.nFilterIndex = 1;
            ofn.lpstrFile = config->filename;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle = TEXT("Select a YUV file");
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
            if (0 != GetOpenFileName(&ofn)) {
                memset(config->filename_ansi, 0, MAX_PATH);
                WideCharToMultiByte(CP_ACP, 0, config->filename, -1, config->filename_ansi, MAX_PATH, NULL, NULL);
            }
            InvalidateRect(hWnd, &rect, FALSE);
        }
        break;
    case WM_TIMER:
        index_update(config, 1);
        _snprintf(str_index, sizeof(str_index), "%d/%d", config->index, config->index_max);
        SetWindowTextA(config->hwnd_index, str_index);
        InvalidateRect(hWnd, NULL, FALSE);
        SendMessage(GetParent(hWnd), WM_USER, YUV_UPDATE, (LPARAM)hWnd);
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
    wce.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

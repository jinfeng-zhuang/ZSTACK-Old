// 处理 FIFO 队列，直到遇到一个分隔命令，将绘制好的图像放到 Frame Buffer 中。

#include <Windows.h>
#include <process.h>
#include <stdio.h>

struct fifo {
    int size;
    int rd;
    int wr;
    int count;
};

#if 0
enum {
    DRAW_CMD_START,
    DRAW_CMD_BITMAP,
    DRAW_CMD_TEXT,
    DRAW_CMD_LINE,
    DRAW_CMD_END,
    DRAW_CMD_MAX
};

struct draw_cmd {
    int cmd;
    HDC hdc;
    BITMAP bitmap;
    RECT rect;
    POINT point[4];
};

#define CMD_NUMBER  128

struct draw_cmd draw_cmd_array[CMD_NUMBER];
struct fifo draw_cmd_fifo = { CMD_NUMBER, 0, 0 };
#endif

#define HDC_NUMBER  16

static HDC hdc_array[HDC_NUMBER];
static int hdc_width, hdc_height;
static HBITMAP bitmap_array[HDC_NUMBER];
static struct fifo hdc_fifo = { HDC_NUMBER , 0, 0 };

static HDC hdc_bitmap;

static HBITMAP bitmap_user;
static HDC hdc_user;

int DrawCmd_Begin(BOOL empty)
{
    if (hdc_fifo.count >= hdc_fifo.size)
        return -1;

    if (!empty) {
        BitBlt(hdc_array[hdc_fifo.wr], 0, 0, hdc_width, hdc_height, hdc_user, 0, 0, SRCCOPY);
    }
    else {
        Rectangle(hdc_array[hdc_fifo.wr], 0, 0, hdc_width, hdc_height);
    }

    return 0;
}

void DrawCmd_End(void)
{
    hdc_fifo.wr = (hdc_fifo.wr + 1) % hdc_fifo.size;
    hdc_fifo.count++;
}

HDC DrawCmd_Get(void)
{
    if (hdc_fifo.count > 0) {
        BitBlt(hdc_user, 0, 0, hdc_width, hdc_height, hdc_array[hdc_fifo.rd], 0, 0, SRCCOPY);

        hdc_fifo.rd = (hdc_fifo.rd + 1) % hdc_fifo.size;

        hdc_fifo.count--; // 开头已经检查，不可能小于 0 的
    }

    return hdc_user;
}

void DrawCmd_Rectangle(RECT rect, COLORREF color, BOOL fill)
{
    HBRUSH brush;
    HBRUSH oldBrush;
    HDC hdc = hdc_array[hdc_fifo.wr];

    if (fill) {
        brush = CreateSolidBrush(color);
        oldBrush = (HBRUSH)SelectObject(hdc, brush);

        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

        SelectObject(hdc, oldBrush);
    }
    else {
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }
}

void DrawCmd_Line(POINT a, POINT b)
{
    HDC hdc = hdc_array[hdc_fifo.wr];

    MoveToEx(hdc, a.x, a.y, NULL);
    LineTo(hdc, b.x, b.y);
}

void DrawCmd_Bitmap(HBITMAP bitmap, RECT rect)
{
    HDC hdc = hdc_array[hdc_fifo.wr];

    SelectObject(hdc_bitmap, bitmap);

    BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, hdc_bitmap, 0, 0, SRCCOPY);
}

void DrawCmd_Text(wchar_t *text, RECT rect)
{
    HDC hdc = hdc_array[hdc_fifo.wr];

    DrawText(hdc, text, -1, &rect, DT_WORDBREAK | DT_LEFT | DT_VCENTER);
}

#if 0
static int DrawThread(void* argv)
{
    int id;

    printf("%s Started\n", __func__);

    while (1)
    {
        
        Sleep(1000);
    }
}
#endif

int DrawThread_Init(HWND hwnd)
{
    int i;
#if 0
    HANDLE hThread;
#endif
    HDC hdc;
    RECT rect;

#if 0
    hThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)DrawThread, NULL, 0, NULL);
    if (NULL == hThread)
        return -1;
#endif

    hdc = GetDC(hwnd);

    GetClientRect(hwnd, &rect);

    hdc_width = rect.right - rect.left;
    hdc_height = rect.bottom - rect.top;

    for (i = 0; i < HDC_NUMBER; i++) {
        bitmap_array[i] = CreateBitmap(hdc_width, hdc_height, 32, 1, NULL);

        hdc_array[i] = CreateCompatibleDC(hdc);

        SelectObject(hdc_array[i], bitmap_array[i]);
    }

    // its bitmap will imported by user
    hdc_bitmap = CreateCompatibleDC(hdc);

    bitmap_user = CreateBitmap(hdc_width, hdc_height, 32, 1, NULL);
    hdc_user = CreateCompatibleDC(hdc);
    SelectObject(hdc_user, bitmap_user);

    ReleaseDC(hwnd, hdc);

    return 0;
}

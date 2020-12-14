#include <Windows.h>
#include <zstack.h>
#include <3rdparty/png.h>

extern png_bytep* read_png_file(const char* file_name, int* width, int* height);
extern long ReadPngData(const char* szPath, int* pnWidth, int* pnHeight, unsigned char** cbData);

#define FUNCTION_MAX    8

enum {
    FUNC_Battlecry,
};

struct card {
    int id;
    wchar_t* name;
    int health;
    int damage;
    int cost;
    int durability;
    int rarity;
    wchar_t* description;
    int function[FUNCTION_MAX];
    wchar_t* image;
};

struct circle {
    int x;
    int y;
    int r;
};

struct rect {
    LONG x;
    LONG y;
    LONG w;
    LONG h;
};

struct card_internal {
    struct rect top;

    struct rect cost;
    struct rect health;
    struct rect damage;

    struct rect image;
    struct rect name;
    struct rect rarity;
    struct rect desc;
};

#define CARD_CIRCLE_WIDTH  30
#define CARD_WIDTH  300
#define CARD_HEIGHT 400
#define CARD_IMAGE_HEIGHT 150
#define CARD_NAME_HEIGHT 30
#define CARD_RARITY_HEIGHT 30
#define CARD_DESC_HEIGHT (CARD_HEIGHT - CARD_CIRCLE_WIDTH * 2 - CARD_IMAGE_HEIGHT - CARD_NAME_HEIGHT - CARD_RARITY_HEIGHT)

struct card_internal card_normal = {
    {0, 0, CARD_WIDTH, CARD_HEIGHT},

    {0, 0, CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH},
    {0, CARD_HEIGHT - CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH},
    {CARD_WIDTH - CARD_CIRCLE_WIDTH, CARD_HEIGHT - CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH},

    {CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH, CARD_WIDTH - 2 * CARD_CIRCLE_WIDTH, CARD_IMAGE_HEIGHT},
    {CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH + CARD_IMAGE_HEIGHT, CARD_WIDTH - 2 * CARD_CIRCLE_WIDTH, CARD_NAME_HEIGHT},
    {CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH + CARD_IMAGE_HEIGHT + CARD_NAME_HEIGHT, CARD_WIDTH - 2 * CARD_CIRCLE_WIDTH, CARD_RARITY_HEIGHT},
    {CARD_CIRCLE_WIDTH, CARD_CIRCLE_WIDTH + CARD_IMAGE_HEIGHT + CARD_NAME_HEIGHT + CARD_RARITY_HEIGHT, CARD_WIDTH - 2 * CARD_CIRCLE_WIDTH, CARD_DESC_HEIGHT},
};

static int offset;

static RECT card_pos_origin;

static int index = -1;

#define WM_USER_INDEX (WM_USER + 2)

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    offset++;

    offset %= 30;

    InvalidateRect(hwnd, NULL, TRUE);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdcOrig;
    static HDC hdc;
    HDC hdcImg;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int i;
    static HBITMAP hPngFr;
    static HBITMAP hPngBk;
    unsigned char* data;
    static HBRUSH blueBrush;
    static HBRUSH yellowBrush;
    static HBRUSH redBrush;
    static HBRUSH oldBrush;
    RECT rect;
    static POINT left_btn_down_pos = { 0, 0 };
    POINT delta;
    POINT cur_mouse_pos;

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_LEAVE | TME_HOVER;
    tme.dwHoverTime = 1;
    tme.hwndTrack = hWnd;

    static int min_entry = FALSE;
    static int hover = FALSE;

    static int drag = FALSE;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        int w, h;
        ReadPngData("test.png", &w, &h, &data);

        hPngFr = CreateBitmap(w, h, 32, 1, data);
        hPngBk = CreateBitmap(CARD_WIDTH, CARD_HEIGHT, 32, 1, NULL);

        blueBrush = CreateSolidBrush(RGB(0x33, 0x99, 0xFF));
        yellowBrush = CreateSolidBrush(RGB(200, 200, 0));
        redBrush = CreateSolidBrush(RGB(200, 0, 0));
        
        // 绘制
        hdcOrig = GetDC(hWnd);

        hdc = CreateCompatibleDC(hdcOrig);
        hdcImg = CreateCompatibleDC(hdcOrig);

        // 必须选入一张位图才能绘制
        SelectObject(hdc, hPngBk);
        SelectObject(hdcImg, hPngFr);

        SetBkMode(hdc, TRANSPARENT);

        oldBrush = (HBRUSH)SelectObject(hdc, blueBrush);
        Rectangle(hdc, card_normal.top.x, card_normal.top.y, card_normal.top.x + card_normal.top.w, card_normal.top.y + card_normal.top.h);

        SelectObject(hdc, blueBrush);
        Ellipse(hdc, card_normal.cost.x, card_normal.cost.y, card_normal.cost.x + card_normal.cost.w, card_normal.cost.y + card_normal.cost.h);

        SelectObject(hdc, yellowBrush);
        Ellipse(hdc, card_normal.health.x, card_normal.health.y, card_normal.health.x + card_normal.health.w, card_normal.health.y + card_normal.health.h);

        SelectObject(hdc, redBrush);
        Ellipse(hdc, card_normal.damage.x, card_normal.damage.y, card_normal.damage.x + card_normal.damage.w, card_normal.damage.y + card_normal.damage.h);

        SelectObject(hdc, oldBrush);
        Rectangle(hdc, card_normal.image.x, card_normal.image.y, card_normal.image.x + card_normal.image.w, card_normal.image.y + card_normal.image.h);
        Rectangle(hdc, card_normal.name.x, card_normal.name.y, card_normal.name.x + card_normal.name.w, card_normal.name.y + card_normal.name.h);
        Rectangle(hdc, card_normal.rarity.x, card_normal.rarity.y, card_normal.rarity.x + card_normal.rarity.w, card_normal.rarity.y + card_normal.rarity.h);
        Rectangle(hdc, card_normal.desc.x, card_normal.desc.y, card_normal.desc.x + card_normal.desc.w, card_normal.desc.y + card_normal.desc.h);

        BitBlt(hdc, card_normal.image.x, card_normal.image.y, card_normal.image.w, card_normal.image.h, hdcImg, 0, 0, SRCCOPY);
        DeleteDC(hdcImg);

        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, card_normal.cost.x + card_normal.cost.w / 2, card_normal.cost.y + card_normal.cost.h / 2, TEXT("7"), wcslen(TEXT("7")));
        TextOut(hdc, card_normal.damage.x + card_normal.damage.w / 2, card_normal.damage.y + card_normal.damage.h / 2, TEXT("7"), wcslen(TEXT("7")));
        TextOut(hdc, card_normal.health.x + card_normal.health.w / 2, card_normal.health.y + card_normal.health.h / 2, TEXT("5"), wcslen(TEXT("5")));

        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, card_normal.name.x, card_normal.name.y, TEXT("大法师安东尼奥"), wcslen(TEXT("大法师安东尼奥")));

        TextOut(hdc, card_normal.rarity.x, card_normal.rarity.y, TEXT("传说"), wcslen(TEXT("传说")));

        rect.left = card_normal.desc.x;
        rect.top = card_normal.desc.y;
        rect.right = card_normal.desc.x + card_normal.desc.w;
        rect.bottom = card_normal.desc.y + card_normal.desc.h;
        DrawText(hdc, TEXT("每当你释放一个法术，将一张“火球术”置入你的手牌"), -1, &rect, DT_WORDBREAK | DT_LEFT | DT_VCENTER);

        ReleaseDC(hWnd, hdcOrig);

        // 30 FPS
        //SetTimer(hWnd, 0, 33, TimerProc);

        break;
    case WM_SIZE:
        break;
#if 1
    case WM_MOUSEMOVE:
        if (!min_entry) {
            TrackMouseEvent(&tme);
        }

        if (drag) {
            GetCursorPos(&cur_mouse_pos);
            ScreenToClient(hWnd, &cur_mouse_pos);

            delta.x = cur_mouse_pos.x - left_btn_down_pos.x;
            delta.y = cur_mouse_pos.y - left_btn_down_pos.y;

            // send cur mouse pos - lbuttondown mouse pos
            SendMessage(GetParent(hWnd), WM_USER + 4, (WPARAM)& delta, (LPARAM)hWnd);
        }
        break;
    case WM_MOUSEHOVER:
        if (!hover) {
            hover = TRUE;
            printf("hover\n");
            SendMessage(GetParent(hWnd), WM_USER, 0, (LPARAM)hWnd);
        }
        break;
    case WM_MOUSELEAVE:
        hover = FALSE;
        SendMessage(GetParent(hWnd), WM_USER + 1, 0, (LPARAM)hWnd);
        printf("leave\n");
        break;
    case WM_LBUTTONDOWN:
        drag = TRUE;
        GetCursorPos(&left_btn_down_pos);
        ScreenToClient(hWnd, &left_btn_down_pos);
        SendMessage(GetParent(hWnd), WM_USER + 2, 0, (LPARAM)hWnd);
        break;
    case WM_LBUTTONUP:
        drag = FALSE;
        SendMessage(GetParent(hWnd), WM_USER + 3, 0, (LPARAM)hWnd);
        break;
#endif

    case WM_PAINT:
        printf("redraw %x\n", hWnd);

        hdcOrig = BeginPaint(hWnd, &ps);
        
        BitBlt(hdcOrig, offset * 5, 0, CARD_WIDTH, CARD_HEIGHT, hdc, 0, 0, SRCCOPY);

        //AlphaBlend(hdc, 100, 0, fr_x, fr_y, hdcFr, 0, 0, fr_x, fr_y, bf);

        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int CardBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("CardBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

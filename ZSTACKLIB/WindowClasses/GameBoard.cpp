#include <Windows.h>
#include <math.h>
#include "common.h"

struct card_db {
    int status; // 0 - prepare, 1 - attack
    HWND hwnd;
};

struct card_db card_db[4];

static HBITMAP bitmap[2];
static int bitmap_index;

int card_status(HWND hwnd)
{
    int i;
    int size = sizeof(card_db) / sizeof(struct card_db);
    for (i = 0; i < size; i++) {
        if (hwnd == card_db[i].hwnd)
            break;
    }

    if (i == size)
        return -1;

    return i;
}

static POINT cursor[2];

struct block {
    RECT rect;
    POINT center;
    HBITMAP bitmap;
    int status;
    int card;
};

struct card {
    int position;
    HBITMAP bitmap;
};

struct card cards[4] = {
    {0, NULL},
    {1, NULL},
    {2, NULL},
    {3, NULL},
};

static struct block block_array[(10 + 1 + 7) * 2];

static RECT rect_array[(10 + 1 + 7) * 2];

static void rect_array_init(int width, int height)
{
    int i, j;
    int block_width;
    int block_height;
    int real_width;
    int margin;

    block_width = width / 10;
    block_height = height / 6;


    for (i = 0; i < 10; i++) {
        block_array[i].rect.left = block_width * i;
        block_array[i].rect.right = block_width * (i + 1);
        block_array[i].rect.top = 0;
        block_array[i].rect.bottom = block_height;
        block_array[i].status = 0;
    }

    block_array[10].rect.left = width / 2 - block_width / 2;
    block_array[10].rect.right = width / 2 + block_width / 2;
    block_array[10].rect.top = block_height;
    block_array[10].rect.bottom = block_height * 2;
    block_array[i].status = 1;

    real_width = width / 7;
    margin = (real_width - block_width) / 2;

    for (i = 11; i < 18; i++) {
        j = i - 11;

        block_array[i].rect.left = real_width * j + margin;
        block_array[i].rect.right = block_array[i].rect.left + block_width;
        block_array[i].rect.top = block_height * 2;
        block_array[i].rect.bottom = block_height * 3;
        block_array[i].status = 1;
    }

    for (i = 18; i < 18 + 7; i++) {
        j = i - 18;

        block_array[i].rect.left = real_width * j + margin;
        block_array[i].rect.right = block_array[i].rect.left + block_width;
        block_array[i].rect.top = block_height * 3;
        block_array[i].rect.bottom = block_height * 4;
        block_array[i].status = 1;
    }

    block_array[25].rect.left = width / 2 - block_width / 2;
    block_array[25].rect.right = width / 2 + block_width / 2;
    block_array[25].rect.top = height - block_height * 2;
    block_array[25].rect.bottom = height - block_height;
    block_array[i].status = 1;

    for (i = 26; i < 36; i++) {
        j = i - 26;
        block_array[i].rect.left = block_width * j;
        block_array[i].rect.right = block_width * (j + 1);
        block_array[i].rect.top = height - block_height;
        block_array[i].rect.bottom = height;
        block_array[i].status = 0;
    }
}

static POINT BlockCenter(RECT rect)
{
    POINT p;

    p.x = (rect.left + rect.right) >> 1;
    p.y = (rect.top + rect.bottom) >> 1;

    return p;
}

// 动画系统
// 指向运动： src ---- cur ---- dst
// 攻击攻击： src ---- cur ---- dst
// GIF ？
// 特效 ？
// 记录的是一个行为，适用于所有的对象
// 需要增加触发条件吗？
// 完成后从队列里删除

struct animation_move {
    int valid;
    int direction;
    int X; // 作为参数给算法用于计算 step
    int src;
    int dst;
    POINT cur;
};

struct animation_move animation_move_array[1];

struct attack {
    int valid;

    int from;
    int to;

    int step_x;
    int step_y;

    RECT origin;
    int direction;
};

struct attack attack;

static int isCollide(RECT a, RECT b)
{
    int distance;

    distance = (abs(BlockCenter(a).x - BlockCenter(b).x) + abs(BlockCenter(a).y - BlockCenter(b).y))^2;

    if (distance < (50 ^ 2))
        return TRUE;

    return FALSE;
}

// 历史：
// 注意保持符号：不能强制赋值常量
// 如何水平或垂直，速度就会很快，一下子就到了，这是因为 X= 1, DletaY = SPEED= 直线距离
// 对角线运动会抖动，这和直线算法的平滑度有关，如何倍数，这个抖动会更加厉害
static void AttackAdjustStep(RECT from, RECT to)
{
    POINT a, b, c;
    int speed = 50;

    a = BlockCenter(from);
    b = BlockCenter(to);

    c.x = b.x - a.x;
    c.y = b.y - a.y;

    // TODO 0
    int k = (abs(c.x) > abs(c.y)) ? abs(c.x) : abs(c.y);

    if (k != 0) {
        attack.step_x = c.x / k;
        attack.step_y = c.y / k;

        attack.step_x *= speed;
        attack.step_y *= speed;
    }
    else {
        attack.step_x = 0;
        attack.step_y = 0;
    }
}

// src & dest = block index
static void Attack(int from, int to)
{ 
    POINT a, b;
    int c;

    attack.valid = 1;

    a = BlockCenter(block_array[from].rect);
    b = BlockCenter(block_array[to].rect);

    attack.from = from;
    attack.to = to;
    attack.origin = block_array[from].rect;
    
    attack.direction = 0; // 0 = to, 1 = back
}

static void BlockStep(RECT *rect, int step_x, int step_y)
{
    rect->left += step_x;
    rect->right += step_x;
    rect->top += step_y;
    rect->bottom += step_y;
}

static int FindBlock(POINT p)
{
    int i;

    for (i = 0; i < sizeof(block_array) / sizeof(block_array[0]); i++) {
        if ((p.x >= block_array[i].rect.left) && (p.x < block_array[i].rect.right)) {
            if ((p.y >= block_array[i].rect.top) && (p.y < block_array[i].rect.bottom)) {
                return i;
            }
        }
    }

    return -1;
}

static int selected = -1;

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    RECT rect;
    int i;

    // 更新卡牌位置和箭头以及其他特效？
    if (attack.valid) {
        if (attack.direction == 0) {
            AttackAdjustStep(block_array[attack.from].rect, block_array[attack.to].rect);
            if (isCollide(block_array[attack.from].rect, block_array[attack.to].rect)) {
                attack.direction = 1;
            }
        }
        else {
            AttackAdjustStep(block_array[attack.from].rect, attack.origin);
            if (memcmp(&block_array[attack.from].rect, &attack.origin, sizeof(RECT)) == 0) {
                attack.direction = 0;
                attack.valid = 0;
            }
        }

        BlockStep(&block_array[attack.from].rect, attack.step_x, attack.step_y);
    }

    // END 特效

    if (0 == DrawCmd_Begin(TRUE)) {

        // 绘制辅助线，固定的位置分布
        for (i = 0; i < sizeof(block_array) / sizeof(block_array[0]); i++) {
            DrawCmd_Rectangle(block_array[i].rect, 0, FALSE);
        }

        // 绘制卡牌，根据卡牌所在位置

        // 绘制箭头，如果存在的话
        if ((selected != -1) && (block_array[selected].status == 1)) {
            DrawCmd_Line(cursor[0], cursor[1]);
        }

        DrawCmd_End();
    }

    InvalidateRect(hwnd, NULL, FALSE);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    HDC hdcFrame;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int i;
    HWND hwnd;
    POINT mousePos;
    static HWND hwndSelected = NULL;
    RECT rect;
    POINT delta;
    static int drag = 0;
    static int status = 0;
    POINT apt[4];
    static POINT mousePosPrev = { 0, 0 };
    int index;
    
    POINT center;

    //printf("Window Message: 0x%04X\n", uMsg);

    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

    switch (uMsg)
    {
    case WM_CREATE:
        DrawThread_Init(hWnd);
        SetTimer(hWnd, 0, 1000/30, TimerProc);
        GetClientRect(hWnd, &rect);
        rect_array_init(rect.right - rect.left, rect.bottom - rect.top);
        break;
    case WM_SIZE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rect);

        hdcFrame = DrawCmd_Get();

        if (hdcFrame != NULL)
            BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcFrame, 0, 0, SRCCOPY);

        EndPaint(hWnd, &ps);
        break;
    case WM_KEYDOWN:
        break;
    case WM_COMMAND:
        break;

    case WM_LBUTTONDOWN:
        cursor[0].x = LOWORD(lParam);
        cursor[0].y = HIWORD(lParam);

        index = FindBlock(cursor[0]);
        if (index != -1)
            selected = index;
        break;

    case WM_LBUTTONUP:

        cursor[1].x = LOWORD(lParam);
        cursor[1].y = HIWORD(lParam);

        if ((selected != -1) && (FindBlock(cursor[1]) != -1) && (block_array[selected].status == 1)) {
            Attack(selected, FindBlock(cursor[1]));
        }

        selected = -1;
        break;

    case WM_MOUSEMOVE:
        cursor[1].x = LOWORD(lParam);
        cursor[1].y = HIWORD(lParam);

        if ((selected != -1) && (block_array[selected].status == 0)) {
            center = BlockCenter(block_array[selected].rect);
            BlockStep(&block_array[selected].rect, cursor[1].x - center.x, cursor[1].y - center.y);
        }

        break;

#if 0
    case WM_MOUSEMOVE:

        if (hwndSelected) {
            hdc = GetDC(hWnd);

            InvalidateRect(hWnd, NULL, FALSE);

            GetCursorPos(&mousePos);
            ScreenToClient(hWnd, &mousePos);

            apt[0].x = mousePosPrev.x;
            apt[0].y = mousePosPrev.y;

            apt[3].x = mousePos.x;
            apt[3].y = mousePos.y;

            if (abs(mousePos.x - mousePosPrev.x) > abs(mousePos.y - mousePosPrev.y)) {
                apt[1].x = (apt[0].x + apt[3].x) / 2;
                apt[1].y = (apt[0].y + apt[3].y) / 2 + 50;
                apt[2] = apt[1];
            }
            else {
                apt[1].x = (apt[0].x + apt[3].x) / 2 + 50;
                apt[1].y = (apt[0].y + apt[3].y) / 2;
                apt[2] = apt[1];
            }

            PolyBezier(hdc, apt, 4);

            ReleaseDC(hWnd, hdc);
        }
#if 0
        GetCursorPos(&mousePos);
        ScreenToClient(hWnd, &mousePos);
        hwnd = GetCardHwnd(mousePos);
        printf("%d %d hwnd %x\n", mousePos.x, mousePos.y, hwnd);
        if (hwnd) {
            InvalidateRect(hwnd, NULL, TRUE);
            GetWindowRect(hwnd, &rect);
            MapWindowPoints(NULL, hWnd, (LPPOINT)& rect, 2); // 2 ?
            SetWindowPos(hwnd, NULL, rect.left, 400, 300, 400, 0);
        }
#endif
#if 0 //  if mouse in card area, can't trigger this event in parent area
        if (hwndSelected) {
            InvalidateRect(hwndSelected, NULL, TRUE);
            GetCursorPos(&mousePos);
            ScreenToClient(hWnd, &mousePos);
            SetWindowPos(hwndSelected, NULL, mousePos.x, mousePos.y, 300, 400, 0);
        }
#endif 
        break;

        // 结束拖拽
    case WM_USER +3:
        hwnd = (HWND)lParam;
        drag = 0;
        
        if (hwnd != hwndSelected) {
            printf("%x attack %x\n", hwndSelected, hwnd);
        }

        hwndSelected = NULL;
        break;

    case WM_LBUTTONUP:
        // cancel attack
        hwndSelected = NULL;

        break;

        // 进行拖拽
    case WM_USER+4:
        hwnd = (HWND)lParam;
#if 0
        
        delta = *(POINT*)wParam;
        GetWindowRect(hwnd, &rect);
        MapWindowPoints(NULL, hWnd, (LPPOINT)&rect, 2);
        rect.left += delta.x;
        rect.top += delta.y;

        SetWindowPos(hwnd, 0, rect.left, rect.top, 300, 400, 0);
#else
        // attack
        printf("%x prepare attack\n", hwnd);
#endif
        break;
#endif
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int GameBoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("GameBoard");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

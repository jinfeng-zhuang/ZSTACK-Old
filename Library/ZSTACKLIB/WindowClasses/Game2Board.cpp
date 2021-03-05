#include <Windows.h>
#include <zstack.h>

// 数据库定义
// 数据越多，需要同步的数据也越多，越容易含义不明确

// 代表 8 个不同的对象的格子，原石计划有很多格子，这里只有固定的36个格子
// 格子本身没有属性吗，那为什么必须放在那些位置呢？所以还是有的。但是就像 MAC 和 IP 一样，分为两层，不要聚在一起。
// 格子的调整和卡牌无关，所以卡牌不能直接使用 RECT 表示自己的位置，而是像 IP 一样。
// 后期实现格子的调整，比如只有 5 个卡牌，调整一下格子，让这些格子分布宽松一些。
// 即函数 adjust_position(window size, hand card number, hero number, ...)

// 地址要解决一个问题，敌方和我方的问题
// 如果是对所有敌方随从造成伤害，这个从哪里遍历，从地址开始，还是另一个数组？应该从地址开始，从 MAC 层开始遍历

RECT position_array[8]; 

// 192.168.1.1
// 卡牌类型.卡牌位置索引
// 建立一个映射关系
// 所有的映射关系，应该能用数据表示，逻辑代码一定是简单的，不能是业务的

struct card {
    int position;
    HBITMAP bitmap;
    int type; // 法术/随从
    int cost; // 法力值消耗 注意要两份，会被更改
};

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HINSTANCE hInstance;

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
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

int Game2BoardRegister(HINSTANCE hInstance)
{
    WNDCLASSEX wce = { 0 };

    wce.cbSize = sizeof(wce);
    wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wce.hCursor = LoadCursor(NULL, IDC_ARROW);
    wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wce.hInstance = hInstance;
    wce.lpfnWndProc = WindowProc;
    wce.lpszClassName = TEXT("Game2Board");
    wce.style = CS_HREDRAW | CS_VREDRAW;
    if (!RegisterClassEx(&wce)) {
        return 1;
    }

    return 0;
}

#include <Windows.h>
#include "common.h"

// ���ݿⶨ��
// ����Խ�࣬��Ҫͬ��������ҲԽ�࣬Խ���׺��岻��ȷ

// ���� 8 ����ͬ�Ķ���ĸ��ӣ�ԭʯ�ƻ��кܶ���ӣ�����ֻ�й̶���36������
// ���ӱ���û����������Ϊʲô���������Щλ���أ����Ի����еġ����Ǿ��� MAC �� IP һ������Ϊ���㣬��Ҫ����һ��
// ���ӵĵ����Ϳ����޹أ����Կ��Ʋ���ֱ��ʹ�� RECT ��ʾ�Լ���λ�ã������� IP һ����
// ����ʵ�ָ��ӵĵ���������ֻ�� 5 �����ƣ�����һ�¸��ӣ�����Щ���ӷֲ�����һЩ��
// ������ adjust_position(window size, hand card number, hero number, ...)

// ��ַҪ���һ�����⣬�з����ҷ�������
// ����Ƕ����ез��������˺������������������ӵ�ַ��ʼ��������һ�����飿Ӧ�ôӵ�ַ��ʼ���� MAC �㿪ʼ����

RECT position_array[8]; 

// 192.168.1.1
// ��������.����λ������
// ����һ��ӳ���ϵ
// ���е�ӳ���ϵ��Ӧ���������ݱ�ʾ���߼�����һ���Ǽ򵥵ģ�������ҵ���

struct card {
    int position;
    HBITMAP bitmap;
    int type; // ����/���
    int cost; // ����ֵ���� ע��Ҫ���ݣ��ᱻ����
};

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HINSTANCE hInstance;
    int i;

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

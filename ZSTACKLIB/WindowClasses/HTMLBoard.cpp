//
// ������ʾһ�����ݵ��㷨��
// ���룺һ�����֣���ǰ�豸����
// �����INDEX 1 INDEX 2 INDEX 3
// 1. ͳ�Ƶ�������
// 2. ���������һ�����ʵĳ��ȴ���һ�е��ܳ��ȣ��Ͱ���������һ��
// 3. ���һ�����ʵĳ��ȱ����ͱ���Ҫ�󣬲���Ҫ���ӷ���ֱ�ӰѶ���Ĳ��ַŵ���һ��
// 
// ��������
// 1. ��ȡ�û��������ݵĴ�С���������� BUFFER
// 2. �����û���������ݽ� BUFFER A��������һ�Σ��������ԣ��õ�Ԫ��������������Ϊ�˶Ͽ�������ߵ���ϵ
// 3. �ڶ��ν���ǰ���������ú��ʵ����Ժ����ݿռ䣬������ŵ� BUFFER ATTR �� BUFFER DATA ���ͷ� BUFFER A
//
// ��ʾ��
// ��ʵӦ���е�һ�λ�ȡԪ��������ȷ����Ҫ������ڴ��С������������ʱ�䣻
// �ڶ���Ҫ��ȡ������ϸ�����ݣ���������ֵ�����̫���ˣ�û���ڵ�һ�η����ꣻ
// �Ҹ�������һ���õ�һ�������� BUFFER �� DUMP ���ݵ�ʱ��Ҳ�ܷ��㣬�Լ��Ķ����Լ��ܡ�
// �����ǹ̶��ģ��ܿռ�Ҳ�ǹ̶��ģ��������ݳ����ǿɱ�ģ����������ǲ���ġ�
//

#include <Windows.h>
#include "common.h"
#include "tinyxml2.h"

// stored as ANSI, but restored with Unicode, I don't know why
// there is a c code to copy data from harddisk to ddr, maybe the c code decode this
static wchar_t* html;

// css table
// global: consolas
// h1: 50
// h2: 40
// h3: 30
// p:  20
// b = i = ps

struct attr {
    const char name[32];
    int fontsize;
    unsigned int bgcolor;
    unsigned int fgcolor;
};

struct attr attr_map[] = {
    {"h1", 50, 0xFFFFFF, 0x000000},
    {"h2", 40, 0xFFFFFF, 0x000000},
    {"h3", 30, 0xFFFFFF, 0x000000},
    {"p", 20, 0xFFFFFF, 0x000000},
    {"a", 20, 0xFFFFFF, 0xFF0000},
    {"span", 20, 0xFFFFFF, 0x000000},
};

int get_one_line(HDC hdc, const WCHAR* text, RECT rect)
{
    int i;
    SIZE size;
    int line_size = rect.right - rect.left;

    for (i = 0; i < lstrlen(text); i++) {
        GetTextExtentPoint(hdc, text, i, &size);
        if (size.cx >= line_size) {
            break;
        }
    }

    if (i == lstrlen(text))
        return 0;

    return i - 1;
}

int draw_text(HDC hdc, const WCHAR *text, RECT *rect, COLORREF bgcolor, COLORREF fgcolor, int fontsize)
{
    DRAWTEXTPARAMS drawtextparams;
    LOGFONT logicfont;
    HFONT hfont;
    int ret;
    SIZE size;
    int start;

    memset(&logicfont, 0, sizeof(logicfont));
    wsprintf(logicfont.lfFaceName, TEXT("Consolas"));
    logicfont.lfHeight = fontsize;
    hfont = CreateFontIndirect(&logicfont);
    SelectObject(hdc, hfont);

    GetTextExtentPoint(hdc, L"X", lstrlen(L"X"), &size);

    SetBkColor(hdc, bgcolor);
    SetTextColor(hdc, fgcolor);

    drawtextparams.cbSize = sizeof(DRAWTEXTPARAMS);
    drawtextparams.iTabLength = 2;
    drawtextparams.iLeftMargin = 1;
    drawtextparams.iRightMargin = 1;

    start = 0;

    while (1) {
        ret = get_one_line(hdc, &text[start], *rect);

        DrawTextEx(hdc, (wchar_t *)&text[start], ret ? ret : -1, rect, DT_EXPANDTABS, &drawtextparams);

        rect->top += size.cy;

        if (0 == ret)
            break;

        start += ret;
    }

    return 0;
}

// accept only unicode
void HTMLShow(HWND hwnd, const WCHAR *text)
{
    RECT rect;

    html = (WCHAR *)text;

    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, true);
}

void node_scan(HDC hdc, tinyxml2::XMLElement* node, unsigned char *buffer, int buffer_length, RECT *rect)
{
    int i;

    if (node) {
        for (i = 0; i < sizeof(attr_map) / sizeof(attr_map[0]); i++) {
            if (0 == strcmp(node->Value(), attr_map[i].name)) {
                memset(buffer, 0, buffer_length);
                if (NULL != node->GetText()) {
                    MultiByteToWideChar(CP_UTF8, 0, (char*)node->GetText(), strlen(node->GetText()), (wchar_t*)buffer, buffer_length);
                    draw_text(hdc, (wchar_t*)buffer, rect, attr_map[i].bgcolor, attr_map[i].fgcolor, attr_map[i].fontsize);
                }
            }
        }

        node_scan(hdc, node->FirstChildElement(), buffer, buffer_length, rect);
        node_scan(hdc, node->NextSiblingElement(), buffer, buffer_length, rect);
    } 
}

static void preprocess(char* src, char* dest)
{
    int i;
    int j;
    int tmp;
    int slash_flag = false;

    for (i = 0, j = 0; i < strlen(src); i++) {

        if (src[i] == '<') {
            if ((0 == memcmp(&src[i], "<img", 4)) ||
                (0 == memcmp(&src[i], "<br", 3)) ||
                (0 == memcmp(&src[i], "<link", 5)) ||
                (0 == memcmp(&src[i], "<meta", 5)) ||
                (0 == memcmp(&src[i], "<input", 6))) {
                tmp = i;
                while (src[tmp++] != '>');
                if (src[tmp - 2] != '/') {
                    slash_flag = true;
                }
            }

            if (0 == memcmp(&src[i], "<script", 7)) {
                for (tmp = i + 7; ; tmp++) {
                    if (0 == memcmp(&src[tmp], "</script>", 9)) {
                        i = tmp + 9;
                        break;
                    }
                }
            }
        }

        if (src[i] == '>' && slash_flag) {
            slash_flag = false;
            dest[j++] = '/';
        }

        dest[j++] = src[i];
    }
}

int top = 20;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
    RECT clientrect;
    RECT rect;
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement *root = NULL;
    unsigned char* buffer_orig = NULL;
    unsigned char* buffer = NULL;
    int buffer_lenth;
    FILE* fp = NULL;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_SIZE:
		break;
	case WM_KEYDOWN:
        switch (wParam) {
        case VK_PRIOR:
            top += 100;
            break;
        case VK_NEXT:
            top -= 100;
        }
        GetClientRect(hWnd, &rect);
        InvalidateRect(hWnd, &rect, true);
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &clientrect);

        rect.left = (clientrect.right - clientrect.left - 800) / 2;
        rect.right = rect.left + 800;
        rect.top = top;

        buffer_lenth = lstrlen(html) * 2;

        buffer_orig = (unsigned char*)malloc(buffer_lenth);
        if (NULL == buffer_orig)
            goto END_PAINT;

        memset(buffer_orig, 0, buffer_lenth);

        buffer = (unsigned char*)malloc(buffer_lenth);
        if (NULL == buffer)
            goto END_PAINT;

        memset(buffer, 0, buffer_lenth);

        WideCharToMultiByte(CP_UTF8, 0, html, lstrlen(html), (char*)buffer_orig, buffer_lenth, NULL, NULL);

        preprocess((char *)buffer_orig, (char*)buffer);

        fp = fopen("debug.html", "w");
        fwrite(buffer, 1, strlen((char*)buffer), fp);
        fflush(fp);
        fclose(fp);

        doc.Parse((const char*)buffer);

        root = doc.RootElement();
        if (root) {
            node_scan(hdc, root, buffer, buffer_lenth, &rect);
        }
        else {
            buffer = (unsigned char*)malloc(1024);
            if (NULL != buffer) {
                memset(buffer, 0, 1024);
                MultiByteToWideChar(CP_UTF8, 0, (char*)doc.ErrorStr(), strlen(doc.ErrorStr()), (wchar_t*)buffer, 1024);
                draw_text(hdc, (wchar_t*)buffer, &rect, 0xFFFFFF, 0x000000, 20);
            }
        }

    END_PAINT:
        if (buffer_orig)
            free(buffer_orig);

        if (buffer)
            free(buffer);

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

int HTMLBoardRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("HTMLBoard");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

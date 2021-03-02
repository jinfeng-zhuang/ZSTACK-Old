#include <Windows.h>
#include <zstack.h>

static WCHAR email[] = TEXT("\
On Tue, 17 Dec 2002, Linus Torvalds wrote :\n\
>\n\
> Which is ok for a regular fast system call(ebp will get restored\n\
> immediately), but it is NOT ok for the system call restart case, since in\n\
> that case we want % ebp to contain the old stack pointer, not the sixth\n\
> argument.\n");


static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	int i;
	TEXTMETRICW tm;
    RECT rect;
    RECT clientrect;
    DRAWTEXTPARAMS drawtextparams;
    COLORREF colorref;
    COLORREF bgcolor;
    LOGFONT logicfont;
    HFONT hfont;
    SIZE size;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	switch (uMsg)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc, &tm);
		ReleaseDC(hWnd, hdc);
		break;
	case WM_SIZE:
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &clientrect);

        memset(&logicfont, 0, sizeof(logicfont));
        wsprintf(logicfont.lfFaceName, TEXT("Consolas"));
        logicfont.lfCharSet = GB2312_CHARSET;
        logicfont.lfHeight = 20;
        hfont = CreateFontIndirect(&logicfont);

        colorref = 0xFF0000;
        bgcolor = 0xFFFFFF;

        drawtextparams.cbSize = sizeof(DRAWTEXTPARAMS);
        drawtextparams.iTabLength = 2;
        drawtextparams.iLeftMargin = 1;
        drawtextparams.iRightMargin = 1;

        SelectObject(hdc, hfont);
        SetBkColor(hdc, bgcolor);
        SetTextColor(hdc, colorref);

        rect.top = 220;
        rect.left = 0;
        rect.right = clientrect.right;
        rect.bottom = 400;

        //GetTextExtentPoint(hdc, L"HELLO WORD END", lstrlen(L"HELLO WORD END"), &size);
        //SetTextJustification(hdc, clientrect.right - clientrect.left - size.cx, 2);
        DrawTextEx(hdc, email, -1, &rect, DT_WORDBREAK | DT_EXPANDTABS, &drawtextparams);

        //SetTextJustification(hdc, 0, 1);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int ArticleBoardRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("ArticleBoard");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

#include <Windows.h>
#include <zstack.h>

extern HBITMAP hBitmap;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
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
        
        SelectObject(hdc, GetStockObject(SYSTEM_FONT));

        GetTextExtentPoint(hdc, L"HELLO", 5, &size);
        wprintf(L"width %d height %d\n", size.cx, size.cy);

        TextOut(hdc, 0, 0, L"TextOut", 7);

        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

        GetTextExtentPoint(hdc, L"HELLO", 5, &size);
        wprintf(L"width %d height %d\n", size.cx, size.cy);
        
        TabbedTextOut(hdc, 0, 20, L"\tTextOut", 8, NULL, NULL, 0);

        SelectObject(hdc, GetStockObject(OEM_FIXED_FONT));

        ExtTextOut(hdc, 0, 40, 0, NULL, L"TextOut", 7, NULL);

        bgcolor = 0xC0C0C0;
        rect.top = 60;
        rect.left = 0;
        rect.right = 100;
        rect.bottom = 100;

        //SetBkMode(hdc, TRANSPARENT);
        SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
        SetBkColor(hdc, bgcolor);
        DrawText(hdc, L"Text Hello World\nOut", -1, &rect, DT_RIGHT | DT_WORDBREAK | DT_EXPANDTABS);

        memset(&logicfont, 0, sizeof(logicfont));
        wsprintf(logicfont.lfFaceName, TEXT("Consolas"));
        //logicfont.lfItalic = true;
        logicfont.lfCharSet = GB2312_CHARSET;
        logicfont.lfHeight = 20;

        colorref = 0xFF0000;
        bgcolor = 0xFFFFFF;

        rect.top = 120;
        rect.left = 0;
        rect.right = 100;
        rect.bottom = 200;

        drawtextparams.cbSize = sizeof(DRAWTEXTPARAMS);
        drawtextparams.iTabLength = 2;
        drawtextparams.iLeftMargin = 1;
        drawtextparams.iRightMargin = 1;
        
        hfont = CreateFontIndirect(&logicfont);
        SelectObject(hdc, hfont);

        GetTextExtentPoint(hdc, L"HELLO", 5, &size);
        wprintf(L"width %d height %d\n", size.cx, size.cy);

        SetBkColor(hdc, bgcolor);
        SetTextColor(hdc, colorref);
        DrawTextEx(hdc, (WCHAR *)L"\tText Hello World\nOut", -1, &rect, DT_WORDBREAK | DT_EXPANDTABS, &drawtextparams);

        rect.top = 220;
        rect.left = 0;
        rect.right = clientrect.right;
        rect.bottom = 400;

        GetTextExtentPoint(hdc, L"HELLO WORD END", lstrlen(L"HELLO WORD END"), &size);
        SetTextJustification(hdc, clientrect.right - clientrect.left - size.cx, 2);
        DrawTextEx(hdc, (WCHAR*)L"HELLO WORD END", -1, &rect, DT_WORDBREAK | DT_EXPANDTABS, &drawtextparams);

        SetTextJustification(hdc, 0, 1);

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

int TextBoardRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("TextBoard");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

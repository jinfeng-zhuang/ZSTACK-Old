#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <commctrl.h>
#include <zstack/zstack.h>

#define WIDTH	20
#define TOP		10

static struct layout layout[] = {
	// Toolbar
	{"Toolbar", "ToolbarWindow32", {0, TOP, WIDTH, 5}, TBSTYLE_TOOLTIPS},

	// Input
	{"Version", "Static", {0, TOP, WIDTH, 5}},
	{"ESRING", "RingBuffer", {0, TOP+10, WIDTH, 5}},
	{"ESDUMP", "BUTTON", {0, TOP+20, WIDTH, 5}, BS_AUTOCHECKBOX},
	{"Metadata", "BUTTON", {0, TOP+25, WIDTH, 5}, BS_AUTOCHECKBOX},
	{"ShareInfo",	"AVMIPSVideo_Input", {0, TOP+35, WIDTH, 40}},

	// Decoder
	{"DECODER", "AVMIPSVideo_Decoder", {25, TOP, WIDTH, 50}},

	// AVSync
	{"AVSYNC_OnOff", "BUTTON", {50, TOP, WIDTH, 10}, BS_AUTOCHECKBOX},
	{"AVSYNC", "AVMIPSVideo_AVSync", {50, TOP+15, WIDTH, 50}},

	// Output
	{"FrameBuffer",	"AVMIPSVideo_FrameBuffer", {75, TOP, WIDTH, 20}},
	{"YUVWindow",	"YUV", {75, TOP+25, WIDTH, 20}},
	{"MPEGFormat",	"AVMIPSVideo_MpegFormat", {75, TOP+50, WIDTH, 40}},
	
	{NULL}
};

int toolbar_init_standard(HWND hwnd)
{
	TBBUTTON tbBtn;
	TBADDBITMAP tbBitmap;

	memset(&tbBtn, 0, sizeof(TBBUTTON));
	tbBtn.iBitmap = STD_FILENEW;
	tbBtn.fsState = TBSTATE_ENABLED;
	tbBtn.fsStyle = TBSTYLE_BUTTON;
	tbBtn.iString = TEXT("Open");

	tbBitmap.hInst = HINST_COMMCTRL;
	tbBitmap.nID = IDB_STD_SMALL_COLOR;

	SendMessage(hwnd, TB_ADDBITMAP, 0, (LPARAM)&tbBitmap);
	SendMessage(hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hwnd, TB_ADDBUTTONS, (WPARAM)1, (LPARAM)&tbBtn);

	return 0;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance;
	RECT rc;
	HWND hwndTmp;
	const char* name;

	switch (uMsg)
	{
	case WM_CREATE:
		layout_create(hWnd, layout);
		
		hwndTmp = layout_find_hwnd(layout, "Toolbar");
		if (hwndTmp) {
			toolbar_init_standard(hwndTmp);
		}
		break;
	case WM_SIZE:
		layout_resize(hWnd, layout);
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		name = layout_find_name(layout, lParam);
		if (0 == strcmp(name, "Step")) {
			avmips_set_step_flag(TRUE);
			
		}
		else if (0 == strcmp(name, "Play")) {
			avmips_set_step_flag(FALSE);
		}
		break;
	case WM_USER:
		if (NULL == wParam)
			break;

		hwndTmp = layout_find_hwnd(layout, wParam);
		if (NULL == hwndTmp)
			break;

		if (0 == strcmp(wParam, "Version")) {
			SendMessage(hwndTmp, WM_SETTEXT, 0, lParam);
		}
		else {
			SendMessage(hwndTmp, WM_USER, 0, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Class_AVMIPSVideo_Register(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("AVMIPSVideo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

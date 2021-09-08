#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <zstack/zstack.h>

#if 0
struct UI layout[] = {
	{"ES BUFFER",	"AVMIPSVideo_MpegFormat",	NULL, {0, 0, 30, 50}},
	{"DECODER",		"AVMIPSVideo_Decoder",		NULL, {50, 0, 30, 50}},
	{"AVSYNC",		"AVMIPSVideo_MpegFormat",	NULL, {0, 50, 30, 50}},
	{"DISPLAY",		"YUV",	NULL, {50, 50, 30, 50}},
};
#endif
struct UI layout[] = {
	{"DISPLAY",		"YUV",	NULL, {10, 10, 80, 80}},
};

static void Init(HWND hWnd, HINSTANCE hInstance)
{
	HWND hwnd;
	RECT rc;
	struct layout_rect lr = { 0 };
	int i;
	HWND group;
	unsigned char* buffer = malloc(1920 * 1080 * 3 / 2);
	struct YUV yuv;

	GetClientRect(hWnd, &rc);

	for (i = 0; i < ARRAY_SIZE(layout); i++) {
		layout_percent_to_real(&rc, &layout[i].rect, &lr);

		layout[i].hwnd = CreateWindowEx(
			0,
			layout[i].classname,
			NULL,
			WS_CHILD | WS_VISIBLE,
			lr.x, lr.y, lr.w, lr.h,
			hWnd,
			NULL,
			NULL,
			NULL);
	}

	file_load("C:\\Users\\jinfengz\\test.yuv", 0, buffer, 1920 * 1080 * 3 / 2);

	yuv.format = YU12;
	yuv.width = 1920;
	yuv.height = 1080;
	yuv.buffer = buffer;

	SendMessage(layout[0].hwnd, WM_USER, 0, (LPARAM)&yuv);
}

static void Update(HWND hWnd)
{
	HWND hwnd;
	RECT rc;
	struct layout_rect lr = { 0 };
	int i;

	GetClientRect(hWnd, &rc);

	for (i = 0; i < ARRAY_SIZE(layout); i++) {
		layout_percent_to_real(&rc, &layout[i].rect, &lr);

		MoveWindow(layout[i].hwnd, lr.x, lr.y, lr.w, lr.h, TRUE);
	}
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance;
	RECT rc;

	switch (uMsg)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		Init(hWnd, hInstance);
		break;
	case WM_SIZE:
		Update(hWnd);
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		break;
	case WM_USER:
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

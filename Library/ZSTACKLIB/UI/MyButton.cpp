#include <Windows.h>
#include <zstack.h>

struct window_desc {
    int x;
    int y;
    int width;
    int height;
    int style;
    wchar_t value[32];
};

struct window_desc vertical_buttons[] = {
	{0, 0, 200, 20, BS_PUSHBUTTON, TEXT("BS_PUSHBUTTON")},
	{0, 0, 200, 20, BS_DEFPUSHBUTTON, TEXT("BS_DEFPUSHBUTTON")},
	{0, 0, 200, 20, BS_CHECKBOX, TEXT("BS_CHECKBOX")},
	{0, 0, 200, 20, BS_AUTOCHECKBOX, TEXT("BS_AUTOCHECKBOX")},
	{0, 0, 200, 20, BS_RADIOBUTTON, TEXT("BS_RADIOBUTTON")},
	{0, 0, 200, 20, BS_3STATE, TEXT("BS_3STATE")},
	{0, 0, 200, 20, BS_AUTO3STATE, TEXT("BS_AUTO3STATE")},
	{0, 0, 200, 20, BS_GROUPBOX, TEXT("BS_AUTO3STATE")},
	{0, 0, 200, 20, BS_USERBUTTON, TEXT("BS_USERBUTTON")},
	{0, 0, 200, 20, BS_AUTORADIOBUTTON, TEXT("BS_AUTORADIOBUTTON")},
	{0, 0, 200, 20, BS_PUSHBOX, TEXT("BS_PUSHBOX")},
	{0, 0, 200, 20, BS_OWNERDRAW, TEXT("BS_OWNERDRAW")},
};

static void window_position_calc(void)
{
	int i;
	int x_offset, y_offset;
	int vertical_gap = 10;

	x_offset = 0;
	y_offset = 0;

	for (i = 0; i < sizeof(vertical_buttons) / sizeof(vertical_buttons[0]); i++) {
		vertical_buttons[i].x = x_offset;
		vertical_buttons[i].y = y_offset;

		y_offset += vertical_buttons[i].height + vertical_gap;
	}
}

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
		window_position_calc();

		for (i = 0; i < sizeof(vertical_buttons) / sizeof(vertical_buttons[0]); i++) {
			CreateWindowEx(
				0, // dwExStyle
				TEXT("Button"),
				vertical_buttons[i].value, // Title
				WS_CHILD | WS_VISIBLE | vertical_buttons[i].style,
				vertical_buttons[i].x,
				vertical_buttons[i].y,
				vertical_buttons[i].width,
				vertical_buttons[i].height,
				hWnd, // hWndParent
				NULL, // hMenu
				hInstance,
				NULL);
		}
		break;
	case WM_SIZE:
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

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

int MyButtonRegister(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("MyButton");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

#include <Windows.h>
#include <commctrl.h>

static HWND ctrl;

static void insert_item(int row, char *id, char *value)
{
	LVITEM item;

	memset(&item, 0, sizeof(LVITEM));
	item.mask = LVIF_TEXT;
	item.iItem = row;
	ListView_InsertItem(ctrl, &item);
	ListView_SetItemText(ctrl, row, 0, id);
	ListView_SetItemText(ctrl, row, 1, value);
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT client;
	int evt;
	int sel;
	LV_COLUMN col;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &client);

	switch (uMsg)
	{
	case WM_CREATE:
		ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("SysListView32"), NULL,
			WS_CHILD | WS_VISIBLE | LVS_REPORT,
			client.left, client.top, client.right, client.bottom,
			hWnd, NULL, NULL, NULL);

		col.mask = LVCF_TEXT | LVCF_WIDTH;
		col.pszText = TEXT("ID");
		col.cx = 60;
		SendMessage(ctrl, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

		col.mask = LVCF_TEXT | LVCF_WIDTH;
		col.pszText = TEXT("Value");
		col.cx = 60;
		SendMessage(ctrl, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

		insert_item(0, "x", "0");
		insert_item(1, "y", "0");

		//SendMessage(ctrl, LVM_SETTEXTCOLOR, 0, RGB(255, 255, 255));
		//SendMessage(ctrl, LVM_SETBKCOLOR, 0, RGB(100, 100, 100));
		//SendMessage(ctrl, LVM_SETTEXTBKCOLOR, 0, RGB(0, 0, 0));
		break;
	case WM_SIZE:
		MoveWindow(ctrl, client.left, client.top, client.right - client.left, client.bottom - client.top, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		evt = HIWORD(wParam);
		switch (evt) {
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

int Init_SysListView32Demo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("SysListView32Demo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

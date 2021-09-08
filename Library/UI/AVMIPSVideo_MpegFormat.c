#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <commctrl.h>
#include <zstack/zstack.h>

static HWND ctrl;
static HWND group;

static void insert_item(HWND ctrl, int row, char* id, char* value)
{
	LVITEM item;

	memset(&item, 0, sizeof(LVITEM));
	item.mask = LVIF_TEXT;
	item.iItem = row;
	ListView_InsertItem(ctrl, &item);
	ListView_SetItemText(ctrl, row, 0, id);
	ListView_SetItemText(ctrl, row, 1, value);
}

static void Init(HWND hWnd, HINSTANCE hInstance)
{
	LV_COLUMN col;
	RECT rc;

	GetClientRect(hWnd, &rc);

	group = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("BUTTON"), TEXT("MpegFormat"),
		WS_CHILD | WS_VISIBLE | WS_GROUP | BS_GROUPBOX,
		rc.left, rc.top, rc.right, rc.bottom,
		hWnd, NULL, NULL, NULL);

	GetClientRect(group, &rc);

	ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("SysListView32"), NULL,
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		rc.left, rc.top, rc.right, rc.bottom,
		group, NULL, NULL, NULL);

	col.mask = LVCF_TEXT | LVCF_WIDTH;
	col.pszText = TEXT("Name");
	col.cx = 60;
	SendMessage(ctrl, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

	col.mask = LVCF_TEXT | LVCF_WIDTH;
	col.pszText = TEXT("Value");
	col.cx = 60;
	SendMessage(ctrl, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

	insert_item(ctrl, 0, "HDR Mode", "N/A");
	insert_item(ctrl, 1, "ES Buffer", "N/A");
	insert_item(ctrl, 1, "Stream", "N/A");
	insert_item(ctrl, 1, "Profile", "N/A");
	insert_item(ctrl, 1, "Level", "N/A");
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
		GetClientRect(hWnd, &rc);
		MoveWindow(group, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		GetClientRect(group, &rc);
		MoveWindow(ctrl, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		break;
	case WM_USER:
		// set column 1 always
		ListView_SetItemText(ctrl, wParam, 1, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Class_AVMIPSVideo_MpegFormat_Register(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("AVMIPSVideo_MpegFormat");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

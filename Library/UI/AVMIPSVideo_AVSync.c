#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <commctrl.h>
#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

static HWND ctrl;
static triVideoSharedInfo_t shareinfo;

static const char* items[] = {
	"Status", "PTS (ms?)", "Delta",
};

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
	int i;

	GetClientRect(hWnd, &rc);

	ctrl = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("SysListView32"), NULL,
		WS_CHILD | WS_VISIBLE | LVS_REPORT,
		rc.left, rc.top, rc.right, rc.bottom,
		hWnd, NULL, NULL, NULL);

	col.mask = LVCF_TEXT | LVCF_WIDTH;
	col.pszText = TEXT("Name");
	col.cx = 60;
	SendMessage(ctrl, LVM_INSERTCOLUMN, 0, (LPARAM)&col);

	col.mask = LVCF_TEXT | LVCF_WIDTH;
	col.pszText = TEXT("Value");
	col.cx = 60;
	SendMessage(ctrl, LVM_INSERTCOLUMN, 1, (LPARAM)&col);

	for (i = 0; i < ARRAY_SIZE(items); i++) {
		insert_item(ctrl, i, items[i], "N/A");
	}
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance;
	RECT rc;
	char tmp[64];

	switch (uMsg)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		Init(hWnd, hInstance);
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &rc);
		MoveWindow(ctrl, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		break;
	case WM_USER:
		if (NULL == lParam)
			break;

		memcpy(&shareinfo, lParam, sizeof(triVideoSharedInfo_t));

		snprintf(tmp, sizeof(tmp), "%d", shareinfo.bVideoSyncOn);
		ListView_SetItemText(ctrl, 0, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%lld", shareinfo.VideoPTS);
		ListView_SetItemText(ctrl, 1, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%lld", shareinfo.VideoPtsStcDelta);
		ListView_SetItemText(ctrl, 2, 1, tmp);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Class_AVMIPSVideo_AVSync_Register(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("AVMIPSVideo_AVSync");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

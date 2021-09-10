#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <commctrl.h>
#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

static HWND ctrl;
static HWND group;
static struct VideoFrameInfo frameinfo;

static const char* items[] = {
	"Width", "Height", "PTS", "Codec", "Profile", "Level"
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
		MoveWindow(group, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		GetClientRect(group, &rc);
		MoveWindow(ctrl, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		break;
	case WM_USER:
		if (NULL == lParam)
			break;

		memcpy(&frameinfo, lParam, sizeof(struct VideoFrameInfo));

		snprintf(tmp, sizeof(tmp), "%d", frameinfo.format.wHsize);
		ListView_SetItemText(ctrl, 0, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%d", frameinfo.format.wVsize);
		ListView_SetItemText(ctrl, 1, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%d", frameinfo.tPrivData.PTS);
		ListView_SetItemText(ctrl, 2, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%d", frameinfo.tProfileLvl.VideoStandard);
		ListView_SetItemText(ctrl, 3, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%d", frameinfo.tProfileLvl.VideoProfile);
		ListView_SetItemText(ctrl, 4, 1, tmp);

		snprintf(tmp, sizeof(tmp), "%d", frameinfo.tProfileLvl.VideoLevel);
		ListView_SetItemText(ctrl, 5, 1, tmp);
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

#include <Windows.h>
#include <commctrl.h>

HTREEITEM AddItemToTree(HWND hwndTV, LPSTR lpszItem, HTREEITEM hParent)
{
	TVITEM tParent;
	TVINSERTSTRUCT tvins;
	HTREEITEM hme;
	TVITEMA tvi;

	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_DI_SETITEM | TVIF_PARAM;
	tvi.pszText = lpszItem;

	tvins.item = tvi;
	tvins.hInsertAfter = TVI_ROOT;
	if (hParent == NULL) {
		tvins.hParent = TVI_ROOT;
	}
	else {
		tvins.hParent = hParent;
	}

	hme = TreeView_InsertItem(hwndTV, &tvins);
	return hme;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HINSTANCE hInstance;
	RECT clientrect;
	HWND treeview;

	//printf("Window Message: 0x%04X\n", uMsg);

	hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

	GetClientRect(hWnd, &clientrect);

	switch (uMsg)
	{
	case WM_CREATE:
		treeview = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, NULL,
			WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
			0, 0, 400, 400, hWnd, NULL, NULL, NULL);

		HTREEITEM hRoot = AddItemToTree(treeview, "VideoFrameInfo_t", NULL);
		HTREEITEM hp = AddItemToTree(treeview, "ulStartCode", hRoot);
		hp = AddItemToTree(treeview, "tInfoVer", hRoot);
		HTREEITEM hp1 = AddItemToTree(treeview, "mpg_ver = 1", hp);
		hp1 = AddItemToTree(treeview, "ext_ver = 1", hp);
		break;
	case WM_SIZE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

int Init_TreeViewDemo(HINSTANCE hInstance)
{
	WNDCLASSEX wce = { 0 };

	wce.cbSize = sizeof(wce);
	wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wce.hCursor = LoadCursor(NULL, IDC_ARROW);
	wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wce.hInstance = hInstance;
	wce.lpfnWndProc = WindowProc;
	wce.lpszClassName = TEXT("TreeViewDemo");
	wce.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wce)) {
		return 1;
	}

	return 0;
}

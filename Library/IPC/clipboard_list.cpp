#include <Windows.h>
#include <stdio.h>

static HBITMAP hBitmap;

extern const wchar_t* GetPredefinedClipboardFormatName(UINT format);
extern void hex_dump(unsigned char* buffer, unsigned int length);

static void CF_TEXT_proc(void)
{
	HGLOBAL global_memory = GetClipboardData(CF_TEXT);

	LPCSTR clipboard_data = (LPCSTR)GlobalLock(global_memory);
	DWORD data_size = GlobalSize(global_memory);

	printf("Data Size = %d\n", data_size);
	printf("%s\n", clipboard_data);

	GlobalUnlock(global_memory);
}

static void CF_BITMAP_proc(void)
{
	hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
}

static void CF_HTML_proc(void)
{
	HGLOBAL global_memory = (HBITMAP)GetClipboardData(0xC0EB);
	LPCSTR clipboard_data = (LPCSTR)GlobalLock(global_memory);
	GlobalUnlock(global_memory);
}

void clipboard_list(void)
{
	if (OpenClipboard(NULL)) {
		printf("CountClipboardFormats = %d\n", CountClipboardFormats());
		UINT format = 0;
		wchar_t name[100];
		const wchar_t* pName;

		while (1) {
			format = EnumClipboardFormats(format);
			if (0 == format)
				break;

			pName = GetPredefinedClipboardFormatName(format);
			if (pName) {
				wprintf(TEXT("%04X: %s\n"), format, pName);
			}
			else {
				if (GetClipboardFormatName(format, name, 100))
					wprintf(TEXT("%04X: %s\n"), format, name);
				else
					wprintf(TEXT("%04X: Unknown\n"), format);
			}

			switch (format) {
			case CF_TEXT:
				CF_TEXT_proc();
				break;
			case CF_BITMAP:
				CF_BITMAP_proc();
				break;
			case 0xC0EB:
				CF_HTML_proc();
				break;
			}
			printf("\n");
		}
		CloseClipboard();
	}
}

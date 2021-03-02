#include <Windows.h>

static const wchar_t *name[] = {
	L"CF_RSVD",
	L"CF_TEXT",
	L"CF_BITMAP",
	L"CF_METAFILEPICT",
	L"CF_SYLK",
	L"CF_DIF",
	L"CF_TIFF",
	L"CF_OEMTEXT",
	L"CF_DIB",
	L"CF_PALETTE",
	L"CF_PENDATA",
	L"CF_RIFF",
	L"CF_WAVE",
	L"CF_UNICODETEXT",
	L"CF_ENHMETAFILE",
#if(WINVER >= 0x0400)
	L"CF_HDROP",
	L"CF_LOCALE",
#endif
#if(WINVER >= 0x0500)
	L"CF_DIBV5",
#endif
};

const wchar_t *GetPredefinedClipboardFormatName(UINT format)
{
	if (0 == format)
		return NULL;

	if (format < CF_MAX)
		return name[format];

	return NULL;
}

#include <Windows.h>

#include  <zstack/misc.h>

extern int FieldMergeWindow_Register(HINSTANCE hInstance);
extern int FieldMergeWindowEx_Register(HINSTANCE hInstance);
extern int YUVWindow_Register(HINSTANCE hInstance);
extern int HexView_Register(HINSTANCE hInstance);

extern int MyButtonRegister(HINSTANCE hInstance);
extern int EmptyBoardRegister(HINSTANCE hInstance);
extern int TextBoardRegister(HINSTANCE hInstance);
extern int ArticleBoardRegister(HINSTANCE hInstance);
extern int HTMLBoardRegister(HINSTANCE hInstance);
extern int BitmapBoardRegister(HINSTANCE hInstance);
extern int LogBoardRegister(HINSTANCE hInstance);
extern int ScrollBoardRegister(HINSTANCE hInstance);
extern int ListBoardRegister(HINSTANCE hInstance);
extern int PaintBoardRegister(HINSTANCE hInstance);
extern int TVBoardRegister(HINSTANCE hInstance);
extern int RingBufferBoardRegister(HINSTANCE hInstance);
extern int MenuBoardRegister(HINSTANCE hInstance);
extern int CardBoardRegister(HINSTANCE hInstance);
extern int GameBoardRegister(HINSTANCE hInstance);
extern int Game2BoardRegister(HINSTANCE hInstance);
extern int CurveBoardRegister(HINSTANCE hInstance);
extern int DoubleBufferBoardRegister(HINSTANCE hInstance);

int Platform_Register(HINSTANCE hInstance)
{
    FieldMergeWindow_Register(hInstance);
    FieldMergeWindowEx_Register(hInstance);

    return 0;
}

int window_class_register(void)
{
    HINSTANCE hInstance = NULL;

    YUVWindow_Register(NULL);
    FieldMergeWindow_Register(NULL);
    FieldMergeWindowEx_Register(NULL);
    HexView_Register(NULL);

#if 1
	MyButtonRegister(hInstance);
	EmptyBoardRegister(hInstance);
	TextBoardRegister(hInstance);
    ArticleBoardRegister(hInstance);
    HTMLBoardRegister(hInstance);
    BitmapBoardRegister(hInstance);
    LogBoardRegister(hInstance);
    ScrollBoardRegister(hInstance);
    ListBoardRegister(hInstance);
    PaintBoardRegister(hInstance);
    TVBoardRegister(hInstance);
    RingBufferBoardRegister(hInstance);
    MenuBoardRegister(hInstance);
    CardBoardRegister(hInstance);
    GameBoardRegister(hInstance);
    Game2BoardRegister(hInstance);
    CurveBoardRegister(hInstance);
    DoubleBufferBoardRegister(hInstance);
#endif

    return 0;
}

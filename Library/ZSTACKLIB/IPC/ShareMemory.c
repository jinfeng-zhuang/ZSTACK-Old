#include <windows.h>
#include <stdio.h>

#if 0
#pragma pack(1)
struct ShareData
{
    int iType;
    int iSize;
    char strData[4096];
};
#pragma pack()

int main()
{
    string strMapName("ShareMemory");                // �ڴ�ӳ���������
    string strComData("This is common data!");        // �����ڴ��е�����
    LPVOID pBuffer;                                    // �����ڴ�ָ��

    // ������ͼ��һ���������ڴ�ӳ���ļ�����  
    HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, strMapName.c_str());
    if (NULL == hMap)
    {    // ��ʧ�ܣ�����֮
        hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            strComData.length() + 1,
            strMapName.c_str());
        // ӳ������һ����ͼ���õ�ָ�����ڴ��ָ�룬�������������
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        strcpy((char*)pBuffer, strComData.c_str());
        cout << "д�빲���ڴ����ݣ�" << (char*)pBuffer << endl;
    }
    else
    {    // �򿪳ɹ���ӳ������һ����ͼ���õ�ָ�����ڴ��ָ�룬��ʾ�����������
        pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        cout << "��ȡ�����ڴ����ݣ�" << (char*)pBuffer << endl;
    }

    getchar();            // ע�⣬���̹رպ����о���Զ��رգ�����Ҫ��������ͣ

    // ����ļ�ӳ�䣬�ر��ڴ�ӳ���ļ�������
    ::UnmapViewOfFile(pBuffer);
    ::CloseHandle(hMap);
    system("pause");
    return 0;
}
#endif

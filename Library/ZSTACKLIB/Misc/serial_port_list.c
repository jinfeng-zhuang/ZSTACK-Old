#ifdef _WIN32

#include <Windows.h>
#include <stdio.h>

#define NAME_LEN 100

void serial_port_list(void)
{
    HKEY hKey;
    LPCTSTR lpSubKey = TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");
    char szValueName[NAME_LEN];
    BYTE szPortName[NAME_LEN];
    LONG status;
    DWORD dwIndex = 0;
    DWORD dwSizeValueName;
    DWORD dwSizeofPortName;
    DWORD Type;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
        return;
    }

    do {
        dwSizeValueName = NAME_LEN;
        dwSizeofPortName = NAME_LEN;

        status = RegEnumValue(hKey, dwIndex++, szValueName, &dwSizeValueName, NULL, &Type, szPortName, &dwSizeofPortName);
        if (status == ERROR_SUCCESS) {
            //wprintf(L"%s\n", (wchar_t *)szPortName);
            printf("%s: %x\n", (char *)szPortName, Type);
            fflush(stdout);
        }
    } while ((status != ERROR_NO_MORE_ITEMS));
    
    RegCloseKey(hKey);
}
#endif

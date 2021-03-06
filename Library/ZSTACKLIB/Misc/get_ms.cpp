#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <Windows.h>
#include <time.h>

unsigned int get_ms(void)
{
    return GetTickCount();
}

unsigned int get_date_time(struct tm *tm)
{
    time_t tt;
    struct tm *lt;

    tt = time(NULL);

    lt = localtime(&tt);

    memcpy(tm, lt, sizeof(struct tm));

    return 0;
}

#else
unsigned int get_ms(void)
{
    return 0;
}
#endif

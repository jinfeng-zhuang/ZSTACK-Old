#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <time.h>
#include <zstack/time.h>

void get_time(struct time* t)
{
    time_t tt;
    struct tm* lt;

    tt = time(NULL);

    lt = localtime(&tt);

    t->year = lt->tm_year + 1900;
    t->month = lt->tm_mon + 1;
    t->day = lt->tm_mday;
    t->hour = lt->tm_hour;
    t->min = lt->tm_min;
    t->sec = lt->tm_sec;
}

u64 get_ms(void)
{
    return GetTickCount64();
}

void time_msleep(u32 ms)
{
    Sleep(ms);
}

u32 hr_timer_freq(void)
{
    LARGE_INTEGER freq;

    QueryPerformanceFrequency(&freq);

    return (u32)freq.QuadPart;
}

u64 hr_timer_counter(void)
{
    LARGE_INTEGER counter;

    QueryPerformanceCounter(&counter);

    return (u64)counter.QuadPart;
}

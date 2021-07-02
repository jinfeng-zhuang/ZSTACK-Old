#ifndef ZSTACK_TIME
#define ZSTACK_TIME

#include "types.h"

struct time {
    u32 year;
    u32 month;
    u32 day;
    u32 hour;
    u32 min;
    u32 sec;
};

extern void msleep(u32 ms);
extern void mdelay(u32 ms);
extern void udelay(u32 ms);

extern u64 get_ms(void);
extern u64 get_second(void);
extern void get_time(struct time *dt);

extern u32 hr_timer_freq(void);
extern u64 hr_timer_counter(void);

#endif
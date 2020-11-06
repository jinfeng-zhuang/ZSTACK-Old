#include <stdio.h>

#include <zstack/log.h>

const char *usage = 
"avapp - AVMIPS & HOST interface, build @ %s\n"
"usage: avapp <ip> [options]...\n"
"options:\n"
"\t-d dump <n>      dump es data from channel [x]\n"
"\t-s show          display while receive from network\n"
"\t-w window        UI show ring buffer\n"
"\t-d debug <lvl>   debug level\n"
"\t-l loop      show mpeg format periodically\n";

void print_usage(void)
{
    printf(usage, __DATE__);
}

#include <stdio.h>

#include <zstack/log.h>

const char *usage = 
"avapp - AVMIPS & HOST interface, build @ %s\n"
"usage: avapp <ip> [options]...\n"
"options:\n"
"\t-d dump <n>      dump es data from channel [x]\n"
"\t-t type <s>      pts or es\n"
"\t-s show          display while receive from network\n"
"\t-w window        UI show ring buffer\n"
"\t-d debug <conf>  debug config: \"default:3,ui:3\"\n"
"example:\n"
"\tavapp 10.86.79.123 -t \"pts\" -d \"avmips:3\" -w";

void print_usage(void)
{
    printf(usage, __DATE__);
}

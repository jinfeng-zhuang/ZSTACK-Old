#include "application.h"

const char *usage = 
"fieldmerge - build @ %s\n"
"usage: fieldmerge [options]... <output>\n"
"options:\n"
"\t-d dump <n>                      \n"
"\t-r resolution <720x288>          Field Resolution\n"
"\t-t top <filename:index>          \n"
"\t-w window                        \n"
"\t-b bottom <filename:index>       \n";

void print_usage(void)
{
    printf(usage, __DATE__);
}

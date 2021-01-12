const char *usage =
"\
Usage: aves <IP> <FILE> [OPTION]...\n\
\n\
If output file not specified, it will not dump es data.\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
  \n\
  --channel <0|1|2>\n\
\n\
Examples:\n\
  aves --log \"app:5\"\n\
  aves 10.86.79.111 --channel=1\n\
  aves 10.86.79.111 dump.es\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";
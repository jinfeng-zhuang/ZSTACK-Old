const char *usage =
"\
Usage: sxmemr <IP> [OPTION]... [FILE]...\n\
\n\
[DESCRIPTION]\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
  \n\
  --addr\n\
  --size\n\
\n\
Examples:\n\
  sxmemr --log \"app:5\"\n\
  sxmemr 10.86.79.111 --addr=0x11700000 --size=0x100000 | xargs ffplay\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";
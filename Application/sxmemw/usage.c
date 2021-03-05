const char *usage =
"\
Usage: sxmemw <IP> [OPTION]... [FILE]...\n\
\n\
[DESCRIPTION]\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
  \n\
  --addr\n\
  --size\n\
  --pman_pass\n\
\n\
Examples:\n\
  sxmemw --log \"app:5\"\n\
  sxmemw 10.86.79.111 --addr=0x11700000 --size=0x100000 demo.bin\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";
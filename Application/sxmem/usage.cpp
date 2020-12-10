const char *usage =
"\
Usage: sxmem [OPTION]... [FILE]...\n\
\n\
This is a template project to generate a new one.\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
\n\
Examples:\n\
  sxmem --log \"app:5\"\n\
  sxmem 10.86.40.89 --address=0x11000000 --bytecount=1024 --operation=write --format=\"struct\" --data=\"4 : 1 12 0x22, 2 : 2 5 0xFF\"\n\
  sxmem 10.86.40.89 --address=0x11000000 --bytecount=1024 --operation=write --format=\"binary\" --datafile=\"input.bin\"\n\
  sxmem 10.86.40.89 --address=0x11000000 --bytecount=1024 --operation=write --format=\"hexdump\" --datafile=\"input.txt\"\n\
  sxmem 10.86.40.89 --address=0x11000000 --bytecount=1024 --operation=write --format=\"hexdump\" --data=\"11 22 FF C0 C0 C0\"\n\
  sxmem 10.86.40.89 --address=0x11000000 --bytecount=1024 --operation=read --format=\"struct\" --data=\"4 : 5, 1 : 6\"\n\
  sxmem 10.86.40.89 --address=0x11000000 --bytecount=1024 --operation=read --format=\"binary\" --datafile=\"output.bin\"\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";

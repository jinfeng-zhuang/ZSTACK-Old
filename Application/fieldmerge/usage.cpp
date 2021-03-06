const char *usage =
"\
Usage: fieldmerge [OPTION]... [FILE]...\n\
\n\
[DESCRIPTION]\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
\n\
Examples:\n\
  fieldmerge --log \"app:5\"\n\
  fieldmerge --top_field_first --top=top.yuv --bottom=bottom.yuv --top_index=4 --bottom_index=5 --resolution=\"1920x1080\"\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";
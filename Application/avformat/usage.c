const char *usage =
"\
Usage: template [OPTION]... [FILE]...\n\
\n\
This is a template project to generate a new one.\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
  --frame       dump current frame decoded info\n\
  --mpeg        mpeg format info\n\
  --edr         EDR info\n\
  --hdr         HDR info\n\
  --codec       codec, profile, level\n\
  --output      output format, like compress, tile, raster mode\n\
  --dump        dump current frame to yuv file\n\
  --simple      print only different info\n\
  --reg         print mpegdisp registers\n\
  --loop        show mpeg format periodically\n\
\n\
Examples:\n\
  template --log \"app:5\"\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";
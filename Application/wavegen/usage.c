const char *usage =
"\
Usage: wavegen [OPTION]... [FILE]...\n\
\n\
This is a template project to generate a new one.\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
  --format []   output format\n\
\n\
Examples:\n\
  wavegen --log \"app:5\"\n\
  wavegen --number=2048 --freq=50 --sample=15360\n\
  wavegen --number=2048 --freq=50 --sample=15360 --format=\"c-array\"\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";
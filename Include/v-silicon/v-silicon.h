#ifndef VS_H
#define VS_H

#include "avmips.h"
#include "dbg.h"
#include "pman.h"
#include "sx.h"

extern int avmips_get_version(void);
extern unsigned int avmips_get_log_desc(void);
extern char* avmips_get_log(unsigned int index);
extern int avmips_log_init(char* setting);

#endif